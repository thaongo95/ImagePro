import QtQuick
import QtQuick.Controls
Item{
    id: hisTools
    property int selectedIndex: -1
    ListView{
        id: hisToolsColumn
        anchors.fill: parent
        orientation: ListView.Horizontal
        spacing: 10
        clip: true
        model: [{label: "Equalization", source: "assets/icons/grape.png", color: "#7fffd4", sourcefile: "histograms/Equalization.qml"},
        {label: "Calculation", source: "assets/icons/papaya.png", color: "#daa520", sourcefile: "histograms/Calculation.qml"},
        {label: "Comparition", source: "assets/icons/tomato.png", color: "#4169e1", sourcefile: "histograms/Comparition.qml"},
        {label: "Back Projection", source: "assets/icons/avocado.png", color: "#f08080", sourcefile: "histograms/BackProjection.qml"},
        {label: "Temp Matching", source: "assets/icons/watermelon.png", color: "#4169e1", sourcefile: "histograms/TempMatching.qml"}]
        delegate: Rectangle{
            height: parent.height*3/4
            anchors.verticalCenter: parent.verticalCenter
            width: parent.height
            radius: width/8
            color: index === hisTools.selectedIndex ? Qt.darker(modelData.color, 1.4) :"transparent"
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
                onClicked:{
                    hisTools.selectedIndex = index
                    mainWindow.getToolsInput(modelData.sourcefile, modelData.color)
                }
            }
        }
    }
}
