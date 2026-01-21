/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

import QtLocation
import QtPositioning
import QtQuick.Window
import QtQml.Models

import QGroundControl
import QGroundControl.Controllers
import QGroundControl.Controls
import QGroundControl.FactSystem
import QGroundControl.FlightDisplay
import QGroundControl.FlightMap
import QGroundControl.Palette
import QGroundControl.ScreenTools
import QGroundControl.Vehicle

// To implement a custom overlay copy this code to your own control in your custom code source. Then override the
// FlyViewCustomLayer.qml resource with your own qml. See the custom example and documentation for details.
Item {
    id: _root

    property var parentToolInsets               // These insets tell you what screen real estate is available for positioning the controls in your overlay
    property var totalToolInsets:   _toolInsets // These are the insets for your custom overlay additions
    property var mapControl

    // since this file is a placeholder for the custom layer in a standard build, we will just pass through the parent insets
    QGCToolInsets {
        id:                     _toolInsets
        leftEdgeTopInset:       parentToolInsets.leftEdgeTopInset
        leftEdgeCenterInset:    parentToolInsets.leftEdgeCenterInset
        leftEdgeBottomInset:    parentToolInsets.leftEdgeBottomInset
        rightEdgeTopInset:      parentToolInsets.rightEdgeTopInset
        rightEdgeCenterInset:   parentToolInsets.rightEdgeCenterInset
        rightEdgeBottomInset:   parentToolInsets.rightEdgeBottomInset
        topEdgeLeftInset:       parentToolInsets.topEdgeLeftInset
        topEdgeCenterInset:     parentToolInsets.topEdgeCenterInset
        topEdgeRightInset:      parentToolInsets.topEdgeRightInset
        bottomEdgeLeftInset:    parentToolInsets.bottomEdgeLeftInset
        bottomEdgeCenterInset:  parentToolInsets.bottomEdgeCenterInset
        bottomEdgeRightInset:   parentToolInsets.bottomEdgeRightInset
    }
    
    // FIRE_MISSION_START 버튼 (우측 하단에 배치)
    property var _activeVehicle: QGroundControl.multiVehicleManager.activeVehicle
    
    // 빈 창 컴포넌트
    Component {
        id: customWindowComponent
        CustomEmptyWindow { }
    }
    
    property var _customWindowInstance: null
    
    // 빈 창 열기 버튼
    QGCButton {
        id: openWindowButton
        anchors.right: parent.right
        anchors.bottom: fireMissionButton.top
        anchors.margins: ScreenTools.defaultFontPixelWidth
        text: qsTr("Open Window")
        
        onClicked: {
            if (_customWindowInstance === null) {
                _customWindowInstance = customWindowComponent.createObject(_root)
                if (_customWindowInstance) {
                    _customWindowInstance.closing.connect(function() {
                        _customWindowInstance = null
                    })
                    _customWindowInstance.show()
                }
            } else {
                _customWindowInstance.raise()
                _customWindowInstance.requestActivate()
            }
        }
    }
    
    QGCButton {
        id: fireMissionButton
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: ScreenTools.defaultFontPixelWidth
        text: qsTr("Fire Mission")
        visible: _activeVehicle !== null
        
        onClicked: {
            if (!_activeVehicle) {
                QGroundControl.showAppMessage(qsTr("No active vehicle"));
                return;
            }
            
            // 저장된 타겟 위치 확인
            var savedTargetPos = QGroundControl.fireMissionTargetPosition
            if (!savedTargetPos.isValid || savedTargetPos.latitude === 0 || savedTargetPos.longitude === 0) {
                QGroundControl.showAppMessage(qsTr("No target position saved. Please select a target position on the map first."));
                return;
            }
            
            // ============================================
            // FIRE_MISSION_START 파라미터 설정
            // 저장된 타겟 위치를 사용
            // ============================================
            
            // target_lat, target_lon: 위도/경도 (degE7 형식 = 도 * 1E7)
            // 저장된 위치에서 가져옴
            var targetLatDegE7 = Math.round(savedTargetPos.latitude * 1e7);  // 위도 (degE7)
            var targetLonDegE7 = Math.round(savedTargetPos.longitude * 1e7); // 경도 (degE7)
            
            // target_alt: 고도 (미터) - 저장된 위치의 고도 사용, 없으면 0
            var targetAlt = savedTargetPos.altitude && !isNaN(savedTargetPos.altitude) ? savedTargetPos.altitude : 0.0;
            
            // auto_fire: 자동 발사 (0 = False, 1 = True)
            var autoFire = 1;  // 0 또는 1 - 여기에 원하는 값 입력
            
            // max_projectiles: 최대 발사체 수
            var maxProjectiles = 10;  // 최대 발사체 수 - 여기에 원하는 값 입력

            // ============================================
            // 파라미터 순서 (custom_messages.xml 구조와 동일):
            // 메시지 전송
            // ============================================
            // 1. targetSystem (uint8_t) - 타겟 시스템 ID
            // 2. targetComponent (uint8_t) - 타겟 컴포넌트 ID
            // 3. targetLat (int32_t, degE7) - 위도 * 1E7
            // 4. targetLon (int32_t, degE7) - 경도 * 1E7
            // 5. targetAlt (float, m) - 고도 미터
            // 6. autoFire (int, 0/1) - 자동 발사 활성화
            // 7. maxProjectiles (int) - 최대 발사체 수
            // ============================================
            
            try {
                _activeVehicle.sendFireMissionStart(1, 1, 0, 0, 2, autoFire, maxProjectiles);
                QGroundControl.showAppMessage(qsTr("Fire Mission Start command sent to: %1, %2").arg(savedTargetPos.latitude.toFixed(6)).arg(savedTargetPos.longitude.toFixed(6)));
            } catch(e) {
                console.error("Error calling sendFireMissionStart:", e);
                QGroundControl.showAppMessage(qsTr("Error: %1").arg(e.toString()));
            }
        }
        
        ToolTip.visible: hovered
        ToolTip.text: {
            var savedTargetPos = QGroundControl.fireMissionTargetPosition
            if (savedTargetPos.isValid && savedTargetPos.latitude !== 0 && savedTargetPos.longitude !== 0) {
                return qsTr("Send FIRE_MISSION_START to saved target: %1, %2").arg(savedTargetPos.latitude.toFixed(6)).arg(savedTargetPos.longitude.toFixed(6))
            } else {
                return qsTr("Send FIRE_MISSION_START - No target position saved. Select target on map first.")
            }
        }
    }
}
