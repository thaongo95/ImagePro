import QtQuick
import QtQuick.Controls
Item{
    id: _funcTools
    property int selectedIndex: -1
    ListView{
        id: funcToolsColumn
        anchors.fill: parent
        orientation: ListView.Horizontal
        spacing: 10
        clip: true
        model: [{label: "Icon Color", source: "assets/icons/axe.png", color: "#f08080", sourcefile : "functions/IconColor.qml"},
            {label: "Dataset", source: "assets/icons/axedown.png", color: "#9932cc", sourcefile : ""},
            {label: "Labelimg", source: "assets/icons/hand-saw.png", color: "#daa520", sourcefile : ""},
            {label: "Mask", source: "assets/icons/secateurs.png", color: "#4169e1", sourcefile : ""},
            {label: "Blending", source: "assets/icons/knife.png", color: "#daa520", sourcefile : ""},
            {label: "Segmentaion", source: "assets/icons/sword.png", color: "#4169e1", sourcefile : ""},
            {label: "Mosiac", source: "assets/icons/trowel.png", color: "#7fffd4", sourcefile : ""}]
        delegate: Rectangle{
            height: parent.height*3/4
            anchors.verticalCenter: parent.verticalCenter
            width: parent.height
            radius: width/8
            color: index === _funcTools.selectedIndex ? Qt.darker(modelData.color, 1.4) :"transparent"
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
                    _funcTools.selectedIndex = index
                    mainWindow.getToolsInput(modelData.sourcefile, modelData.color)
                }
            }
        }
    }
}
