import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: _root
    color: "transparent"
    property real _value: 1
    Text{
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "Gamma Correction"
        font.pixelSize: 16
    }
    Slider {
        id: _gamma
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: label.bottom
        anchors.topMargin: 10
        from: -100
        to: 100
        value: 0
        stepSize: 1
        width: parent.width*2/3

        onValueChanged: {
            if (value<0){
                let temp = (value+100)/100.0
                _root._value = 0.1 + temp*(1.0-0.1)
            }
            else{
                let temp = value/100.0;
                _root._value = 1.0 + temp*(5.0-1.0)
            }
            cvController.gammaCorrect(_root._value)
            mainWindow.showResult()
        }
    }

    Text {
        anchors.verticalCenter: _gamma.verticalCenter
        anchors.right: _gamma.left
        anchors.rightMargin: 5
        text: "Gamma: " + _root._value.toFixed(1)
        font.pointSize: 12
    }
    Label{
        id: note
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: _gamma.bottom
        anchors.topMargin: 20
        color: "#2f4f4f"
        text: "Access pixel by formula O = (I/255)^gamma*255 to change the brightness and contrast \nof the image. Using the lookupTable(LUT) to faster the mutliple operation"
        font.italic: true
        font.bold: true
        font.pixelSize: 14
    }
}
