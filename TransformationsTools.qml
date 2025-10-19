import QtQuick
import QtQuick.Controls
Item{
    id: _transTools
    property int selectedIndex: -1
    ListView{
        id: transToolsColumn
        anchors.fill: parent
        orientation: ListView.Horizontal
        spacing: 10
        clip: true
        model: [{label: "Filter2D", source: "assets/icons/ankh.png", color:"#daa520", sourcefile: "transformations/Filter2D.qml"},
        {label: "Sobel", source: "assets/icons/christian.png", color:"#9932cc", sourcefile: "transformations/Sobel.qml"},
        {label: "Laplacian", source: "assets/icons/enso.png", color:"#9932cc", sourcefile: "transformations/Laplacian.qml"},
        {label: "Canny", source: "assets/icons/hamsa.png", color:"#f08080", sourcefile: "transformations/Canny.qml"},
        {label: "Hough-Line", source: "assets/icons/infinite.png", color:"#f08080", sourcefile: "transformations/HoughLine.qml"},
        {label: "Hough-Circle", source: "assets/icons/peace.png", color:"#4169e1", sourcefile: "transformations/HoughCircle.qml"},
        {label: "Guil-Ballard", source: "assets/icons/peacebol.png", color:"#7fffd4", sourcefile: "transformations/GuilHough.qml"},
        {label: "Remap", source: "assets/icons/perspective.png", color:"#f08080", sourcefile: "transformations/Remap.qml"},
        {label: "Rotation", source: "assets/icons/chinese-symbol.png", color:"#7fffd4", sourcefile: "transformations/Rotation.qml"},
        {label: "Crop", source: "assets/icons/symbol.png", color:"#9932cc", sourcefile: "transformations/Crop.qml"},
        {label: "Affine", source: "assets/icons/radiation.png", color:"#daa520", sourcefile: "transformations/Affine.qml"}
        ]
        delegate: Rectangle{
            height: parent.height*3/4
            anchors.verticalCenter: parent.verticalCenter
            width: parent.height
            radius: width/8
            color: index === _transTools.selectedIndex ? Qt.darker(modelData.color, 1.4) :"transparent"
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
                    _transTools.selectedIndex = index
                    mainWindow.getToolsInput(modelData.sourcefile, modelData.color)
                }
            }
        }
    }
}
