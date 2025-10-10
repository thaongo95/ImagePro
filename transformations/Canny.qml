import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: _root
    color: "transparent"
    property int low_thre: 50
    property int high_thre: 200
    property int ksize: 3
    property bool l2: false
    property int ksizeIndex: 0
    property int l2Index: 0
    Text{
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "Sobel"
        font.pixelSize: 16
    }
    Row {
        id: _ksize
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: label.bottom
        anchors.topMargin: 10
        width: parent.width/3
        height: parent.height/15
        spacing: 10
        Repeater{
            model: [{name: "3x3", value: 3},{name: "5x5", value: 5},{name: "7x7", value: 7}]
            delegate: Button{
                text: modelData.name
                font.pixelSize: 12
                background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 40
                            radius: 8
                            color: index === _root.ksizeIndex ? "#44aaff" : "white"
                            border.color: "#222"
                            border.width: 1
                        }
                height: parent.height
                width: (parent.width-20)/3
                onClicked:{
                    _root.ksizeIndex = index
                    _root.ksize = modelData.value
                    cvController.cannyDetect( _root.low_thre ,_root.high_thre, _root.ksize, _root.l2)
                    mainWindow.showResult()
                }
            }
        }
    }
    Slider {
        id: low_thres
        anchors.right: parent.horizontalCenter
        anchors.top: _ksize.bottom
        anchors.topMargin: 10
        from: 1; to: 150; value: 50; stepSize: 1
        width: parent.width/4

        onValueChanged: {
            _root.low_thre = value
            cvController.cannyDetect( _root.low_thre ,_root.high_thre, _root.ksize, _root.l2)
            mainWindow.showResult()
        }
    }

    Text {
        anchors.verticalCenter: low_thres.verticalCenter
        anchors.right: low_thres.left
        anchors.rightMargin: 5
        text: "Low threshold: " + low_thres.value.toFixed(0)
        font.pointSize: 14
    }
    Slider {
        id: high_thres
        anchors.left: parent.horizontalCenter
        anchors.top: low_thres.bottom
        anchors.topMargin: 10
        from: 100
        to: 300
        value: 150
        stepSize: 1
        width: parent.width/4
        onValueChanged: {
            _root.high_thre = value
            cvController.cannyDetect( _root.low_thre ,_root.high_thre, _root.ksize, _root.l2)
            mainWindow.showResult()
        }
    }

    Text {
        anchors.verticalCenter: high_thres.verticalCenter
        anchors.right: low_thres.left
        anchors.rightMargin: 5
        text: "High threshold: " + high_thres.value.toFixed(0)
        font.pointSize: 14
    }
    Row {
        id: _l2
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: high_thres.bottom
        anchors.topMargin: 10
        width: parent.width/3
        height: parent.height/15
        spacing: 10
        Repeater{
            model: [{name: "L1 gradient", value: false},{name: "L2 gradient", value: true}]
            delegate: Button{
                text: modelData.name
                font.pixelSize: 12
                background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 40
                            radius: 8
                            color: index === _root.l2Index ? "#44aaff" : "white"
                            border.color: "#222"
                            border.width: 1
                        }
                height: parent.height
                width: (parent.width-10)/2
                onClicked:{
                    _root.l2Index = index
                    _root.l2 = modelData.value
                    cvController.cannyDetect( _root.low_thre ,_root.high_thre, _root.ksize, _root.l2)
                    mainWindow.showResult()
                }
            }
        }
    }
    Label{
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: _l2.bottom
        anchors.topMargin: 20
        text: "Low threshold controls which gradients are considered weak edges\n"
            + "Pixels with gradient magnitude below this are discarded immediately\n"
            + "High threshold marks strong edges, any gradient above this is definitely an edge\n"
            + "Weak edges (between low and high) are included only if connected to strong edges\n"
            + "uses L1 norm → |dx| + |dy|; uses L2 norm → sqrt(dx² + dy²)\n"
        font.italic: true
        font.pixelSize: 14
        font.bold: true
        color: "#2f4f4f"
    }
}
