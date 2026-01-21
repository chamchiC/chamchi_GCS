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

import QGroundControl
import QGroundControl.Controls
import QGroundControl.Palette
import QGroundControl.ScreenTools

Window {
    id: customWindow
    
    width: 400
    height: 300
    minimumWidth: 300
    minimumHeight: 200
    
    title: qsTr("Custom Window")
    
    // 창이 닫힐 때 파괴되도록 설정
    // visible: true  // 부모에서 show() 호출로 표시
    
    color: qgcPal.window
    
    QGCPalette { id: qgcPal; colorGroupEnabled: true }
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: ScreenTools.defaultFontPixelWidth
        spacing: ScreenTools.defaultFontPixelHeight
        
        // 헤더
        QGCLabel {
            Layout.fillWidth: true
            text: qsTr("Custom Empty Window")
            font.pointSize: ScreenTools.largeFontPointSize
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
        }
        
        // 중앙 컨텐츠 영역 (비어있음 - 필요에 따라 추가)
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: qgcPal.windowShade
            radius: ScreenTools.defaultFontPixelWidth / 2
            
            QGCLabel {
                anchors.centerIn: parent
                text: qsTr("여기에 원하는 내용을 추가하세요")
                color: qgcPal.text
            }
        }
        
        // 하단 버튼
        RowLayout {
            Layout.fillWidth: true
            spacing: ScreenTools.defaultFontPixelWidth
            
            Item { Layout.fillWidth: true }
            
            QGCButton {
                text: qsTr("Close")
                onClicked: customWindow.close()
            }
        }
    }
}
