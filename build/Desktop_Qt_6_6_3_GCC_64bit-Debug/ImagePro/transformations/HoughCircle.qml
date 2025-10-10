import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: _root
    color: "transparent"
    property var circleParam: [1,30,30]
    Text{
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "Hough Circle"
        font.pixelSize: 16
    }
    Row {
        id: paramlabel
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: label.bottom
        anchors.topMargin: 10
        width: parent.width/2
        height: parent.height/15
        spacing: 10
        Repeater{
            model: [{name: "Min Radius"},{name: "Max Radius"}, {name: "Distance"}]
            delegate: Label{
                text: modelData.name
                font.pixelSize: 12
                height: parent.height
                width: (parent.width-20)/3
            }
        }
    }
    Row {
        id: hough_p_thres
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: paramlabel.bottom
        width: parent.width/2
        height: parent.height/15
        spacing: 10
        Repeater{
            model: 3
            delegate: TextField{
                placeholderText: _root.circleParam[index]
                font.pixelSize: 12
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 0; top: 255 }
                height: parent.height
                width: (parent.width-20)/3
                onTextChanged: {
                    let val = parseInt(text) // convert to int
                    _root.circleParam[index] = val
                }
            }
        }
    }
    Button{
        id: result
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: hough_p_thres.bottom
        anchors.topMargin: 10
        width: parent.width/4
        height: width/5
        text: "Result"
        background: Rectangle{
            implicitWidth: 100
            implicitHeight: 40
            radius: 8
            color: "#87ceeb"
            border.color: "#222"
            border.width: 1
        }
        onClicked: {
            cvController.hough_circle(_root.circleParam[0], _root.circleParam[1], _root.circleParam[2])
            mainWindow.showResult()
        }
    }
    Label{
        id: note
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: result.bottom
        anchors.topMargin: 20
        color: "#2f4f4f"
        text: " min_dist: Minimum distance between detected centers.\nmin_radius = 0: Minimum radius to be detected. If unknown, put zero as default. \nmax_radius = 0: Maximum radius to be detected. If unknown, put zero as default."
        font.italic: true
        font.bold: true
        font.pixelSize: 14
    }
}
