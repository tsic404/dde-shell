import QtQuick 2.15
import QtQuick.Controls 2.15

import org.deepin.ds 1.0
import org.deepin.dtk 1.0 as D

AppletItem {
    id: showdesktop
    implicitWidth: 40
    implicitHeight: 40

    Item {
        anchors.fill: parent
        D.DciIcon {
            anchors.fill: parent
            sourceSize {
                width: showdesktop.width
                height: showdesktop.height
            }
            name: Applet.iconName
        }

        MouseArea {
            anchors.fill: parent
            onClicked: Applet.toggleShowDesktop()
        }
    }
    
}
