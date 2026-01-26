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

    // 헤더 영역 새로고침 버튼 (오른쪽 상단, 헤더 스타일)
    Rectangle {
        id:                 refreshButtonContainer
        anchors.top:        parent.top
        anchors.right:      parent.right
        anchors.topMargin:  -ScreenTools.defaultFontPixelHeight * 2.5
        anchors.rightMargin: ScreenTools.defaultFontPixelWidth
        width:              refreshButton.width + ScreenTools.defaultFontPixelWidth * 2
        height:             ScreenTools.defaultFontPixelHeight * 2
        color:              qgcPal.buttonHighlight
        radius:             ScreenTools.defaultFontPixelHeight / 4

        QGCLabel {
            id:                 refreshButton
            anchors.centerIn:   parent
            text:               "새로고침"
            color:              qgcPal.buttonHighlightText
        }

        MouseArea {
            anchors.fill:   parent
            onClicked:      webView.reload()
            cursorShape:    Qt.PointingHandCursor
        }
    }
}
