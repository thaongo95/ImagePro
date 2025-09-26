import QtQuick
import QtQuick.Controls
Rectangle{
    color: "#bdb76b"
    Text {
        text: qsTr("Tools")
        anchors.centerIn: parent
        font.pixelSize: 24
        color: "white"
    }
    MouseArea{
        anchors.fill: parent
        onClicked: clickedOutside()
    }
}
