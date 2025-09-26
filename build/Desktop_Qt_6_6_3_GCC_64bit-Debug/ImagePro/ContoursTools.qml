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
        model: [{label: "Finding", source: "assets/icons/beehive.png", color: "#4169e1"},
        {label: "Convex-Hull", source: "assets/icons/mountain.png", color: "#daa520"},
        {label: "Box", source: "assets/icons/iceberg.png", color: "#9932cc"},
        {label: "Circle", source: "assets/icons/creek.png", color: "#9932cc"},
        {label: "Polygon", source: "assets/icons/tree.png", color: "#4169e1"}]
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
