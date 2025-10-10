import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: _root
    color: "transparent"
    property int kernelType: -1
    Text{
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "Filter Image"
        font.pixelSize: 16
    }
    Grid{
        id: btColumn
        anchors.top: label.bottom
        anchors.topMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter
        columns: 3
        rows: 3
        width: parent.width/2
        height: width/2
        spacing: 10
        Repeater{
            model: [{name: "Sharpen"}, {name: "Stronger Sharpen"}, {name: "Edge Horizontal"},
                {name: "Edge Vertical"}, {name: "Laplacian"}, {name: "Stronger Laplacian"},
                {name: "Emboss"}, {name: "Outline"}, {name: "Normalize"}]
            delegate: Button{
                width: (parent.width-20)/3
                height: (parent.height-20)/3
                text: modelData.name
                background: Rectangle{
                    radius: 8
                    color: (_root.kernelType === index) ? "#44aaff" : "white"
                    border.color: "#222"
                    border.width: 1
                }
                onClicked: {
                    _root.kernelType = index
                    cvController.filter_2d(_root.kernelType)
                    mainWindow.showResult()
                }
            }
        }
    }
    Label{
        id: note
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: btColumn.bottom
        anchors.topMargin: 20
        color: "#2f4f4f"
        text: "Add 3x3 convolution filter"
        font.italic: true
        font.bold: true
        font.pixelSize: 14
    }
}
