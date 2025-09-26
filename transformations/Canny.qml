import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: canny
    color: "transparent"
    property int low_thre: 50
    property int high_thre: 200
    Slider {
        id: scaleSlider_l
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 10
        from: 1
        to: 180
        value: 50
        stepSize: 1
        width: 400

        onValueChanged: {
            canny.low_thre = value
            cvController.cannyDetect(canny.low_thre, canny.high_thre)
            mainWindow.showResult()
        }
    }

    Text {
        anchors.verticalCenter: scaleSlider_l.verticalCenter
        anchors.right: scaleSlider_l.left
        anchors.rightMargin: 5
        text: "Low threshold: " + scaleSlider_l.value.toFixed(0)
        font.pointSize: 14
    }
    Slider {
        id: scaleSlider_h
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: scaleSlider_l.bottom
        anchors.topMargin: 10
        from: 1
        to: 255
        value: 200
        stepSize: 1
        width: 400

        onValueChanged: {
            canny.high_thre = value
            cvController.cannyDetect(canny.low_thre, canny.high_thre)
            mainWindow.showResult()
        }
    }

    Text {
        anchors.verticalCenter: scaleSlider_h.verticalCenter
        anchors.right: scaleSlider_h.left
        anchors.rightMargin: 5
        text: "High threshold: " + scaleSlider_h.value.toFixed(0)
        font.pointSize: 14
    }
}
