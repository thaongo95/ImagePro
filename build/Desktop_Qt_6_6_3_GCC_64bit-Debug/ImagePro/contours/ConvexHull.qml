import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: _root
    color: "transparent"
    property int cannyThres: 100
    property int colorType: 0
    Text{
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "Convex Hull"
        font.pixelSize: 16
    }
    Slider {
        id: canny_thres
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: label.bottom
        anchors.topMargin: 10
        from: 0 ; to: 180 ; value: 100 ; stepSize: 1
        width: parent.width/2
        onValueChanged: {
            _root.cannyThres = value
            cvController.convexHull(_root.cannyThres, _root.colorType)
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: canny_thres.verticalCenter
        anchors.right: canny_thres.left
        anchors.rightMargin: 5
        text: "Canny threshold: " + canny_thres.value.toFixed(0)
        font.pointSize: 12
    }
    Row {
        id: color_type
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: canny_thres.bottom
        anchors.topMargin: 10
        width: parent.width/2
        height: parent.height/15
        spacing: 10
        Repeater{
            model: [{name: "RED"},{name: "GREEN"},{name: "BLUE"}, {name: "YELLOW"}, {name: "RANDOM"}]
            delegate: Button{
                text: modelData.name
                font.pixelSize: 12
                background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 40
                            radius: 8
                            color: index === _root.colorType ? "#44aaff" : "white"
                            border.color: "#222"
                            border.width: 1
                        }
                height: parent.height
                width: (parent.width-40)/5
                onClicked:{
                    _root.colorType = index
                    cvController.convexHull(_root.cannyThres, _root.colorType)
                    mainWindow.showResult()
                }
            }
        }
    }
    Label{
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: color_type.bottom
        anchors.topMargin: 20
        text: "   This operations consists of convolving an image with some kernel,\nwhich can have any shape and size. To implement the convolution, it\nrequired a structuring element(shape, size(2n+1), anchor points(n,n)).\nfirst we need to cv::getStructuringElement to declare the kernel, and\nuse cv::erode or cv::dilate to morphologiccal operations"
        font.italic: true
        font.pixelSize: 12
        font.bold: true
    }

}
