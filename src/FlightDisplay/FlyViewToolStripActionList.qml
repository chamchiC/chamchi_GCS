/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQml.Models

import QGroundControl
import QGroundControl.Controls

ToolStripActionList {
    id: _root

    signal displayPreFlightChecklist
    signal toggle3DView

    property bool isViewer3DOpen: false

    model: [
        PreFlightCheckListShowAction { onTriggered: displayPreFlightChecklist() },
        GuidedActionTakeoff { },
        GuidedActionLand { },
        GuidedActionRTL { },
        GuidedActionPause { },
        FlyViewAdditionalActionsButton { },
        GuidedToolStripAction {
            text:       _guidedController._customController.customButtonTitle
            iconSource: "/res/gear-white.svg"
            visible:    true
            enabled:    true
            actionID:   _guidedController._customController.actionCustomButton
        },
        ToolStripAction {
            text:       isViewer3DOpen ? qsTr("Fly View") : qsTr("3D View")
            iconSource: isViewer3DOpen ? "/qmlimages/PaperPlane.svg" : "/qmlimages/Viewer3D/City3DMapIcon.svg"
            visible:    QGroundControl.settingsManager.viewer3DSettings.enabled.rawValue
            onTriggered: toggle3DView()
        }
    ]
}
