/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick
import QtQuick.Layouts
import QtWebEngine

import QGroundControl
import QGroundControl.Controls
import QGroundControl.Palette
import QGroundControl.ScreenTools

Rectangle {
    color:          qgcPal.window
    anchors.fill:   parent

    QGCPalette { id: qgcPal; colorGroupEnabled: true }

    WebEngineView {
        id:             webView
        anchors.fill:   parent
        url:            "http://192.168.100.11:5000"
        
        onLoadingChanged: function(loadRequest) {
            if (loadRequest.status === WebEngineView.LoadFailedStatus) {
                console.warn("Web GUI: Failed to load " + loadRequest.url)
            }
        }
    }
}
