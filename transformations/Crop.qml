import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: _root
    color: "transparent"
    property real pt1_x_scale: 0.2
    property real pt1_y_scale: 0.2
    property real pt2_x_scale: 0.8
    property real pt2_y_scale: 0.8
    property bool isCrop: false
    Text{
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "Crop Image"
        font.pixelSize: 16
    }
    Slider {
        id: point1_x_scale
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: label.bottom
        anchors.topMargin: 10
        from: 0;to: 1;value: _root.pt1_x_scale ;stepSize: 0.01
        width: parent.width/2
        onValueChanged: {
            _root.pt1_x_scale = value
            _root.isCrop = false
            cvController.crop( _root.pt1_x_scale ,   _root.pt1_y_scale , _root.pt2_x_scale ,
                                _root.pt2_y_scale, _root.isCrop)
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: point1_x_scale.verticalCenter
        anchors.right: point1_x_scale.left
        anchors.rightMargin: 5
        text: "Point 1-x: "
        font.pointSize: 12
    }
    Slider {
        id: point1_y_scale
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: point1_x_scale.bottom
        anchors.topMargin: 10
        from: 0;to: 1;value: _root.pt1_y_scale ;stepSize: 0.01
        width: parent.width/2
        onValueChanged: {
            _root.pt1_y_scale = value
            _root.isCrop = false
            cvController.crop( _root.pt1_x_scale ,   _root.pt1_y_scale , _root.pt2_x_scale ,
                                _root.pt2_y_scale, _root.isCrop)
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: point1_y_scale.verticalCenter
        anchors.right: point1_y_scale.left
        anchors.rightMargin: 5
        text: "Point 1-y: "
        font.pointSize: 12
    }
    Slider {
        id: point2_x_scale
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: point1_y_scale.bottom
        anchors.topMargin: 10
        from: 0;to: 1;value: _root.pt2_x_scale ;stepSize: 0.01
        width: parent.width/2
        onValueChanged: {
            _root.pt2_x_scale = value
            _root.isCrop = false
            cvController.crop( _root.pt1_x_scale ,   _root.pt1_y_scale , _root.pt2_x_scale ,
                                _root.pt2_y_scale, _root.isCrop)
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: point2_x_scale.verticalCenter
        anchors.right: point2_x_scale.left
        anchors.rightMargin: 5
        text: "Point 2-x: "
        font.pointSize: 12
    }
    Slider {
        id: point2_y_scale
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: point2_x_scale.bottom
        anchors.topMargin: 10
        from: 0;to: 1;value: _root.pt2_y_scale ;stepSize: 0.01
        width: parent.width/2
        onValueChanged: {
            _root.pt2_y_scale = value
            _root.isCrop = false
            cvController.crop( _root.pt1_x_scale ,   _root.pt1_y_scale , _root.pt2_x_scale ,
                                _root.pt2_y_scale, _root.isCrop)
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: point2_y_scale.verticalCenter
        anchors.right: point2_y_scale.left
        anchors.rightMargin: 5
        text: "Point 2-y: "
        font.pointSize: 12
    }

    Button{
        id: cropBt
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: point2_y_scale.bottom
        anchors.topMargin: 20
        width: parent.width/8
        height: width/3
        text: "Crop"
        onClicked: {
            _root.isCrop = true
            cvController.crop( _root.pt1_x_scale ,   _root.pt1_y_scale , _root.pt2_x_scale ,
                                _root.pt2_y_scale, _root.isCrop)
            mainWindow.showResult()
        }
    }
    Label{
        id: note
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: cropBt.bottom
        anchors.topMargin: 20
        color: "#2f4f4f"
        text: " "
        font.italic: true
        font.bold: true
        font.pixelSize: 14
    }
}
