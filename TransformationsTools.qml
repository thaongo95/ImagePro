import QtQuick
import QtQuick.Controls
Item{
    id: _transformationsTools
    ListView{
        id: transformationsToolsColumn
        anchors.fill: parent
        orientation: ListView.Horizontal
        spacing: 10
        clip: true
        model: [{label: "Linear-filter", source: "assets/icons/ankh.png", color:"#daa520", sourefile: ""},
        {label: "Add-border", source: "assets/icons/chinese-symbol.png", color:"#7fffd4", sourefile: ""},
        {label: "Sobel", source: "assets/icons/christian.png", color:"#9932cc", sourefile: ""},
        {label: "Laplace", source: "assets/icons/enso.png", color:"#9932cc", sourefile: ""},
        {label: "Canny", source: "assets/icons/hamsa.png", color:"#f08080", sourefile: "transformations/Canny.qml"},
        {label: "Hough-Line", source: "assets/icons/infinite.png", color:"#f08080", sourefile: ""},
        {label: "Hough-Circle", source: "assets/icons/peace.png", color:"#4169e1", sourefile: ""},
        {label: "Guil-Hough", source: "assets/icons/peacebol.png", color:"#7fffd4", sourefile: ""},
        {label: "Remap", source: "assets/icons/perspective.png", color:"#f08080", sourefile: ""},
        {label: "Affine", source: "assets/icons/radiation.png", color:"#daa520", sourefile: ""}]
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
            MouseArea{
                anchors.fill: parent
                onClicked: mainWindow.getToolsInput(modelData.sourefile)
            }
        }
    }
}
