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
import QGroundControl.FlightDisplay

ToolStrip {
    id: _root

    signal displayPreFlightChecklist
    signal toggle3DView
    property bool isViewer3DOpen: false

    FlyViewToolStripActionList {
        id: flyViewToolStripActionList

        isViewer3DOpen: _root.isViewer3DOpen
        onDisplayPreFlightChecklist: _root.displayPreFlightChecklist()
        onToggle3DView: _root.toggle3DView()
    }

    model: flyViewToolStripActionList.model
}
