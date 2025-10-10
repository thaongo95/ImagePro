import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: _root
    color: "transparent"
    //property alias _color: erosionLabel.color
    property int ksize: 0
    property real scale: 1
    property int delta: 0
    property int selectedIndex: -1
    Text{
        id: erosionLabel
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "Sobel"
        font.pixelSize: 16
    }
    Row {
        id: _ksize
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: erosionLabel.bottom
        anchors.topMargin: 10
        width: parent.width*2/3
        height: parent.height/15
        spacing: 10
        Repeater{
            model: [{name: "Stardard", value: 3},{name: "Smoother Edges", value: 5},
            {name: "Large Gradients", value: 7},{name: "Scharr", value: 1}]
            delegate: Button{
                id: typeBt
                text: modelData.name
                font.pixelSize: 12
                background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 40
                            radius: 8
                            color: index === _root.selectedIndex ? "#44aaff" : "white"
                            border.color: "#222"
                            border.width: 1
                        }
                height: parent.height
                width: (parent.width-30)/4
                onClicked:{
                    _root.selectedIndex = index
                    _root.ksize = modelData.value
                    cvController.sobel( _root.ksize ,_root.scale, _root.delta)
                    mainWindow.showResult()
                }
            }
        }
    }

    Slider {
        id: _scale
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: _ksize.bottom
        anchors.topMargin: 10
        from: 0.5;to: 2;value: 1;stepSize: 0.05
        width: parent.width/2
        onValueChanged: {
            _root.scale = value
            cvController.sobel( _root.ksize ,  _root.scale, _root.delta)
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: _scale.verticalCenter
        anchors.right: _scale.left
        anchors.rightMargin: 5
        text: "Scale: " + _scale.value.toFixed(0)
        font.pointSize: 12
    }
    Slider {
        id: _delta
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: _scale.bottom
        anchors.topMargin: 10
        from: -255;to: 255;value: 0;stepSize: 1
        width: parent.width/2
        onValueChanged: {
            _root.delta = value
            cvController.sobel( _root.ksize ,  _root.scale, _root.delta)
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: _delta.verticalCenter
        anchors.right: _delta.left
        anchors.rightMargin: 5
        text: "Delta: " + _delta.value.toFixed(0)
        font.pointSize: 12
    }
    Label{
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: _delta.bottom
        anchors.topMargin: 20
        text: "First-order derivative (gradient-based) filter \nType: is the size of kernel: Standard(3x3);Smoother edges(5x5);Large gradients(7x7);Scharr(1x1)\nScale: to adjust gradient strength \n Delta: to shift intensity for visualizing gradients"
        font.italic: true
        font.pixelSize: 14
        font.bold: true
        color: "#2f4f4f"
    }

}
