import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: _root
    color: "transparent"
    property int houghType: 0
    property int houghThres: 150
    property var houghPThres: [50, 50, 10]
    Text{
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "Hough Line Transform"
        font.pixelSize: 16
    }
    Row {
        id: _kernelType
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: label.bottom
        anchors.topMargin: 10
        width: parent.width/2
        height: parent.height/15
        spacing: 10
        Repeater{
            model: [{name: "STANDARD"},{name: "PROBABILISIC"}]
            delegate: Button{
                text: modelData.name
                font.pixelSize: 12
                height: parent.height
                width: parent.width/2-10
                background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 40
                            radius: 8
                            color: index === _root.houghType ? "#44aaff" : "white"
                            border.color: "#222"
                            border.width: 1
                        }
                onClicked:{
                    _root.houghType = index
                }
            }
        }
    }
    TextField{
        id: hough_thres
        anchors.top: _kernelType.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: _kernelType.horizontalCenter
        placeholderText: "Threshold"
        font.pixelSize: 12
        inputMethodHints: Qt.ImhDigitsOnly
        height: _kernelType.height
        width: _kernelType.width/3
        visible: _root.houghType===0
        onTextChanged: {
            let val = parseInt(text) // convert to int
            _root.houghThres = val
        }
    }
    Row {
        id: hough_p_thres
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: hough_thres.bottom
        anchors.topMargin: 10
        width: parent.width/2
        height: parent.height/15
        visible: _root.houghType===1
        spacing: 10
        Repeater{
            model: [{holder: "Threshold"}, {holder: "Min Line Length"}, {holder: "Max Line Gap"}]
            delegate: TextField{
                placeholderText: modelData.holder
                font.pixelSize: 12
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 0; top: 255 }
                height: parent.height
                width: (parent.width-20)/3
                onTextChanged: {
                    let val = parseInt(text) // convert to int
                    _root.houghPThres[index] = val
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
        height: width/3
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
            if(_root.houghType===0){
                cvController.houghline(_root.houghThres)
                mainWindow.showResult()
            }
            else{
                cvController.houghline_p(_root.houghPThres[0], _root.houghPThres[1], _root.houghPThres[2])
                mainWindow.showResult()
            }
        }
    }
    Label{
        id: note
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: result.bottom
        anchors.topMargin: 20
        color: "#2f4f4f"
        text: "The Hough Line Transform (cv::HoughLines or cv::HoughLinesP) is a classic tool \nfor detecting straight lines in an image, especially after applying Canny edge detection."
        font.italic: true
        font.bold: true
        font.pixelSize: 14
    }
}
