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
    
    // 타겟 위치 (지도에서 선택한 위치)
    property real _targetLatitude: 35.9060968   // 기본값
    property real _targetLongitude: 128.7982812 // 기본값
    
    // 타겟 위치 설정 함수 (FlyViewMap.qml에서 호출)
    function setTargetPosition(latitude, longitude) {
        // 로컬 속성에 저장
        _targetLatitude = latitude;
        _targetLongitude = longitude;
        
        // FlyView.qml의 공유 속성에도 저장 (다른 컴포넌트에서 접근 가능하도록)
        var flyViewRoot = parent;
        while (flyViewRoot && !flyViewRoot.hasOwnProperty("fireMissionTargetLatitude")) {
            flyViewRoot = flyViewRoot.parent;
            if (!flyViewRoot) break;
        }
        if (flyViewRoot && flyViewRoot.hasOwnProperty("fireMissionTargetLatitude")) {
            flyViewRoot.fireMissionTargetLatitude = latitude;
            flyViewRoot.fireMissionTargetLongitude = longitude;
        }
        
        console.log("Target position set:", latitude, longitude);
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
            
            // ============================================
            // FIRE_MISSION_START 파라미터 설정
            // custom_messages.xml 구조에 맞춰 직접 값을 입력하세요
            // ============================================
            
            // target_lat, target_lon: 위도/경도 (degE7 형식 = 도 * 1E7)
            // 지도에서 "Target Position" 버튼으로 설정한 위치 사용
            var targetLat = _targetLatitude;   // 기본값 또는 setTargetPosition()으로 업데이트됨
            var targetLon = _targetLongitude;  // 기본값 또는 setTargetPosition()으로 업데이트됨
            
            // FlyView.qml의 공유 속성에서 타겟 위치 읽기 (Target Position 버튼으로 설정된 값)
            var flyViewRoot = parent;
            while (flyViewRoot && !flyViewRoot.hasOwnProperty("fireMissionTargetLatitude")) {
                flyViewRoot = flyViewRoot.parent;
                if (!flyViewRoot) break;
            }
            if (flyViewRoot && flyViewRoot.hasOwnProperty("fireMissionTargetLatitude")) {
                // FlyView.qml의 공유 속성 값 사용 (가장 최신 값)
                targetLat = flyViewRoot.fireMissionTargetLatitude;
                targetLon = flyViewRoot.fireMissionTargetLongitude;
            }
            
            console.log("Fire Mission: Using target position - Lat:", targetLat, "Lon:", targetLon);
            
            var targetLatDegE7 = Math.round(targetLat * 1e7);  // 위도 (degE7)
            var targetLonDegE7 = Math.round(targetLon * 1e7); // 경도 (degE7)
            
            // target_alt: 고도 (미터)
            var targetAlt = 1.0;  // 고도 (m) - 여기에 원하는 값 입력
            
            // auto_fire: 자동 발사 (0 = False, 1 = True)
            var autoFire = 1;  // 0 또는 1 - 여기에 원하는 값 입력
            
            // max_projectiles: 최대 발사체 수
            var maxProjectiles = 10;  // 최대 발사체 수 - 여기에 원하는 값 입력
            
            // ============================================
            // 메시지 전송
            // ============================================
            // vehicle.sendFireMissionStart(targetSystem, targetComponent, targetLatDegE7, targetLonDegE7, targetAlt, autoFire, maxProjectiles)
            // 
            // 파라미터 순서 (custom_messages.xml 구조와 동일):
            // 1. targetSystem (uint8_t) - 타겟 시스템 ID
            // 2. targetComponent (uint8_t) - 타겟 컴포넌트 ID
            // 3. targetLat (int32_t, degE7) - 위도 * 1E7
            // 4. targetLon (int32_t, degE7) - 경도 * 1E7  
            // 5. targetAlt (float, m) - 고도 미터
            // 6. autoFire (int, 0/1) - 자동 발사 활성화
            // 7. maxProjectiles (int) - 최대 발사체 수
            // ============================================
            
            try {
                _activeVehicle.sendFireMissionStart(2, 1, targetLatDegE7, targetLonDegE7, targetAlt, autoFire, maxProjectiles);
                QGroundControl.showAppMessage(qsTr("Fire Mission Start command sent"));
            } catch(e) {
                console.error("Error calling sendFireMissionStart:", e);
                QGroundControl.showAppMessage(qsTr("Error: %1").arg(e.toString()));
            }
        }
        
        ToolTip.visible: hovered
        ToolTip.text: qsTr("Send FIRE_MISSION_START message - Edit parameters in code")
    }
}
