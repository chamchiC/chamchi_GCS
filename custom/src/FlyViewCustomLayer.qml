/****************************************************************************
 *
 * (c) 2009-2019 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 * @file
 *   @author Gus Grubba <gus@auterion.com>
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QGroundControl
import QGroundControl.Controls
import QGroundControl.Palette
import QGroundControl.ScreenTools

import Custom.Widgets

Item {
    property var parentToolInsets                       // These insets tell you what screen real estate is available for positioning the controls in your overlay
    property var totalToolInsets:   _totalToolInsets    // The insets updated for the custom overlay additions
    property var mapControl

    readonly property string noGPS:         qsTr("NO GPS")
    readonly property real   indicatorValueWidth:   ScreenTools.defaultFontPixelWidth * 7

    property var    _activeVehicle:         QGroundControl.multiVehicleManager.activeVehicle
    property real   _indicatorDiameter:     ScreenTools.defaultFontPixelWidth * 18
    property real   _indicatorsHeight:      ScreenTools.defaultFontPixelHeight
    property var    _sepColor:              qgcPal.globalTheme === QGCPalette.Light ? Qt.rgba(0,0,0,0.5) : Qt.rgba(1,1,1,0.5)
    property color  _indicatorsColor:       qgcPal.text
    property bool   _isVehicleGps:          _activeVehicle ? _activeVehicle.gps.count.rawValue > 1 && _activeVehicle.gps.hdop.rawValue < 1.4 : false
    property string _altitude:              _activeVehicle ? (isNaN(_activeVehicle.altitudeRelative.value) ? "0.0" : _activeVehicle.altitudeRelative.value.toFixed(1)) + ' ' + _activeVehicle.altitudeRelative.units : "0.0"
    property string _distanceStr:           isNaN(_distance) ? "0" : _distance.toFixed(0) + ' ' + QGroundControl.unitsConversion.appSettingsHorizontalDistanceUnitsString
    property real   _heading:               _activeVehicle   ? _activeVehicle.heading.rawValue : 0
    property real   _distance:              _activeVehicle ? _activeVehicle.distanceToHome.rawValue : 0
    property string _messageTitle:          ""
    property string _messageText:           ""
    property real   _toolsMargin:           ScreenTools.defaultFontPixelWidth * 0.75

    function secondsToHHMMSS(timeS) {
        var sec_num = parseInt(timeS, 10);
        var hours   = Math.floor(sec_num / 3600);
        var minutes = Math.floor((sec_num - (hours * 3600)) / 60);
        var seconds = sec_num - (hours * 3600) - (minutes * 60);
        if (hours   < 10) {hours   = "0"+hours;}
        if (minutes < 10) {minutes = "0"+minutes;}
        if (seconds < 10) {seconds = "0"+seconds;}
        return hours+':'+minutes+':'+seconds;
    }

    QGCToolInsets {
        id:                     _totalToolInsets
        leftEdgeTopInset:       parentToolInsets.leftEdgeTopInset
        leftEdgeCenterInset:    exampleRectangle.leftEdgeCenterInset
        leftEdgeBottomInset:    parentToolInsets.leftEdgeBottomInset
        rightEdgeTopInset:      parentToolInsets.rightEdgeTopInset
        rightEdgeCenterInset:   parentToolInsets.rightEdgeCenterInset
        rightEdgeBottomInset:   parent.width - compassBackground.x
        topEdgeLeftInset:       parentToolInsets.topEdgeLeftInset
        topEdgeCenterInset:     compassArrowIndicator.y + compassArrowIndicator.height
        topEdgeRightInset:      parentToolInsets.topEdgeRightInset
        bottomEdgeLeftInset:    parentToolInsets.bottomEdgeLeftInset
        bottomEdgeCenterInset:  parentToolInsets.bottomEdgeCenterInset
        bottomEdgeRightInset:   parent.height - attitudeIndicator.y
    }

    // This is an example of how you can use parent tool insets to position an element on the custom fly view layer
    // - we use parent topEdgeLeftInset to position the widget below the toolstrip
    // - we use parent bottomEdgeLeftInset to dodge the virtual joystick if enabled
    // - we use the parent leftEdgeTopInset to size our element to the same width as the ToolStripAction
    // - we export the width of this element as the leftEdgeCenterInset so that the map will recenter if the vehicle flys behind this element
    Rectangle {
        id: exampleRectangle
        visible: false // to see this example, set this to true. To view insets, enable the insets viewer FlyView.qml
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: parentToolInsets.topEdgeLeftInset + _toolsMargin
        anchors.bottomMargin: parentToolInsets.bottomEdgeLeftInset + _toolsMargin
        anchors.leftMargin: _toolsMargin
        width: parentToolInsets.leftEdgeTopInset - _toolsMargin
        color: 'red'

        property real leftEdgeCenterInset: visible ? x + width : 0
    }

    //-------------------------------------------------------------------------
    //-- Heading Indicator (숨김)
    Rectangle {
        id:                         compassBar
        visible:                    false
        height:                     ScreenTools.defaultFontPixelHeight * 1.5
        width:                      ScreenTools.defaultFontPixelWidth  * 50
        anchors.bottom:             parent.bottom
        anchors.bottomMargin:       _toolsMargin
        color:                      "#DEDEDE"
        radius:                     2
        clip:                       true
        anchors.horizontalCenter:   parent.horizontalCenter
        Repeater {
            model: 720
            QGCLabel {
                function _normalize(degrees) {
                    var a = degrees % 360
                    if (a < 0) a += 360
                    return a
                }
                property int _startAngle: modelData + 180 + _heading
                property int _angle: _normalize(_startAngle)
                anchors.verticalCenter: parent.verticalCenter
                x:              visible ? ((modelData * (compassBar.width / 360)) - (width * 0.5)) : 0
                visible:        _angle % 45 == 0
                color:          "#75505565"
                font.pointSize: ScreenTools.smallFontPointSize
                text: {
                    switch(_angle) {
                    case 0:     return "N"
                    case 45:    return "NE"
                    case 90:    return "E"
                    case 135:   return "SE"
                    case 180:   return "S"
                    case 225:   return "SW"
                    case 270:   return "W"
                    case 315:   return "NW"
                    }
                    return ""
                }
            }
        }
    }
    Rectangle {
        id:                         headingIndicator
        visible:                    false  // 버튼과 겹쳐서 숨김
        height:                     ScreenTools.defaultFontPixelHeight
        width:                      ScreenTools.defaultFontPixelWidth * 4
        color:                      qgcPal.windowShadeDark
        anchors.top:                compassBar.top
        anchors.topMargin:          -headingIndicator.height / 2
        anchors.horizontalCenter:   parent.horizontalCenter
        QGCLabel {
            text:                   _heading
            color:                  qgcPal.text
            font.pointSize:         ScreenTools.smallFontPointSize
            anchors.centerIn:       parent
        }
    }
    Image {
        id:                         compassArrowIndicator
        visible:                    false  // 버튼과 겹쳐서 숨김
        height:                     _indicatorsHeight
        width:                      height
        source:                     "/custom/img/compass_pointer.svg"
        fillMode:                   Image.PreserveAspectFit
        sourceSize.height:          height
        anchors.top:                compassBar.bottom
        anchors.topMargin:          -height / 2
        anchors.horizontalCenter:   parent.horizontalCenter
    }

    Rectangle {
        id:                     compassBackground
        anchors.bottom:         attitudeIndicator.bottom
        anchors.right:          attitudeIndicator.left
        anchors.rightMargin:    -attitudeIndicator.width / 2
        width:                  -anchors.rightMargin + compassBezel.width + (_toolsMargin * 2)
        height:                 attitudeIndicator.height * 0.75
        radius:                 2
        color:                  qgcPal.window

        Rectangle {
            id:                     compassBezel
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin:     _toolsMargin
            anchors.left:           parent.left
            width:                  height
            height:                 parent.height - (northLabelBackground.height / 2) - (headingLabelBackground.height / 2)
            radius:                 height / 2
            border.color:           qgcPal.text
            border.width:           1
            color:                  Qt.rgba(0,0,0,0)
        }

        Rectangle {
            id:                         northLabelBackground
            anchors.top:                compassBezel.top
            anchors.topMargin:          -height / 2
            anchors.horizontalCenter:   compassBezel.horizontalCenter
            width:                      northLabel.contentWidth * 1.5
            height:                     northLabel.contentHeight * 1.5
            radius:                     ScreenTools.defaultFontPixelWidth  * 0.25
            color:                      qgcPal.windowShade

            QGCLabel {
                id:                 northLabel
                anchors.centerIn:   parent
                text:               "N"
                color:              qgcPal.text
                font.pointSize:     ScreenTools.smallFontPointSize
            }
        }

        Image {
            id:                 headingNeedle
            anchors.centerIn:   compassBezel
            height:             compassBezel.height * 0.75
            width:              height
            source:             "/custom/img/compass_needle.svg"
            fillMode:           Image.PreserveAspectFit
            sourceSize.height:  height
            transform: [
                Rotation {
                    origin.x:   headingNeedle.width  / 2
                    origin.y:   headingNeedle.height / 2
                    angle:      _heading
                }]
        }

        Rectangle {
            id:                         headingLabelBackground
            anchors.top:                compassBezel.bottom
            anchors.topMargin:          -height / 2
            anchors.horizontalCenter:   compassBezel.horizontalCenter
            width:                      headingLabel.contentWidth * 1.5
            height:                     headingLabel.contentHeight * 1.5
            radius:                     ScreenTools.defaultFontPixelWidth  * 0.25
            color:                      qgcPal.windowShade

            QGCLabel {
                id:                 headingLabel
                anchors.centerIn:   parent
                text:               _heading
                color:              qgcPal.text
                font.pointSize:     ScreenTools.smallFontPointSize
            }
        }
    }

    Rectangle {
        id:                     attitudeIndicator
        anchors.bottomMargin:   _toolsMargin + parentToolInsets.bottomEdgeRightInset
        anchors.rightMargin:    _toolsMargin
        anchors.bottom:         parent.bottom
        anchors.right:          parent.right
        height:                 ScreenTools.defaultFontPixelHeight * 6
        width:                  height
        radius:                 height * 0.5
        color:                  qgcPal.windowShade

        CustomAttitudeWidget {
            size:               parent.height * 0.95
            vehicle:            _activeVehicle
            showHeading:        false
            anchors.centerIn:   parent
        }
    }

    // Fire Mission 설정값
    property int _targetSystemId: 1
    property real _targetAltitude: 2.0  // 목표 고도 (미터)
    
    // 버튼 크기 (화면 비율 기준)
    property real _buttonHeight: parent.height * 0.08  // 화면 높이의 8%
    property real _buttonWidth: parent.width * 0.08    // 화면 너비의 8%
    property real _buttonFontSize: ScreenTools.defaultFontPixelHeight * 1.5
    
    // 버튼 패널 (하단 중앙에 배치)
    Row {
        id: buttonPanel
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: ScreenTools.defaultFontPixelHeight
        spacing: ScreenTools.defaultFontPixelWidth * 2
        
        // ========== 왼쪽 그룹: 마브시스 아이디 + 미션 시작 ==========
        Row {
            id: leftButtonGroup
            spacing: ScreenTools.defaultFontPixelWidth
            anchors.verticalCenter: parent.verticalCenter
            
            // 마브시스 아이디 입력
            Rectangle {
                id: sysIdPanel
                width: sysIdRow.width + ScreenTools.defaultFontPixelWidth * 3
                height: _buttonHeight
                color: "#2196F3"  // 파란색
                radius: ScreenTools.defaultFontPixelWidth
                border.color: "#1565C0"
                border.width: 2
                
                Row {
                    id: sysIdRow
                    anchors.centerIn: parent
                    spacing: ScreenTools.defaultFontPixelWidth
                    
                    Text {
                        text: qsTr("MAV ID:")
                        color: "white"
                        font.pixelSize: _buttonFontSize
                        font.bold: true
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QGCTextField {
                        id: targetSystemIdField
                        width: ScreenTools.defaultFontPixelWidth * 6
                        height: _buttonHeight * 0.7
                        text: _targetSystemId.toString()
                        font.pixelSize: _buttonFontSize
                        font.bold: true
                        inputMethodHints: Qt.ImhDigitsOnly
                        validator: IntValidator { bottom: 1; top: 255 }
                        onEditingFinished: {
                            var val = parseInt(text)
                            if (!isNaN(val) && val >= 1 && val <= 255) {
                                _targetSystemId = val
                            } else {
                                text = _targetSystemId.toString()
                            }
                        }
                    }
                }
            }
            
            // 목표 고도 입력
            Rectangle {
                id: altitudePanel
                width: altitudeRow.width + ScreenTools.defaultFontPixelWidth * 3
                height: _buttonHeight
                color: "#9C27B0"  // 보라색
                radius: ScreenTools.defaultFontPixelWidth
                border.color: "#7B1FA2"
                border.width: 2
                
                Row {
                    id: altitudeRow
                    anchors.centerIn: parent
                    spacing: ScreenTools.defaultFontPixelWidth
                    
                    Text {
                        text: qsTr("ALT:")
                        color: "white"
                        font.pixelSize: _buttonFontSize
                        font.bold: true
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QGCTextField {
                        id: targetAltitudeField
                        width: ScreenTools.defaultFontPixelWidth * 8
                        height: _buttonHeight * 0.7
                        text: _targetAltitude.toFixed(1)
                        font.pixelSize: _buttonFontSize
                        font.bold: true
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        validator: DoubleValidator { bottom: 0; top: 10000; decimals: 1 }
                        onEditingFinished: {
                            var val = parseFloat(text)
                            if (!isNaN(val) && val >= 0 && val <= 10000) {
                                _targetAltitude = val
                            } else {
                                text = _targetAltitude.toFixed(1)
                            }
                        }
                    }
                    Text {
                        text: qsTr("m")
                        color: "white"
                        font.pixelSize: _buttonFontSize
                        font.bold: true
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
            
            // 미션 시작 버튼
            Rectangle {
                id: missionStartButton
                width: _buttonWidth * 1.2
                height: _buttonHeight
                color: missionStartMouseArea.pressed ? "#388E3C" : "#4CAF50"  // 초록색
                radius: ScreenTools.defaultFontPixelWidth
                border.color: "#2E7D32"
                border.width: 2
                
                Text {
                    anchors.centerIn: parent
                    text: qsTr("미션 시작")
                    color: "white"
                    font.pixelSize: _buttonFontSize
                    font.bold: true
                }
                
                MouseArea {
                    id: missionStartMouseArea
                    anchors.fill: parent
                    onClicked: {
                        if (!_activeVehicle) {
                            QGroundControl.showAppMessage(qsTr("No active vehicle"));
                            return;
                        }
                        
                        var savedTargetPos = QGroundControl.fireMissionTargetPosition
                        if (!savedTargetPos.isValid || savedTargetPos.latitude === 0 || savedTargetPos.longitude === 0) {
                            QGroundControl.showAppMessage(qsTr("No target position saved"));
                            return;
                        }
                        
                        // 위도, 경도를 degE7로 변환
                        var targetLat = savedTargetPos.latitude * 1e7;
                        var targetLon = savedTargetPos.longitude * 1e7;
                        var autoFire = 1;
                        var maxProjectiles = 10;
                        
                        try {
                            _activeVehicle.sendFireMissionStart(_targetSystemId, 191, targetLat, targetLon, _targetAltitude, autoFire, maxProjectiles);
                            QGroundControl.showAppMessage(qsTr("Mission Start: Lat %1, Lon %2, Alt %3m").arg(savedTargetPos.latitude.toFixed(6)).arg(savedTargetPos.longitude.toFixed(6)).arg(_targetAltitude.toFixed(1)));
                        } catch(e) {
                            console.error("Error:", e);
                            QGroundControl.showAppMessage(qsTr("Error: %1").arg(e.toString()));
                        }
                    }
                }
            }
        }
        
        // ========== 오른쪽 그룹: 자동 조준, 발사, 복귀 ==========
        Row {
            id: rightButtonGroup
            spacing: ScreenTools.defaultFontPixelWidth
            anchors.verticalCenter: parent.verticalCenter
            
            // 자동 조준 버튼
            Rectangle {
                id: autoAimButton
                width: _buttonWidth * 1.2
                height: _buttonHeight
                color: autoAimMouseArea.pressed ? "#F57C00" : "#FF9800"  // 주황색
                radius: ScreenTools.defaultFontPixelWidth
                border.color: "#E65100"
                border.width: 2
                
                Text {
                    anchors.centerIn: parent
                    text: qsTr("자동 조준")
                    color: "white"
                    font.pixelSize: _buttonFontSize
                    font.bold: true
                }
                
                MouseArea {
                    id: autoAimMouseArea
                    anchors.fill: parent
                    onClicked: {
                        if (!_activeVehicle) {
                            QGroundControl.showAppMessage(qsTr("No active vehicle"));
                            return;
                        }
                        try {
                            _activeVehicle.sendAutoAim(_targetSystemId, 191);
                            QGroundControl.showAppMessage(qsTr("Auto Aim (60001) sent to ID: %1").arg(_targetSystemId));
                        } catch(e) {
                            console.error("Error:", e);
                            QGroundControl.showAppMessage(qsTr("Error: %1").arg(e.toString()));
                        }
                    }
                }
            }
            
            // 발사 버튼
            Rectangle {
                id: fireButton
                width: _buttonWidth
                height: _buttonHeight
                color: fireMouseArea.pressed ? "#C62828" : "#F44336"  // 빨간색
                radius: ScreenTools.defaultFontPixelWidth
                border.color: "#B71C1C"
                border.width: 2
                
                Text {
                    anchors.centerIn: parent
                    text: qsTr("발사")
                    color: "white"
                    font.pixelSize: _buttonFontSize
                    font.bold: true
                }
                
                MouseArea {
                    id: fireMouseArea
                    anchors.fill: parent
                    onClicked: {
                        if (!_activeVehicle) {
                            QGroundControl.showAppMessage(qsTr("No active vehicle"));
                            return;
                        }
                        try {
                            _activeVehicle.sendFireCommand(_targetSystemId, 191);
                            QGroundControl.showAppMessage(qsTr("Fire Command (60002) sent to ID: %1").arg(_targetSystemId));
                        } catch(e) {
                            console.error("Error:", e);
                            QGroundControl.showAppMessage(qsTr("Error: %1").arg(e.toString()));
                        }
                    }
                }
            }
            
            // 복귀 버튼
            Rectangle {
                id: returnButton
                width: _buttonWidth
                height: _buttonHeight
                color: returnMouseArea.pressed ? "#7B1FA2" : "#9C27B0"  // 보라색
                radius: ScreenTools.defaultFontPixelWidth
                border.color: "#6A1B9A"
                border.width: 2
                
                Text {
                    anchors.centerIn: parent
                    text: qsTr("복귀")
                    color: "white"
                    font.pixelSize: _buttonFontSize
                    font.bold: true
                }
                
                MouseArea {
                    id: returnMouseArea
                    anchors.fill: parent
                    onClicked: {
                        if (!_activeVehicle) {
                            QGroundControl.showAppMessage(qsTr("No active vehicle"));
                            return;
                        }
                        // Custom RETURN_TO_LAUNCH (60003)
                        _activeVehicle.sendReturnToLaunch(_targetSystemId, 191);
                        QGroundControl.showAppMessage(qsTr("RTL command sent"));
                    }
                }
            }
        }
    }
}
