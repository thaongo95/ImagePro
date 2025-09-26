import QtQuick
import QtQuick.Controls
Rectangle{
    color: "#6495ed"
    Text {
        text: qsTr("Image")
        anchors.centerIn: parent
        font.pixelSize: 24
        color: "white"
    }
    MouseArea{
        anchors.fill: parent
        onClicked: clickedOutside()
    }
}
