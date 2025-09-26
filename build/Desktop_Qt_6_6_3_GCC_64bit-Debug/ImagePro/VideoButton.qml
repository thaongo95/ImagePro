import QtQuick
import QtQuick.Controls
Rectangle{
    color: "#8fbc8f"
    Text {
        text: qsTr("Video")
        anchors.centerIn: parent
        font.pixelSize: 24
        color: "white"
    }
    MouseArea{
        anchors.fill: parent
        onClicked: clickedOutside()
    }
}
