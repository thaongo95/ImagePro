import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: _root
    color: "transparent"
    property real kernelSize: 0.1
    property int kernelType: -1
    Text{
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "Extract Lines"
        font.pixelSize: 16
    }

    Row {
        id: _kernelType
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: label.bottom
        anchors.topMargin: 10
        width: parent.width/2
        height: parent.height/15
        spacing: 10
        Repeater{
            model: [{name: "Horizontal"},{name: "Vertical"}]
            delegate: Button{
                text: modelData.name
                font.pixelSize: 12
                height: parent.height
                width: parent.width/2-10
                background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 40
                            radius: 8
                            color: index === _root.kernelType ? "#44aaff" : "white"
                            border.color: "#222"
                            border.width: 1
                        }
                onClicked:{
                    _root.kernelType = index
                    cvController.extractLines(_root.kernelType,  _root.kernelSize)
                    mainWindow.showResult()
                }
            }
        }
    }
    Slider {
        id: _kernelSize
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: _kernelType.bottom
        anchors.topMargin: 10
        from: 0.01
        to: 0.2
        value: 0.01
        stepSize: 0.01
        width: parent.width/2
        onValueChanged: {
            _root.kernelSize = value
            cvController.extractLines(_root.kernelType,  _root.kernelSize)
            mainWindow.showResult()
        }
    }

    Text {
        anchors.verticalCenter: _kernelSize.verticalCenter
        anchors.right: _kernelSize.left
        anchors.rightMargin: 5
        text: "Size " + _kernelSize.value.toFixed(2)
        font.pointSize: 12
    }

    Label{
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: _kernelSize.bottom
        anchors.topMargin: 20
        text: "   This operations consists of convolving an image with some kernel,\nwhich can have any shape and size. To implement the convolution, it\nrequired a structuring element(shape, size(2n+1), anchor points(n,n)).\nfirst we need to cv::getStructuringElement to declare the kernel, and\nuse cv::erode or cv::dilate to morphologiccal operations"
              + "\n Dilation: The value of the output pixel is the maximum value of all \n the pixels that fall within the structuring element's size and shape; \nErosion: The vice versa"
        font.italic: true
        font.pixelSize: 14
        font.bold: true
        color: "#2f4f4f"
    }

}
