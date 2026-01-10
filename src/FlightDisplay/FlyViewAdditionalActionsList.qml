/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQml

import QGroundControl

QtObject {
    property var guidedController

    property bool anyActionAvailable: guidedController.showStartMission || guidedController.showContinueMission || guidedController.showChangeAlt || 
                                      guidedController.showChangeLoiterRadius ||  guidedController.showLandAbort || guidedController.showChangeSpeed ||
                                      guidedController.showGripper
    property var model: [
        {
            title:      guidedController.startMissionTitle,
            text:       guidedController.startMissionMessage,
            action:     guidedController.actionStartMission,
            visible:    guidedController.showStartMission
        },
        {
            title:      guidedController.continueMissionTitle,
            text:       guidedController.continueMissionMessage,
            action:     guidedController.actionContinueMission,
            visible:    guidedController.showContinueMission
        },
        {
            title:      guidedController.changeAltTitle,
            text:       guidedController.changeAltMessage,
            action:     guidedController.actionChangeAlt,
            visible:    guidedController.showChangeAlt
        },
        {
            title:      guidedController.changeLoiterRadiusTitle,
            text:       guidedController.changeLoiterRadiusMessage,
            action:     guidedController.actionChangeLoiterRadius,
            visible:    guidedController.showChangeLoiterRadius
        },
        {
            title:      guidedController.landAbortTitle,
            text:       guidedController.landAbortMessage,
            action:     guidedController.actionLandAbort,
            visible:    guidedController.showLandAbort
        },
        {
            title:      guidedController.changeSpeedTitle,
            text:       guidedController.changeSpeedMessage,
            action:     guidedController.actionChangeSpeed,
            visible:    guidedController.showChangeSpeed
        },
        {
            title:      guidedController.gripperTitle,
            text:       guidedController.gripperMessage,
            action:     guidedController.actionGripper,
            visible:    guidedController.showGripper
        },
        {
            title:      qsTr("Fire Mission Start"),
            text:       qsTr("Send FIRE_MISSION_START message to drone at current position"),
            action:     function() {
                console.log("Fire Mission Start action triggered from FlyViewAdditionalActionsList");
                var vehicle = QGroundControl.multiVehicleManager.activeVehicle;
                if (vehicle && vehicle.coordinate.isValid) {
                    console.log("Vehicle found, current position:", vehicle.coordinate.latitude, vehicle.coordinate.longitude);
                    try {
                        vehicle.sendFireMissionStartAtCurrentPosition(1, 10);
                        QGroundControl.showAppMessage(qsTr("Fire Mission Start command sent"));
                        console.log("sendFireMissionStartAtCurrentPosition called successfully");
                    } catch(e) {
                        console.error("Error calling sendFireMissionStartAtCurrentPosition:", e);
                        QGroundControl.showAppMessage(qsTr("Error sending Fire Mission Start command: %1").arg(e.toString()));
                    }
                } else {
                    console.warn("Fire Mission: No valid vehicle or coordinate");
                }
            },
            visible:    QGroundControl.multiVehicleManager.activeVehicle !== null
        }
    ]
}
