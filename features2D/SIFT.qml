import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: _root
    color: "transparent"
    property int nfeature: 500
    property real constastThre: 0.04
    Text{
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "SIFT detector"
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
            cvController.siftDetection(_root.nfeature, _root.constastThre)
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
        id: _contrastThre
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: _nfeature.bottom
        anchors.topMargin: 10
        from: 0.001;to: 0.1;value: 0.04;stepSize: 0.001
        width: parent.width/2
        onValueChanged: {
            _root.constastThre = value
            cvController.siftDetection(_root.nfeature, _root.constastThre)
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: _contrastThre.verticalCenter
        anchors.right: _contrastThre.left
        anchors.rightMargin: 5
        text: "contrastThreshold: "
        font.pointSize: 12
    }
    Text {
        anchors.verticalCenter: _contrastThre.verticalCenter
        anchors.left: _contrastThre.right
        anchors.leftMargin: 5
        text: _contrastThre.value.toFixed(3)
        font.pointSize: 12
    }
    Label{
        id: note
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: _contrastThre.bottom
        anchors.topMargin: 20
        color: "#2f4f4f"
        text: "If you want more keypoints, lower contrastThreshold or raise nfeatures."
        font.italic: true
        font.bold: true
        font.pixelSize: 14
    }
}
