import QtQuick
import QtQuick.Controls
Item{
    id: contoursTools
    property int selectedIndex: -1
    ListView{
        id: contoursToolsColumn
        anchors.fill: parent
        orientation: ListView.Horizontal
        spacing: 10
        clip: true
        model: [{label: "Draw Egde", source: "assets/icons/beehive.png", color: "#4169e1", sourcefile : "contours/DrawEgde.qml"},
        {label: "Draw Shape", source: "assets/icons/mountain.png", color: "#daa520", sourcefile : "contours/DrawShape.qml"},
        {label: "Convex Hull", source: "assets/icons/iceberg.png", color: "#9932cc", sourcefile : "contours/ConvexHull.qml"},
        {label: "Moments", source: "assets/icons/creek.png", color: "#9932cc", sourcefile : "contours/Moments.qml"},
        {label: "Point Polygon", source: "assets/icons/tree.png", color: "#4169e1", sourcefile : "contours/PointPolygon.qml"}]
        delegate: Rectangle{
            height: parent.height*3/4
            anchors.verticalCenter: parent.verticalCenter
            width: parent.height
            radius: width/8
            color: index === contoursTools.selectedIndex ? Qt.darker(modelData.color, 1.4) :"transparent"
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
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    contoursTools.selectedIndex = index
                    mainWindow.getToolsInput(modelData.sourcefile, modelData.color)
                }
            }
        }
    }
}
