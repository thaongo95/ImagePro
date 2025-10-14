import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: _root
    color: "transparent"
    property real thresh: 0.004
    Text{
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "AKAZE detector"
        font.pixelSize: 16
    }
    Slider {
        id: _thresh
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: label.bottom
        anchors.topMargin: 10
        from: 0.001;to: 0.03;value: 0.004;stepSize: 0.001
        width: parent.width/2
        onValueChanged: {
            _root.thresh = value
            cvController.akazeDetection(_root.thresh)
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: _thresh.verticalCenter
        anchors.right: _thresh.left
        anchors.rightMargin: 5
        text: "Threshold: "
        font.pointSize: 12
    }
    Text {
        anchors.verticalCenter: _thresh.verticalCenter
        anchors.left: _thresh.right
        anchors.leftMargin: 5
        text: _thresh.value.toFixed(3)
        font.pointSize: 12
    }
    Label{
        id: note
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: _thresh.bottom
        anchors.topMargin: 20
        color: "#2f4f4f"
        text: "Lower → more features (but noisier); Higher → fewer, stronger features"
        font.italic: true
        font.bold: true
        font.pixelSize: 14
    }
}
