import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: _root
    color: "transparent"
    property int kernelType: -1
    property int kernelSize: 1
    Text{
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "Smoothing Image"
        font.pixelSize: 16
    }
    Column{
        id: btColumn
        anchors.top: label.bottom
        anchors.topMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10
        Repeater{
            model: [{name: "Blur"}, {name: "Box Blur"}, {name: "Gaussian Blur"}, {name: "Median Blur"}, {name: "Bilateral Blur"}]
            delegate: Button{
                width: _root.width/5
                height: _root.height/10
                text: modelData.name
                background: Rectangle{
                    radius: 8
                    color: (_root.kernelType === index) ? "#44aaff" : "white"
                    border.color: "#222"
                    border.width: 1
                }
                onClicked: {
                    _root.kernelType = index
                    cvController.smoothing(_root.kernelType, _root.kernelSize)
                    mainWindow.showResult()
                }
            }
        }
    }
    Slider {
        id: _size
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: btColumn.bottom
        anchors.topMargin: 10
        from: 1
        to: 21
        value: 1
        stepSize: 2
        width: parent.width*2/3

        onValueChanged: {
            _root.kernelSize = value
            cvController.smoothing(_root.kernelType, _root.kernelSize)
            mainWindow.showResult()
        }
    }

    Text {
        anchors.verticalCenter: _size.verticalCenter
        anchors.right: _size.left
        anchors.rightMargin: 5
        text: "Kernel Size: " + _root.kernelSize.toFixed(1)
        font.pointSize: 12
    }
    Label{
        id: note
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: _size.bottom
        anchors.topMargin: 20
        color: "#2f4f4f"
        text: "Add filter to smoothing or blur image"
        font.italic: true
        font.bold: true
        font.pixelSize: 14
    }
}
