import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: _root
    color: "transparent"
    //property alias _color: erosionLabel.color
    property int erosion_type: 0
    property int erosion_size: 0
    property int dilation_type: 0
    property int dilation_size: 0
    function getType(temp){
        if( temp === 0 ){ return "MORPH_RECT" }
        else if( temp === 1 ){ return "MORPH_CROSS"}
        else if( temp === 2) { return "MORPH_ELLIPSE"}
    }
    Text{
        id: erosionLabel
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "Erosion"
        font.pixelSize: 16
    }
    Row {
        id: erodeType
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: erosionLabel.bottom
        anchors.topMargin: 10
        width: parent.width*2/3
        height: parent.height/15
        spacing: 10
        Repeater{
            model: 3
            delegate: Button{
                id: typeBt
                text: _root.getType(index)
                font.pixelSize: 12
                background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 40
                            radius: 8
                            color: typeBt.pressed ? "#44aaff" : "white"
                            border.color: "#222"
                            border.width: 1
                        }
                height: parent.height
                width: parent.width/3-20
                onClicked:{
                    _root.erosion_type = index
                    cvController.erosion( _root.erosion_type ,  _root.erosion_size)
                    mainWindow.showResult()
                }
            }
        }
    }

    Slider {
        id: erodeSize
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: erodeType.bottom
        anchors.topMargin: 10
        from: 0
        to: 21
        value: 0
        stepSize: 1
        width: 400
        onValueChanged: {
            _root.erosion_size = value
            cvController.erosion(_root.erosion_type , _root.erosion_size)
            mainWindow.showResult()
        }
    }

    Text {
        anchors.verticalCenter: erodeSize.verticalCenter
        anchors.right: erodeSize.left
        anchors.rightMargin: 5
        text: "Size " + erodeSize.value.toFixed(0)
        font.pointSize: 12
    }
    Text{
            id: dilationLabel
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: erodeSize.bottom
            anchors.topMargin: 15
            text: "Dilation"
            font.pixelSize: 16
        }
    Row {
        id: dilateType
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: dilationLabel.bottom
        anchors.topMargin: 10
        width: parent.width*2/3
        height: parent.height/15
        spacing: 10
        Repeater{
            model: 3
            delegate: Button{
                text: _root.getType(index)
                font.pixelSize: 12
                height: parent.height
                width: parent.width/3-20
                onClicked:{
                    _root.dilation_type = index
                    cvController.dilation( _root.dilation_type ,  _root.dilation_size)
                    mainWindow.showResult()
                }
            }
        }
    }
    Slider {
        id: dilateSize
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: dilateType.bottom
        anchors.topMargin: 10
        from: 0
        to: 21
        value: 0
        stepSize: 1
        width: 400
        onValueChanged: {
            _root.dilation_size = value
            cvController.dilation( _root.dilation_type ,  _root.dilation_size)
            mainWindow.showResult()
        }
    }

    Text {
        anchors.verticalCenter: dilateSize.verticalCenter
        anchors.right: dilateSize.left
        anchors.rightMargin: 5
        text: "Size " + dilateSize.value.toFixed(0)
        font.pointSize: 14
    }

    Label{
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: dilateSize.bottom
        anchors.topMargin: 20
        text: "   This operations consists of convolving an image with some kernel,\nwhich can have any shape and size. To implement the convolution, it\nrequired a structuring element(shape, size(2n+1), anchor points(n,n)).\nfirst we need to cv::getStructuringElement to declare the kernel, and\nuse cv::erode or cv::dilate to morphologiccal operations"
        font.italic: true
        font.pixelSize: 12
        font.bold: true
    }

}
