import QtQuick
import QtQuick.Controls
Rectangle{
    color: "#ff7f50"
    signal clickedOutside()
    Text {
        text: qsTr("File")
        anchors.centerIn: parent
        font.pixelSize: 24
        color: "white"
    }
    MouseArea{
        anchors.fill: parent
        onClicked: clickedOutside()
    }
}
