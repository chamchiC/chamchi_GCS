/****************************************************************************
 *
 * (c) 2009-2024 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import QtMultimedia
import QtWebEngine

import QGroundControl
import QGroundControl.Controls
import QGroundControl.Palette
import QGroundControl.ScreenTools

Window {
    id: customWindow
    
    width: 960
    height: 320
    minimumWidth: 480
    minimumHeight: 200
    
    title: qsTr("Video Monitor")
    
    color: qgcPal.window
    
    QGCPalette { id: qgcPal; colorGroupEnabled: true }
    
    // 비디오 URL
    property string videoUrl1: "http://192.168.100.11:8080/video_feed"
    property string videoUrl2: "http://192.168.100.21:8080/video_feed"
    property string videoUrl3: "http://192.168.100.31:8080/video_feed"
    
    // Web GUI URL
    property string webGuiUrl: "http://192.168.100.11:5000"
    
    // 현재 탭 (0: 카메라, 1: WebGUI)
    property int currentTab: 0
    
    // 자동 재연결 활성화 여부
    property bool autoReconnect: true
    
    // 창이 열릴 때 두 번째 모니터 확인 후 전체화면으로 이동
    Component.onCompleted: {
        // 약간의 딜레이 후 모니터 이동 및 전체화면 적용
        moveToSecondMonitorTimer.start()
    }
    
    Timer {
        id: moveToSecondMonitorTimer
        interval: 200
        repeat: false
        onTriggered: moveToSecondMonitor()
    }
    
    // 주 모니터가 아닌 스크린 찾기 (주 모니터는 virtualX=0, virtualY=0)
    function findSecondaryScreen() {
        var screens = Qt.application.screens
        for (var i = 0; i < screens.length; i++) {
            // 주 모니터가 아닌 스크린을 찾음
            if (screens[i].virtualX !== 0 || screens[i].virtualY !== 0) {
                console.log("Found secondary screen at index " + i + ": " + screens[i].name)
                return screens[i]
            }
        }
        return null // 보조 모니터 없음
    }

    function moveToSecondMonitor() {
        var screens = Qt.application.screens
        console.log("Available screens: " + screens.length)
        
        if (screens.length > 1) {
            // 주 모니터가 아닌 스크린 찾기
            var secondScreen = findSecondaryScreen()
            
            if (secondScreen === null) {
                console.log("No secondary screen found, using screens[1]")
                secondScreen = screens[1]
            }
            
            console.log("Moving to secondary screen: " + secondScreen.name)
            console.log("Screen geometry: " + secondScreen.virtualX + ", " + secondScreen.virtualY + ", " + secondScreen.width + "x" + secondScreen.height)
            
            // 먼저 일반 창 모드로 변경
            customWindow.visibility = Window.Windowed
            
            // 모니터 및 위치 설정
            customWindow.screen = secondScreen
            customWindow.x = secondScreen.virtualX
            customWindow.y = secondScreen.virtualY
            customWindow.width = secondScreen.width
            customWindow.height = secondScreen.height
            
            // 약간의 딜레이 후 전체화면 적용
            fullscreenApplyTimer.start()
        } else {
            console.log("Only one screen available")
        }
    }
    
    Timer {
        id: fullscreenApplyTimer
        interval: 100
        repeat: false
        onTriggered: {
            customWindow.visibility = Window.FullScreen
            console.log("Video Monitor fullscreen applied on second monitor")
        }
    }
    
    // 전체화면 토글 함수
    function toggleFullScreen() {
        if (visibility === Window.FullScreen) {
            visibility = Window.Windowed
        } else {
            visibility = Window.FullScreen
        }
    }
    
    // 개별 비디오 재연결 함수
    function reconnectVideo(player, url) {
        player.stop()
        player.source = ""
        player.source = url
        player.play()
    }
    
    // 비디오 새로고침 함수 - source를 초기화하고 다시 설정
    function refreshAllVideos() {
        videoPlayer1.stop()
        videoPlayer2.stop()
        videoPlayer3.stop()
        
        videoPlayer1.source = ""
        videoPlayer2.source = ""
        videoPlayer3.source = ""
        
        refreshTimer.start()
    }
    
    Timer {
        id: refreshTimer
        interval: 500
        repeat: false
        onTriggered: {
            videoPlayer1.source = videoUrl1
            videoPlayer2.source = videoUrl2
            videoPlayer3.source = videoUrl3
            
            videoPlayer1.play()
            videoPlayer2.play()
            videoPlayer3.play()
        }
    }
    
    // 자동 재연결 타이머 (3초마다 상태 확인)
    Timer {
        id: autoReconnectTimer
        interval: 3000
        repeat: true
        running: autoReconnect && customWindow.visible
        onTriggered: {
            // 재생 중이 아니면 재연결 시도
            if (videoPlayer1.playbackState !== MediaPlayer.PlayingState) {
                console.log("Video 1 reconnecting... (state:" + videoPlayer1.playbackState + ")")
                reconnectVideo(videoPlayer1, videoUrl1)
            }
            if (videoPlayer2.playbackState !== MediaPlayer.PlayingState) {
                console.log("Video 2 reconnecting... (state:" + videoPlayer2.playbackState + ")")
                reconnectVideo(videoPlayer2, videoUrl2)
            }
            if (videoPlayer3.playbackState !== MediaPlayer.PlayingState) {
                console.log("Video 3 reconnecting... (state:" + videoPlayer3.playbackState + ")")
                reconnectVideo(videoPlayer3, videoUrl3)
            }
        }
    }
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5
        spacing: 5
        
        // 상단 버튼 영역
        RowLayout {
            Layout.fillWidth: true
            spacing: ScreenTools.defaultFontPixelWidth
            
            // 탭 버튼들
            Row {
                spacing: 2
                
                Rectangle {
                    width: ScreenTools.defaultFontPixelWidth * 8
                    height: ScreenTools.defaultFontPixelHeight * 2
                    color: currentTab === 0 ? qgcPal.buttonHighlight : qgcPal.button
                    radius: ScreenTools.defaultFontPixelHeight / 4
                    
                    QGCLabel {
                        anchors.centerIn: parent
                        text: "카메라"
                        color: currentTab === 0 ? qgcPal.buttonHighlightText : qgcPal.buttonText
                    }
                    
                    MouseArea {
                        anchors.fill: parent
                        onClicked: currentTab = 0
                        cursorShape: Qt.PointingHandCursor
                    }
                }
                
                Rectangle {
                    width: ScreenTools.defaultFontPixelWidth * 8
                    height: ScreenTools.defaultFontPixelHeight * 2
                    color: currentTab === 1 ? qgcPal.buttonHighlight : qgcPal.button
                    radius: ScreenTools.defaultFontPixelHeight / 4
                    
                    QGCLabel {
                        anchors.centerIn: parent
                        text: "Web GUI"
                        color: currentTab === 1 ? qgcPal.buttonHighlightText : qgcPal.buttonText
                    }
                    
                    MouseArea {
                        anchors.fill: parent
                        onClicked: currentTab = 1
                        cursorShape: Qt.PointingHandCursor
                    }
                }
            }
            
            Item { width: ScreenTools.defaultFontPixelWidth * 2 }
            
            QGCButton {
                text: currentTab === 0 ? qsTr("Refresh") : qsTr("새로고침")
                onClicked: currentTab === 0 ? refreshAllVideos() : webView.reload()
            }
            
            // 자동 재연결 토글 (카메라 탭에서만 표시)
            QGCCheckBox {
                visible: currentTab === 0
                text: qsTr("Auto Reconnect")
                checked: autoReconnect
                onClicked: autoReconnect = checked
            }
            
            Item { Layout.fillWidth: true }
            
            QGCButton {
                text: qsTr("Close")
                onClicked: customWindow.close()
            }
        }
        
        // WebGUI 영역 (탭 1)
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "white"
            visible: currentTab === 1
            
            WebEngineView {
                id: webView
                anchors.fill: parent
                url: webGuiUrl
                
                onLoadingChanged: function(loadRequest) {
                    if (loadRequest.status === WebEngineView.LoadFailedStatus) {
                        console.warn("Web GUI: Failed to load " + loadRequest.url)
                    }
                }
            }
        }
        
        // 비디오 영역 (가로 배치) - 탭 0
        Row {
            id: videoRow
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 5
            visible: currentTab === 0
            
            // === 첫 번째 비디오 ===
            Rectangle {
                width: (parent.width - 10) / 3
                height: parent.height
                color: "black"
                
                VideoOutput {
                    id: videoOutput1
                    anchors.fill: parent
                    fillMode: VideoOutput.PreserveAspectFit
                }
                
                MediaPlayer {
                    id: videoPlayer1
                    source: videoUrl1
                    videoOutput: videoOutput1
                    audioOutput: null
                    Component.onCompleted: play()
                    
                    onErrorOccurred: function(error, errorString) {
                        console.log("Video 1 error:", errorString)
                    }
                }
            }

            // === 두 번째 비디오 ===
            Rectangle {
                width: (parent.width - 10) / 3
                height: parent.height
                color: "black"
                
                VideoOutput {
                    id: videoOutput2
                    anchors.fill: parent
                    fillMode: VideoOutput.PreserveAspectFit
                }
                
                MediaPlayer {
                    id: videoPlayer2
                    source: videoUrl2
                    videoOutput: videoOutput2
                    audioOutput: null
                    Component.onCompleted: play()
                    
                    onErrorOccurred: function(error, errorString) {
                        console.log("Video 2 error:", errorString)
                    }
                }
            }

            // === 세 번째 비디오 ===
            Rectangle {
                width: (parent.width - 10) / 3
                height: parent.height
                color: "black"
                
                VideoOutput {
                    id: videoOutput3
                    anchors.fill: parent
                    fillMode: VideoOutput.PreserveAspectFit
                }
                
                MediaPlayer {
                    id: videoPlayer3
                    source: videoUrl3
                    videoOutput: videoOutput3
                    audioOutput: null
                    Component.onCompleted: play()
                    
                    onErrorOccurred: function(error, errorString) {
                        console.log("Video 3 error:", errorString)
                    }
                }
            }
        }
    }
}
