import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: _root
    color: "transparent"
    property int selectedIndex: -1
    Text{
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "Convert Image"
        font.pixelSize: 16
    }
    Column{
        id: btColumn
        anchors.top: label.bottom
        anchors.topMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10
        Repeater{
            model: [{name: "BGR to Gray"}, {name: "BGR to HSV"}, {name: "BGR to Lab"}, {name: "BGR to YCrCb"}]
            delegate: Button{
                width: _root.width/5
                height: _root.height/10
                text: modelData.name
                background: Rectangle{
                    radius: 8
                    color: (index === _root.selectedIndex) ? "#44aaff" : "white"
                    border.color: "#222"
                    border.width: 1
                }
                onClicked: {
                    _root.selectedIndex = index
                    cvController.convertImg(index)
                    mainWindow.showResult()
                }
            }
        }
    }
    Label{
        id: note
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: btColumn.bottom
        anchors.topMargin: 20
        color: "#2f4f4f"
        text: "Grayscale → edge detection, thresholding, face detection, template matching.\nHSV → color segmentation (e.g. detect red objects regardless of brightness).\nLab → color enhancement, histogram equalization (luminance channel L).\nYCrCb → skin tone detection, video/image compression."
        font.italic: true
        font.bold: true
        font.pixelSize: 14
    }
}
