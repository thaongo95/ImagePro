import QtQuick
import QtQuick.Controls
Rectangle{
    color: "#deb887"
    Text {
        text: qsTr("Function")
        anchors.centerIn: parent
        font.pixelSize: 24
        color: "white"
    }
    MouseArea{
        anchors.fill: parent
        onClicked: clickedOutside()
    }
}
