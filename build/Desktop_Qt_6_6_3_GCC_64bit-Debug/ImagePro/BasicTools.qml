import QtQuick
import QtQuick.Controls
Item{
    id: _basicTools
    property int selectedIndex: -1
    ListView{
        id: basicToolsColumn
        anchors.fill: parent
        orientation: ListView.Horizontal
        spacing: 10
        clip: true
        model: [{label: "Convert", source: "assets/icons/frog.png", color: "#f08080", sourefile: "basic/Convert.qml"},
        {label: "Exposure", source: "assets/icons/ant.png", color: "#7fffd4", sourefile: "basic/Exposure.qml"},
        {label: "Gamma", source: "assets/icons/bee.png", color: "#4169e1", sourefile: "basic/Gamma.qml"},
        {label: "Smoothing", source: "assets/icons/butterfly.png", color: "#f08080", sourefile: "basic/Smoothing.qml"},
        {label: "Erode&Dilate", source: "assets/icons/cow.png", color: "#daa520", sourefile: "basic/ErodeDilate.qml"},
        {label: "Morphology", source: "assets/icons/falcon.png", color: "#9932cc", sourefile: "basic/Morphology.qml"},
        {label: "ExtractLines", source: "assets/icons/parrot.png", color: "#f08080", sourefile: "basic/ExtractLines.qml"},
        {label: "Threshold", source: "assets/icons/rhino.png", color: "#4169e1", sourefile: "basic/Threshold.qml"},
        {label: "InRange", source: "assets/icons/whale.png", color: "#daa520", sourefile: "basic/InRange.qml"},
        {label: "ColorDetect", source: "assets/icons/horse.png", color: "#9932cc", sourefile: "basic/ColorDetection.qml"},
        {label: "Contrast", source: "assets/icons/dog.png", color: "#7fffd4", sourefile: "basic/Contrast.qml"},
        {label: "Temp-Matching", source: "assets/icons/flamingo.png", color: "#daa520", sourefile: "basic/TemplateMatching.qml"}]
        delegate: Rectangle{
            height: parent.height*3/4
            anchors.verticalCenter: parent.verticalCenter
            width: parent.height
            radius: width/8
            color: index === _basicTools.selectedIndex ? Qt.darker(modelData.color, 1.4) :"transparent"
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
                    selectedIndex = index
                    mainWindow.getToolsInput(modelData.sourefile, modelData.color)
                }
            }
        }
    }
}
