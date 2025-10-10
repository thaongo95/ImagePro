import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: _root
    color: "transparent"
    property real pt1_x_scale: 0.0
    property real pt1_y_scale: 0.35
    property real pt2_x_scale: 0.85
    property real pt2_y_scale: 0.25
    property real pt3_x_scale: 0.25
    property real pt3_y_scale: 0.7
    Text{
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "Shearing"
        font.pixelSize: 16
    }
    Slider {
        id: point1_x_scale
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: label.bottom
        anchors.topMargin: 10
        from: 0;to: 1;value: _root.pt1_x_scale ;stepSize: 0.05
        width: parent.width/2
        onValueChanged: {
            _root.pt1_x_scale = value
            cvController.affine( _root.pt1_x_scale ,   _root.pt1_y_scale , _root.pt2_x_scale ,
                                _root.pt2_y_scale , _root.pt3_x_scale , _root.pt3_y_scale )
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: point1_x_scale.verticalCenter
        anchors.right: point1_x_scale.left
        anchors.rightMargin: 5
        text: "Point 1-x: " + point1_x_scale.value.toFixed(2)
        font.pointSize: 12
    }
    Slider {
        id: point1_y_scale
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: point1_x_scale.bottom
        anchors.topMargin: 10
        from: 0;to: 1;value: _root.pt1_y_scale ;stepSize: 0.05
        width: parent.width/2
        onValueChanged: {
            _root.pt1_y_scale = value
            cvController.affine( _root.pt1_x_scale ,   _root.pt1_y_scale , _root.pt2_x_scale ,
                                _root.pt2_y_scale , _root.pt3_x_scale , _root.pt3_y_scale )
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: point1_y_scale.verticalCenter
        anchors.right: point1_y_scale.left
        anchors.rightMargin: 5
        text: "Point 1-y: " + point1_y_scale.value.toFixed(2)
        font.pointSize: 12
    }
    Slider {
        id: point2_x_scale
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: point1_y_scale.bottom
        anchors.topMargin: 10
        from: 0;to: 1;value: _root.pt2_x_scale ;stepSize: 0.05
        width: parent.width/2
        onValueChanged: {
            _root.pt2_x_scale = value
            cvController.affine( _root.pt1_x_scale ,   _root.pt1_y_scale , _root.pt2_x_scale ,
                                _root.pt2_y_scale , _root.pt3_x_scale , _root.pt3_y_scale )
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: point2_x_scale.verticalCenter
        anchors.right: point2_x_scale.left
        anchors.rightMargin: 5
        text: "Point 2-x: " + point2_x_scale.value.toFixed(2)
        font.pointSize: 12
    }
    Slider {
        id: point2_y_scale
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: point2_x_scale.bottom
        anchors.topMargin: 10
        from: 0;to: 1;value: _root.pt2_y_scale ;stepSize: 0.05
        width: parent.width/2
        onValueChanged: {
            _root.pt2_y_scale = value
            cvController.affine( _root.pt1_x_scale ,   _root.pt1_y_scale , _root.pt2_x_scale ,
                                _root.pt2_y_scale , _root.pt3_x_scale , _root.pt3_y_scale )
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: point2_y_scale.verticalCenter
        anchors.right: point2_y_scale.left
        anchors.rightMargin: 5
        text: "Point 2-y: " + point2_y_scale.value.toFixed(2)
        font.pointSize: 12
    }
    Slider {
        id: point3_x_scale
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: point2_y_scale.bottom
        anchors.topMargin: 10
        from: 0;to: 1;value: _root.pt3_x_scale ;stepSize: 0.05
        width: parent.width/2
        onValueChanged: {
            _root.pt3_x_scale = value
            cvController.affine( _root.pt1_x_scale ,   _root.pt1_y_scale , _root.pt2_x_scale ,
                                _root.pt2_y_scale , _root.pt3_x_scale , _root.pt3_y_scale )
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: point3_x_scale.verticalCenter
        anchors.right: point3_x_scale.left
        anchors.rightMargin: 5
        text: "Point 3-x: " + point3_x_scale.value.toFixed(2)
        font.pointSize: 12
    }
    Slider {
        id: point3_y_scale
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: point3_x_scale.bottom
        anchors.topMargin: 10
        from: 0;to: 1;value: _root.pt3_y_scale ;stepSize: 0.05
        width: parent.width/2
        onValueChanged: {
            _root.pt3_y_scale = value
            cvController.affine( _root.pt1_x_scale ,   _root.pt1_y_scale , _root.pt2_x_scale ,
                                _root.pt2_y_scale , _root.pt3_x_scale , _root.pt3_y_scale )
            mainWindow.showResult()
        }
    }
    Text {
        anchors.verticalCenter: point3_y_scale.verticalCenter
        anchors.right: point3_y_scale.left
        anchors.rightMargin: 5
        text: "Point 3-y: " + point3_y_scale.value.toFixed(2)
        font.pointSize: 12
    }
    Label{
        id: note
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: point3_y_scale.bottom
        anchors.topMargin: 20
        color: "#2f4f4f"
        text: " "
        font.italic: true
        font.bold: true
        font.pixelSize: 14
    }
}
