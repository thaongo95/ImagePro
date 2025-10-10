import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: _root
    color: "transparent"
    property var hsv_array: [0, 180, 0, 255, 0, 255]
    property int selectedIndex: -1
    property bool isCustom: false
    property int erea: 500
    Text{
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "Color Detection"
        font.pixelSize: 16
    }
    Row {
        id: colorSelect
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: label.bottom
        anchors.topMargin: 10
        width: parent.width*2/3
        height: parent.height/15
        spacing: 6
        Repeater{
            model: [{name: "RED"},{name: "GREEN"},{name: "BLUE"},{name: "YELLOW"},{name: "BLACK"},{name: "CUSTOM"}]
            delegate: Button{
                text: modelData.name
                font.pixelSize: 12
                background: Rectangle {
                            implicitWidth: 80
                            implicitHeight: 40
                            radius: 8
                            color: index === _root.selectedIndex ? "#44aaff" : "white"
                            border.color: "#222"
                            border.width: 1
                        }
                height: parent.height
                width: parent.width/5-20
                onClicked:{
                    if (index===5) {_root.isCustom = true}
                    else (_root.isCustom=false)
                    _root.selectedIndex = index
                }
            }
        }
    }
    Row {
        id: hsvValue
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: colorSelect.bottom
        anchors.topMargin: 10
        width: parent.width/2
        height: parent.height/15
        visible: isCustom
        spacing: 10
        Repeater{
            model: [{holder: "h_low"}, {holder: "h_high"}, {holder: "s_low"}, {holder: "s_high"}, {holder: "v_low"}, {holder: "v_high"}]
            delegate: TextField{
                placeholderText: modelData.holder
                font.pixelSize: 12
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 0; top: 255 }
                height: parent.height
                width: (parent.width-50)/6
                onTextChanged: {
                    let val = parseInt(text) // convert to int
                    _root.hsv_array[index] = val
                }
            }
        }
    }
    TextField{
        id: ereaInput
        anchors.bottom: result.top
        anchors.bottomMargin: 10
        anchors.horizontalCenter: result.horizontalCenter
        placeholderText: _root.erea
        font.pixelSize: 12
        inputMethodHints: Qt.ImhDigitsOnly
        height: result.height
        width: result.width/2
        onTextChanged: {
            let val = parseInt(text) // convert to int
            _root.erea = val
        }
    }
    Button{
        id: result
        anchors.centerIn: parent
        width: _root.width/5
        height: _root.height/10
        text: "Result"
        background: Rectangle{
            radius: 8
            color:  "#44aaff"
            border.color: "#222"
            border.width: 1
        }
        onClicked: {
            if (isCustom) {
                cvController.customColorDetect(_root.erea, _root.hsv_array[0],_root.hsv_array[1],_root.hsv_array[2],_root.hsv_array[3],_root.hsv_array[4],_root.hsv_array[5])
            }
            else {
                cvController.enumColorDetect(selectedIndex, _root.erea)
            }
            mainWindow.showResult()
        }
    }

    // Label{
    //     anchors.horizontalCenter: parent.horizontalCenter
    //     anchors.top: morphSize.bottom
    //     anchors.topMargin: 20
    //     text: "dst = open(src, element) = dilate(erode(src,element)) \n dst = open(src, element) = erode(dilate(src,element)) \n dst = grad(src, element) = dilate(src, element) - erode(src,element) \n dst = top_hat(src, element) = src - open(src,element)\n dst = black_hat(src, element) = close(src, element) - src"
    //     font.italic: true
    //     font.pixelSize: 12
    //     font.bold: true
    //     color: "#2f4f4f"
    // }


}
