import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: _root
    color: "transparent"
    property int imageType: 0
    property int blendingScale: 1
    Text{
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "Histograms Equalization"
        font.pixelSize: 16
    }
    Row {
        id: _imageType
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: label.bottom
        anchors.topMargin: 10
        width: parent.width/2
        height: parent.height/15
        spacing: 10
        Repeater{
            model: [{name: "Gray"},{name: "YCrCb"}]
            delegate: Button{
                text: modelData.name
                font.pixelSize: 12
                height: parent.height
                width: parent.width/2-10
                background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 40
                            radius: 8
                            color: index === _root.imageType ? "#44aaff" : "white"
                            border.color: "#222"
                            border.width: 1
                        }
                onClicked:{
                    _root.imageType = index
                    cvController.his_equalization(_root.imageType, _root.blendingScale)
                    mainWindow.showResult()
                }
            }
        }
    }
    Slider {
        id: _scale
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: _imageType.bottom
        anchors.topMargin: 10
        from: 0;to: 1;value: 1;stepSize: 0.05
        width: parent.width/2
        onValueChanged: {
            _root.blendingScale = value
            cvController.his_equalization(_root.imageType, _root.blendingScale)
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: _scale.verticalCenter
        anchors.right: _scale.left
        anchors.rightMargin: 5
        text: "Blend: " + _scale.value.toFixed(2)
        font.pointSize: 12
    }
    Label{
        id: note
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: _scale.bottom
        anchors.topMargin: 20
        color: "#2f4f4f"
        text: "To control the intensity, you can blend the result with the original image"
        font.italic: true
        font.bold: true
        font.pixelSize: 14
    }
}
