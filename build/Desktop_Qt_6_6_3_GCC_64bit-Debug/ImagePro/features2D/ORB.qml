import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: _root
    color: "transparent"
    property int nfeature: 500
    property real fastThre: 20
    Text{
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "ORB detector"
        font.pixelSize: 16
    }
    Slider {
        id: _nfeature
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: label.bottom
        anchors.topMargin: 10
        from: 100;to: 3000;value: 500;stepSize: 10
        width: parent.width/2
        onValueChanged: {
            _root.nfeature = value
            cvController.orbDetection(_root.nfeature, _root.fastThre)
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: _nfeature.verticalCenter
        anchors.right: _nfeature.left
        anchors.rightMargin: 5
        text: "nfeature: "
        font.pointSize: 12
    }
    Text {
        anchors.verticalCenter: _nfeature.verticalCenter
        anchors.left: _nfeature.right
        anchors.leftMargin: 5
        text: _nfeature.value.toFixed(0)
        font.pointSize: 12
    }
    Slider {
        id: _fastThre
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: _nfeature.bottom
        anchors.topMargin: 10
        from: 5;to: 50;value: 20;stepSize: 1
        width: parent.width/2
        onValueChanged: {
            _root.fastThre = value
            cvController.orbDetection(_root.nfeature, _root.fastThre)
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: _fastThre.verticalCenter
        anchors.right: _fastThre.left
        anchors.rightMargin: 5
        text: "fastThreshold: "
        font.pointSize: 12
    }
    Text {
        anchors.verticalCenter: _fastThre.verticalCenter
        anchors.left: _fastThre.right
        anchors.leftMargin: 5
        text: _fastThre.value.toFixed(0)
        font.pointSize: 12
    }
    Label{
        id: note
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: _fastThre.bottom
        anchors.topMargin: 20
        color: "#2f4f4f"
        text: "More detection: ↓ fastThreshold, ↑ nfeatures; Faster runtime: ↑ fastThreshold, ↓ nfeatures"
        font.italic: true
        font.bold: true
        font.pixelSize: 14
    }
}
