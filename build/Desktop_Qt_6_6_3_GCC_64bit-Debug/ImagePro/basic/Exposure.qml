import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: exposure
    color: "transparent"
    property real alpha: 1
    property int beta: 0
    Text{
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "Changing the contrast and brightness"
        font.pixelSize: 16
    }
    Slider {
        id: _alpha
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: label.bottom
        anchors.topMargin: 20
        from: 1
        to: 3
        value: 1
        stepSize: 0.1
        width: parent.width*2/3

        onValueChanged: {
            exposure.alpha = value
            cvController.basicExposure(exposure.alpha , exposure.beta)
            mainWindow.showResult()
        }
    }

    Text {
        anchors.verticalCenter: _alpha.verticalCenter
        anchors.right: _alpha.left
        anchors.rightMargin: 5
        text: "Contrast: " + _alpha.value.toFixed(1)
        font.pointSize: 12
    }
    Slider {
        id: _beta
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: _alpha.bottom
        anchors.topMargin: 15
        from: 0
        to: 100
        value: 0
        stepSize: 1
        width: parent.width*2/3

        onValueChanged: {
            exposure.beta = value
            cvController.basicExposure(exposure.alpha , exposure.beta)
            mainWindow.showResult()
        }
    }

    Text {
        anchors.verticalCenter: _beta.verticalCenter
        anchors.right: _beta.left
        anchors.rightMargin: 5
        text: "Brightness: " + _beta.value.toFixed(0)
        font.pointSize: 12
    }
    Label{
        id: note
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: _beta.bottom
        anchors.topMargin: 20
        color: "#2f4f4f"
        text: "Access pixel by formula g(x) = alpha*f(x) + beta to change the brightness and contrast \nof the image. Using the cv::saturate_cast<>() to clip the value out of range"
        font.italic: true
        font.bold: true
        font.pixelSize: 14
    }
}
