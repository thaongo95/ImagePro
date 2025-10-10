import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: _root
    color: "transparent"
    property int rotateAngle: 90
    property real rotateScale: 1
    Text{
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "Rotation"
        font.pixelSize: 16
    }
    Slider {
        id: _angle
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: label.bottom
        anchors.topMargin: 10
        from: -180;to: 180;value: 0;stepSize: 1
        width: parent.width/2
        onValueChanged: {
            _root.rotateAngle = value
            cvController.rotation( _root.rotateAngle ,  _root.rotateScale)
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: _angle.verticalCenter
        anchors.right: _angle.left
        anchors.rightMargin: 5
        text: "Angle: " + _angle.value.toFixed(0) +"°"
        font.pointSize: 12
    }
    Slider {
        id: _scale
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: _angle.bottom
        anchors.topMargin: 10
        from: 0;to: 1;value: 1;stepSize: 0.05
        width: parent.width/2
        onValueChanged: {
            _root.rotateScale = value
            cvController.rotation( _root.rotateAngle ,  _root.rotateScale)
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: _scale.verticalCenter
        anchors.right: _scale.left
        anchors.rightMargin: 5
        text: "Scale: " + _scale.value.toFixed(2)
        font.pointSize: 12
    }
    Label{
        id: note
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: _scale.bottom
        anchors.topMargin: 20
        color: "#2f4f4f"
        text: " "
        font.italic: true
        font.bold: true
        font.pixelSize: 14
    }
}
