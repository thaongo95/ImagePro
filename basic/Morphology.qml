import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: _root
    color: "transparent"
    //property alias _color: erosionLabel.color
    property int morph_type: 0
    property int morph_size: 0
    property int morph_operator: 0

    function getType(temp){
        if( temp === 0 ){ return "MORPH_RECT" }
        else if( temp === 1 ){ return "MORPH_CROSS"}
        else if( temp === 2) { return "MORPH_ELLIPSE"}
    }
    function getMorph(temp){
        if( temp === 0 ){ return "OPENING" }
        else if( temp === 1 ){ return "CLOSING"}
        else if( temp === 2) { return "GRADIENT"}
        else if( temp === 3) { return "TOP HAT"}
        else if( temp === 4) { return "BLACK HAT"}
    }
    Text{
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "Morphology Transformations"
        font.pixelSize: 16
    }
    Row {
        id: morphOperator
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: label.bottom
        anchors.topMargin: 10
        width: parent.width*2/3
        height: parent.height/15
        spacing: 5
        Repeater{
            model: 5
            delegate: Button{
                id: operatorBt
                text: _root.getMorph(index)
                font.pixelSize: 12
                background: Rectangle {
                            implicitWidth: 80
                            implicitHeight: 40
                            radius: 8
                            color: index === _root.morph_operator ? "#44aaff" : "white"
                            border.color: "#222"
                            border.width: 1
                        }
                height: parent.height
                width: (parent.width-20)/5
                onClicked:{
                    _root.morph_operator = index
                    cvController.morphology( _root.morph_operator ,  _root.morph_type, _root.morph_size)
                    mainWindow.showResult()
                }
            }
        }
    }
    Row {
        id: morphType
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: morphOperator.bottom
        anchors.topMargin: 10
        width: parent.width*2/3
        height: parent.height/15
        spacing: 10
        Repeater{
            model: 3
            delegate: Button{
                id: typeBt
                text: _root.getType(index)
                font.pixelSize: 12
                background: Rectangle {
                            implicitWidth: 80
                            implicitHeight: 40
                            radius: 8
                            color: index === _root.morph_type ? "#44aaff" : "white"
                            border.color: "#222"
                            border.width: 1
                        }
                height: parent.height
                width: (parent.width-20)/3
                onClicked:{
                    _root.morph_type = index
                    cvController.morphology( _root.morph_operator ,  _root.morph_type, _root.morph_size)
                    mainWindow.showResult()
                }
            }
        }
    }
    Slider {
        id: morphSize
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: morphType.bottom
        anchors.topMargin: 10
        from: 0
        to: 21
        value: 0
        stepSize: 1
        width: parent.width/2
        onValueChanged: {
            _root.morph_size = value
            cvController.morphology( _root.morph_operator ,  _root.morph_type, _root.morph_size)
            mainWindow.showResult()
        }
    }

    Text {
        anchors.verticalCenter: morphSize.verticalCenter
        anchors.right: morphSize.left
        anchors.rightMargin: 5
        text: "Size " + morphSize.value.toFixed(0)
        font.pointSize: 12
    }

    Label{
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: morphSize.bottom
        anchors.topMargin: 20
        text: "dst = open(src, element) = dilate(erode(src,element)) \n dst = open(src, element) = erode(dilate(src,element)) \n dst = grad(src, element) = dilate(src, element) - erode(src,element) \n dst = top_hat(src, element) = src - open(src,element)\n dst = black_hat(src, element) = close(src, element) - src"
        font.italic: true
        font.pixelSize: 12
        font.bold: true
        color: "#2f4f4f"
    }


}
