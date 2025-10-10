import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: _root
    color: "transparent"
    property int scale: 200
    Text{
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "Harris corner detector"
        font.pixelSize: 16
    }
    Slider {
        id: _scale
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: label.bottom
        anchors.topMargin: 10
        from: 0;to: 255;value: 200;stepSize: 1
        width: parent.width/2
        onValueChanged: {
            _root.scale = value
            cvController.harris_corner(_root.scale)
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: _scale.verticalCenter
        anchors.right: _scale.left
        anchors.rightMargin: 5
        text: "Threshold: " + _scale.value.toFixed(0)
        font.pointSize: 12
    }
    Label{
        id: note
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: _scale.bottom
        anchors.topMargin: 20
        color: "#2f4f4f"
        text: "void cv::cornerHarris\n"
               + "InputArray src, \n"
                + "OutputArray dst,\n"
                + "int blockSize,\n"
                + "int ksize,\n"
                + "double k,\n"
                + "int borderType = BORDER_DEFAULT);\n"
        font.italic: true
        font.bold: true
        font.pixelSize: 14
    }
}
