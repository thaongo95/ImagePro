import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: _root
    color: "transparent"
    property int scale: 5
    property int cornerType: 0
    Text{
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "Custom corner detector"
        font.pixelSize: 16
    }
    Row {
        id: _cornerType
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: label.bottom
        anchors.topMargin: 10
        width: parent.width/2
        height: parent.height/15
        spacing: 10
        Repeater{
            model: [{name: "Harris"},{name: "ShiTomasi"}]
            delegate: Button{
                text: modelData.name
                font.pixelSize: 12
                height: parent.height
                width: parent.width/2-10
                background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 40
                            radius: 8
                            color: index === _root.cornerType ? "#44aaff" : "white"
                            border.color: "#222"
                            border.width: 1
                        }
                onClicked:{
                    _root.cornerType = index
                    cvController.custom(_root.cornerType,  _root.scale)
                    mainWindow.showResult()
                }
            }
        }
    }
    Slider {
        id: _scale
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: _cornerType.bottom
        anchors.topMargin: 10
        from: 1;to: 100;value: 5;stepSize: 1
        width: parent.width/2
        onValueChanged: {
            _root.scale = value
            cvController.custom(_root.cornerType,  _root.scale)
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: _scale.verticalCenter
        anchors.right: _scale.left
        anchors.rightMargin: 5
        text: "Level: " + _scale.value.toFixed(0)
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
