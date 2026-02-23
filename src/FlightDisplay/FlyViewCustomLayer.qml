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
import QtCore

import QGroundControl
import QGroundControl.Controls
import QGroundControl.Palette
import QGroundControl.ScreenTools
import QGroundControl.FlightDisplay

Item {
    id: rootItem
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

    // Custom: 멀티기체 연결 시 나침반/자세계 숨김 (멀티기체 패널에 각 기체별 정보 표시됨)
    property bool _multipleVehicles: QGroundControl.multiVehicleManager.vehicles.count > 1

    Rectangle {
        id:                     compassBackground
        visible:                !_multipleVehicles
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
        visible:                !_multipleVehicles
        anchors.bottomMargin:   ScreenTools.defaultFontPixelHeight * 5
        anchors.rightMargin:    _toolsMargin
        anchors.bottom:         buttonPanel.top
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

    // Fire Mission 설정값 (자동 저장/불러오기)
    Settings {
        id: fireMissionSettings
        category: "FireMission"
        property int targetSystemId: 1
        property real targetAltitude: 2.0
        property real takeoffSpeed: 5.0
        property real flightSpeed: 10.0
    }
    
    // Settings 값 바인딩
    property int _targetSystemId: fireMissionSettings.targetSystemId
    property real _targetAltitude: fireMissionSettings.targetAltitude
    property real _takeoffSpeed: fireMissionSettings.takeoffSpeed
    property real _flightSpeed: fireMissionSettings.flightSpeed
    
    // 버튼 크기
    property real _panelHeight: ScreenTools.defaultFontPixelHeight * 2.8
    property real _fontSize: ScreenTools.largeFontPointSize
    property real _fieldWidth: ScreenTools.defaultFontPixelWidth * 9
    property real _panelRadius: ScreenTools.defaultFontPixelWidth * 0.5
    property real _buttonPadding: ScreenTools.defaultFontPixelWidth * 3

    // Custom: 전달 모드 (0: 단일 기체, 1: 전체 기체, 2: 선택된 기체)
    property int _sendMode: 0

    // 커스텀 메시지를 전달 모드에 따라 전송하는 헬퍼 함수
    // 반환값: 전송 결과 문자열 (Toast 표시용)
    function sendToVehicles(commandFunc) {
        if (_sendMode === 0) {
            // 단일 기체: ID 입력칸의 targetSystemId와 일치하는 기체를 찾아 전송
            var vehicles = QGroundControl.multiVehicleManager.vehicles
            var found = false
            for (var i = 0; i < vehicles.count; i++) {
                var v = vehicles.get(i)
                if (v.id === _targetSystemId) {
                    commandFunc(v)
                    found = true
                    break
                }
            }
            if (!found) {
                return qsTr("ID %1 기체를 찾을 수 없습니다").arg(_targetSystemId)
            }
            return qsTr("ID %1").arg(_targetSystemId)
        } else if (_sendMode === 1) {
            // 전체 기체
            var allVehicles = QGroundControl.multiVehicleManager.vehicles
            for (var j = 0; j < allVehicles.count; j++) {
                commandFunc(allVehicles.get(j))
            }
            return qsTr("전체 %1대").arg(allVehicles.count)
        }
        return ""
    }

    // 버튼 패널 (하단 중앙에 배치)
    Row {
        id: buttonPanel
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: ScreenTools.defaultFontPixelHeight * 0.5
        spacing: ScreenTools.defaultFontPixelWidth
        
        // ========== 입력 패널들 ==========
        Row {
            id: inputPanels
            spacing: ScreenTools.defaultFontPixelWidth
            anchors.verticalCenter: parent.verticalCenter
            
            // MAV ID 입력
            Rectangle {
                id: sysIdPanel
                width: sysIdRow.width + ScreenTools.defaultFontPixelWidth * 3
                height: _panelHeight
                color: qgcPal.windowShade
                radius: _panelRadius
                border.color: qgcPal.text
                border.width: 1
                
                Row {
                    id: sysIdRow
                    anchors.centerIn: parent
                    spacing: ScreenTools.defaultFontPixelWidth
                    
                    QGCLabel {
                        text: "ID"
                        font.pointSize: _fontSize
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QGCTextField {
                        id: targetSystemIdField
                        width: _fieldWidth
                        height: _panelHeight * 0.7
                        text: _targetSystemId.toString()
                        font.pointSize: _fontSize
                        inputMethodHints: Qt.ImhDigitsOnly
                        validator: IntValidator { bottom: 1; top: 255 }
                        onActiveFocusChanged: if (activeFocus) selectAll()
                        onEditingFinished: {
                            focus = false
                            var val = parseInt(text)
                            if (!isNaN(val) && val >= 1 && val <= 255) {
                                fireMissionSettings.targetSystemId = val
                            } else {
                                text = _targetSystemId.toString()
                            }
                        }
                    }
                    // 위/아래 화살표 버튼
                    Column {
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 1
                        
                        // 위 화살표 (증가)
                        Rectangle {
                            width: ScreenTools.defaultFontPixelWidth * 2.5
                            height: _panelHeight * 0.32
                            color: upMouseArea.pressed ? Qt.darker(qgcPal.button, 1.3) : qgcPal.button
                            radius: 2
                            
                            Text {
                                anchors.centerIn: parent
                                text: "▲"
                                font.pixelSize: parent.height * 0.7
                                color: qgcPal.text
                            }
                            
                            MouseArea {
                                id: upMouseArea
                                anchors.fill: parent
                                onClicked: {
                                    if (_targetSystemId < 255) {
                                        fireMissionSettings.targetSystemId = _targetSystemId + 1
                                    }
                                }
                            }
                        }
                        
                        // 아래 화살표 (감소)
                        Rectangle {
                            width: ScreenTools.defaultFontPixelWidth * 2.5
                            height: _panelHeight * 0.32
                            color: downMouseArea.pressed ? Qt.darker(qgcPal.button, 1.3) : qgcPal.button
                            radius: 2
                            
                            Text {
                                anchors.centerIn: parent
                                text: "▼"
                                font.pixelSize: parent.height * 0.7
                                color: qgcPal.text
                            }
                            
                            MouseArea {
                                id: downMouseArea
                                anchors.fill: parent
                                onClicked: {
                                    if (_targetSystemId > 1) {
                                        fireMissionSettings.targetSystemId = _targetSystemId - 1
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
            // 목표 고도 입력
            Rectangle {
                id: altitudePanel
                width: altitudeRow.width + ScreenTools.defaultFontPixelWidth * 3
                height: _panelHeight
                color: qgcPal.windowShade
                radius: _panelRadius
                border.color: qgcPal.text
                border.width: 1
                
                Row {
                    id: altitudeRow
                    anchors.centerIn: parent
                    spacing: ScreenTools.defaultFontPixelWidth
                    
                    QGCLabel {
                        text: "ALT"
                        font.pointSize: _fontSize
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QGCTextField {
                        id: targetAltitudeField
                        width: _fieldWidth
                        height: _panelHeight * 0.7
                        text: _targetAltitude.toFixed(1)
                        font.pointSize: _fontSize
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        validator: DoubleValidator { bottom: 0; top: 10000; decimals: 1 }
                        onActiveFocusChanged: if (activeFocus) selectAll()
                        onEditingFinished: {
                            focus = false
                            var val = parseFloat(text)
                            if (!isNaN(val) && val >= 0 && val <= 10000) {
                                fireMissionSettings.targetAltitude = val
                            } else {
                                text = _targetAltitude.toFixed(1)
                            }
                        }
                    }
                    QGCLabel {
                        text: "m"
                        font.pointSize: _fontSize
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
            
            // 이륙 속도 입력
            Rectangle {
                id: takeoffSpeedPanel
                width: takeoffSpeedRow.width + ScreenTools.defaultFontPixelWidth * 3
                height: _panelHeight
                color: qgcPal.windowShade
                radius: _panelRadius
                border.color: qgcPal.text
                border.width: 1
                
                Row {
                    id: takeoffSpeedRow
                    anchors.centerIn: parent
                    spacing: ScreenTools.defaultFontPixelWidth
                    
                    QGCLabel {
                        text: "이륙속도"
                        font.pointSize: _fontSize
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QGCTextField {
                        id: takeoffSpeedField
                        width: _fieldWidth
                        height: _panelHeight * 0.7
                        text: _takeoffSpeed.toFixed(1)
                        font.pointSize: _fontSize
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        validator: DoubleValidator { bottom: 0; top: 100; decimals: 1 }
                        onActiveFocusChanged: if (activeFocus) selectAll()
                        onEditingFinished: {
                            focus = false
                            var val = parseFloat(text)
                            if (!isNaN(val) && val >= 0 && val <= 100) {
                                fireMissionSettings.takeoffSpeed = val
                            } else {
                                text = _takeoffSpeed.toFixed(1)
                            }
                        }
                    }
                    QGCLabel {
                        text: "m/s"
                        font.pointSize: _fontSize
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
            
            // 비행 속도 입력
            Rectangle {
                id: flightSpeedPanel
                width: flightSpeedRow.width + ScreenTools.defaultFontPixelWidth * 3
                height: _panelHeight
                color: qgcPal.windowShade
                radius: _panelRadius
                border.color: qgcPal.text
                border.width: 1
                
                Row {
                    id: flightSpeedRow
                    anchors.centerIn: parent
                    spacing: ScreenTools.defaultFontPixelWidth
                    
                    QGCLabel {
                        text: "비행속도"
                        font.pointSize: _fontSize
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QGCTextField {
                        id: flightSpeedField
                        width: _fieldWidth
                        height: _panelHeight * 0.7
                        text: _flightSpeed.toFixed(1)
                        font.pointSize: _fontSize
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        validator: DoubleValidator { bottom: 0; top: 100; decimals: 1 }
                        onActiveFocusChanged: if (activeFocus) selectAll()
                        onEditingFinished: {
                            focus = false
                            var val = parseFloat(text)
                            if (!isNaN(val) && val >= 0 && val <= 100) {
                                fireMissionSettings.flightSpeed = val
                            } else {
                                text = _flightSpeed.toFixed(1)
                            }
                        }
                    }
                    QGCLabel {
                        text: "m/s"
                        font.pointSize: _fontSize
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
        }
        
        // ========== 전달 모드 선택 ==========
        Rectangle {
            id: sendModePanel
            width: sendModeRow.width + ScreenTools.defaultFontPixelWidth * 2
            height: _panelHeight
            color: qgcPal.windowShade
            radius: _panelRadius
            border.color: qgcPal.text
            border.width: 1
            anchors.verticalCenter: parent.verticalCenter

            Row {
                id: sendModeRow
                anchors.centerIn: parent
                spacing: 2

                Repeater {
                    model: [qsTr("단일"), qsTr("전체")]

                    Rectangle {
                        width: sendModeLabel.contentWidth + ScreenTools.defaultFontPixelWidth * 2
                        height: _panelHeight * 0.7
                        radius: 2
                        color: _sendMode === index ? qgcPal.buttonHighlight : "transparent"

                        QGCLabel {
                            id: sendModeLabel
                            anchors.centerIn: parent
                            text: modelData
                            font.pointSize: ScreenTools.defaultFontPointSize
                            color: _sendMode === index ? qgcPal.buttonHighlightText : qgcPal.text
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: _sendMode = index
                        }
                    }
                }
            }
        }

        // ========== 버튼들 ==========
        Row {
            id: actionButtons
            spacing: ScreenTools.defaultFontPixelWidth
            anchors.verticalCenter: parent.verticalCenter

            // 미션 시작 버튼
            Rectangle {
                id: missionStartButton
                width: missionStartLabel.width + _buttonPadding * 2
                height: _panelHeight
                color: missionStartMouseArea.pressed ? Qt.darker(qgcPal.colorGreen, 1.5) : Qt.darker(qgcPal.colorGreen, 1.2)
                radius: _panelRadius
                
                QGCLabel {
                    id: missionStartLabel
                    anchors.centerIn: parent
                    text: "시작"
                    font.pointSize: _fontSize
                    font.bold: true
                    color: "white"
                }
                
                MouseArea {
                    id: missionStartMouseArea
                    anchors.fill: parent
                    onClicked: {
                        var savedTargetPos = QGroundControl.fireMissionTargetPosition
                        if (!savedTargetPos.isValid || savedTargetPos.latitude === 0 || savedTargetPos.longitude === 0) {
                            QGroundControl.showAppMessage(qsTr("타겟 위치가 설정되지 않았습니다"))
                            return;
                        }

                        var targetLat = savedTargetPos.latitude * 1e7;
                        var targetLon = savedTargetPos.longitude * 1e7;

                        var result = sendToVehicles(function(vehicle) {
                            try {
                                vehicle.sendFireMissionStart(_targetSystemId, 191, targetLat, targetLon, _targetAltitude, 1, 10, _takeoffSpeed, _flightSpeed);
                            } catch(e) {
                                console.error("Mission Start Error (vehicle " + vehicle.id + "):", e);
                            }
                        })
                        QGroundControl.showAppMessage(qsTr("미션 시작 명령 전송 → %1").arg(result))
                    }
                }
            }
            
            // 자동 조준 버튼
            Rectangle {
                id: autoAimButton
                width: autoAimLabel.width + _buttonPadding * 2
                height: _panelHeight
                color: autoAimMouseArea.pressed ? Qt.darker(qgcPal.colorOrange, 1.5) : Qt.darker(qgcPal.colorOrange, 1.2)
                radius: _panelRadius
                
                QGCLabel {
                    id: autoAimLabel
                    anchors.centerIn: parent
                    text: "조준"
                    font.pointSize: _fontSize
                    font.bold: true
                    color: "white"
                }
                
                MouseArea {
                    id: autoAimMouseArea
                    anchors.fill: parent
                    onClicked: {
                        var result = sendToVehicles(function(vehicle) {
                            try {
                                vehicle.sendAutoAim(_targetSystemId, 191);
                            } catch(e) {
                                console.error("Auto Aim Error (vehicle " + vehicle.id + "):", e);
                            }
                        })
                        QGroundControl.showAppMessage(qsTr("자동 조준 명령 전송 → %1").arg(result))
                    }
                }
            }
            
            // 발사 버튼
            Rectangle {
                id: fireButton
                width: fireLabel.width + _buttonPadding * 2
                height: _panelHeight
                color: fireMouseArea.pressed ? Qt.darker(qgcPal.colorRed, 1.5) : Qt.darker(qgcPal.colorRed, 1.2)
                radius: _panelRadius
                
                QGCLabel {
                    id: fireLabel
                    anchors.centerIn: parent
                    text: "발사"
                    font.pointSize: _fontSize
                    font.bold: true
                    color: "white"
                }
                
                MouseArea {
                    id: fireMouseArea
                    anchors.fill: parent
                    onClicked: {
                        var result = sendToVehicles(function(vehicle) {
                            try {
                                vehicle.sendFireCommand(_targetSystemId, 191);
                            } catch(e) {
                                console.error("Fire Command Error (vehicle " + vehicle.id + "):", e);
                            }
                        })
                        QGroundControl.showAppMessage(qsTr("발사 명령 전송 → %1").arg(result))
                    }
                }
            }
            
            // 복귀 버튼
            Rectangle {
                id: returnButton
                width: returnLabel.width + _buttonPadding * 2
                height: _panelHeight
                color: returnMouseArea.pressed ? Qt.darker(qgcPal.buttonHighlight, 1.5) : Qt.darker(qgcPal.buttonHighlight, 1.2)
                radius: _panelRadius
                
                QGCLabel {
                    id: returnLabel
                    anchors.centerIn: parent
                    text: "복귀"
                    font.pointSize: _fontSize
                    font.bold: true
                    color: "white"
                }
                
                MouseArea {
                    id: returnMouseArea
                    anchors.fill: parent
                    onClicked: {
                        var result = sendToVehicles(function(vehicle) {
                            try {
                                vehicle.sendReturnToLaunch(_targetSystemId, 191);
                            } catch(e) {
                                console.error("RTL Error (vehicle " + vehicle.id + "):", e);
                            }
                        })
                        QGroundControl.showAppMessage(qsTr("복귀 명령 전송 → %1").arg(result))
                    }
                }
            }
        }
    }
}
