import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: _root
    color: "transparent"
    property int h_low: 0
    property int h_high: 180
    property int s_low: 0
    property int s_high: 255
    property int v_low: 0
    property int v_high: 255
    Text{
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "HSV Color inRange"
        font.pixelSize: 16
    }
    Slider {
        id: lowerH
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: label.bottom
        anchors.topMargin: 10
        from: 0 ; to: 180 ; value: 0 ; stepSize: 1
        width: parent.width/2
        onValueChanged: {
            _root.h_low = value
            cvController.inrangeHSV(_root.h_low, _root.h_high, _root.s_low, _root.s_high, _root.v_low, _root.v_high)
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: lowerH.verticalCenter
        anchors.right: lowerH.left
        anchors.rightMargin: 5
        text: "h_low: " + lowerH.value.toFixed(0)
        font.pointSize: 12
    }
    Slider {
        id: higherH
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: lowerH.bottom
        anchors.topMargin: 10
        from: 0 ; to: 180 ; value: 180 ; stepSize: 1
        width: parent.width/2
        onValueChanged: {
            _root.h_high = value
            cvController.inrangeHSV(_root.h_low, _root.h_high, _root.s_low, _root.s_high, _root.v_low, _root.v_high)
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: higherH.verticalCenter
        anchors.right: higherH.left
        anchors.rightMargin: 5
        text: "h_high: " + higherH.value.toFixed(0)
        font.pointSize: 12
    }
    Slider {
        id: lowerS
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: higherH.bottom
        anchors.topMargin: 10
        from: 0 ; to: 255 ; value: 0 ; stepSize: 1
        width: parent.width/2
        onValueChanged: {
            _root.s_low = value
            cvController.inrangeHSV(_root.h_low, _root.h_high, _root.s_low, _root.s_high, _root.v_low, _root.v_high)
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: lowerS.verticalCenter
        anchors.right: lowerS.left
        anchors.rightMargin: 5
        text: "s_low " + lowerS.value.toFixed(0)
        font.pointSize: 12
    }
    Slider {
        id: higherS
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: lowerS.bottom
        anchors.topMargin: 10
        from: 0 ; to: 255 ; value: 255 ; stepSize: 1
        width: parent.width/2
        onValueChanged: {
            _root.s_high = value
            cvController.inrangeHSV(_root.h_low, _root.h_high, _root.s_low, _root.s_high, _root.v_low, _root.v_high)
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: higherS.verticalCenter
        anchors.right: higherS.left
        anchors.rightMargin: 5
        text: "s_high " + higherS.value.toFixed(0)
        font.pointSize: 12
    }
    Slider {
        id: lowerV
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: higherS.bottom
        anchors.topMargin: 10
        from: 0 ; to: 255 ; value: 0 ; stepSize: 1
        width: parent.width/2
        onValueChanged: {
            _root.v_low = value
            cvController.inrangeHSV(_root.h_low, _root.h_high, _root.s_low, _root.s_high, _root.v_low, _root.v_high)
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: lowerV.verticalCenter
        anchors.right: lowerV.left
        anchors.rightMargin: 5
        text: "h_low " + lowerV.value.toFixed(0)
        font.pointSize: 12
    }
    Slider {
        id: higherV
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: lowerV.bottom
        anchors.topMargin: 10
        from: 0 ; to: 255 ; value: 255 ; stepSize: 1
        width: parent.width/2
        onValueChanged: {
            _root.v_high = value
            cvController.inrangeHSV(_root.h_low, _root.h_high, _root.s_low, _root.s_high, _root.v_low, _root.v_high)
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: higherV.verticalCenter
        anchors.right: higherV.left
        anchors.rightMargin: 5
        text: "h_low " + higherV.value.toFixed(0)
        font.pointSize: 12
    }
    Label{
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: higherV.bottom
        anchors.topMargin: 20
        text: "   This operations consists of convolving an image with some kernel,\nwhich can have any shape and size. To implement the convolution, it\nrequired a structuring element(shape, size(2n+1), anchor points(n,n)).\nfirst we need to cv::getStructuringElement to declare the kernel, and\nuse cv::erode or cv::dilate to morphologiccal operations"
        font.italic: true
        font.pixelSize: 12
        font.bold: true
    }

}
