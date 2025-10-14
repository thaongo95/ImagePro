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
        model: [{label: "Harris", source: "assets/icons/grape.png", color: "#7fffd4", sourcefile: "features2D/HarrisCorner.qml"},
        {label: "Shi.Tomasi", source: "assets/icons/papaya.png", color: "#daa520", sourcefile: "features2D/ShiTomasi.qml"},
        {label: "Custom", source: "assets/icons/tomato.png", color: "#4169e1", sourcefile: "features2D/Custom.qml"},
        {label: "Subpix", source: "assets/icons/avocado.png", color: "#f08080", sourcefile: "features2D/Subpix.qml"},
        {label: "SIFT", source: "assets/icons/watermelon.png", color: "#4169e1", sourcefile: "features2D/SIFT.qml"},
        {label: "ORB", source: "assets/icons/pineapple.png", color: "#f08080", sourcefile: "features2D/ORB.qml"},
        {label: "AKAZE", source: "assets/icons/strawberry.png", color: "#7fffd4", sourcefile: "features2D/AKAZE.qml"},
        {label: "Matching", source: "assets/icons/olive.png", color: "#daa520", sourcefile: "features2D/FeaturesMatching.qml"},
        {label: "Detecting", source: "assets/icons/fruits.png", color: "#daa520", sourcefile: "features2D/FeaturesDetecting.qml"},
        {label: "Stitching", source: "assets/icons/almond.png", color: "#daa520", sourcefile: "features2D/FeaturesStitching.qml"}]
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
