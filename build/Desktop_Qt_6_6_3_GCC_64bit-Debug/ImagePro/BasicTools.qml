import QtQuick
import QtQuick.Controls
Item{
    id: _basicTools

    ListView{
        id: basicToolsColumn
        anchors.fill: parent
        orientation: ListView.Horizontal
        spacing: 10
        clip: true
        model: [{label: "Draw", source: "assets/icons/ant.png", color: "#7fffd4"},
        {label: "Generator", source: "assets/icons/bee.png", color: "#4169e1"},
        {label: "Smoothing", source: "assets/icons/butterfly.png", color: "#f08080"},
        {label: "Erode&Dilate", source: "assets/icons/cow.png", color: "#daa520"},
        {label: "Morphology", source: "assets/icons/falcon.png", color: "#9932cc"},
        {label: "Hit-or-miss", source: "assets/icons/frog.png", color: "#f08080"},
        {label: "E-Lines", source: "assets/icons/horse.png", color: "#9932cc"},
        {label: "Pyramids", source: "assets/icons/parrot.png", color: "#f08080"},
        {label: "Threshold", source: "assets/icons/rhino.png", color: "#4169e1"},
        {label: "inRange", source: "assets/icons/whale.png", color: "#daa520"}]
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
