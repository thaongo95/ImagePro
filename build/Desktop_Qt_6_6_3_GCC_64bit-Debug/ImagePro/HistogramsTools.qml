import QtQuick
import QtQuick.Controls
Item{
    id: _hisTools
    ListView{
        id: histogramToolsColumn
        anchors.fill: parent
        orientation: ListView.Horizontal
        spacing: 10
        clip: true
        model: [{label: "Equalization", source: "assets/icons/grape.png", color: "#7fffd4"},
        {label: "Calculation", source: "assets/icons/papaya.png", color: "#daa520"},
        {label: "Comparition", source: "assets/icons/tomato.png", color: "#4169e1"},
        {label: "Temp-match", source: "assets/icons/watermelon.png", color: "#4169e1"},]
        delegate: Rectangle{
            height: parent.height*2/3
            anchors.verticalCenter: parent.verticalCenter
            width: parent.height
            radius: width/8
            color: "transparent"
            Image{
                id: _icon
                anchors.top: parent.top
                anchors.topMargin: parent.height/10
                anchors.horizontalCenter: parent.horizontalCenter
                height: parent.height*2/3
                width: height
                source: modelData.source
            }
            Text{
                anchors.horizontalCenter: _icon.horizontalCenter
                anchors.top: _icon.bottom
                anchors.topMargin: 2
                text: modelData.label
                font.pixelSize: 14
                color: modelData.color
            }
        }
    }
}
