import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    id: _root
    color: "transparent"
    property int selectIndex: -1
    Text{
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        text: "Flip Image"
        font.pixelSize: 16
    }
    Column{
        id: btColumn
        anchors.top: label.bottom
        anchors.topMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10
        Repeater{
            model: [{name: "Horizontal"}, {name: "Vertical"}, {name: "Both"}]
            delegate: Button{
                width: _root.width/5
                height: _root.height/10
                text: modelData.name
                background: Rectangle{
                    radius: 8
                    color: (_root.selectIndex === index) ? "#44aaff" : "white"
                    border.color: "#222"
                    border.width: 1
                }
                onClicked: {
                    _root.selectIndex = index
                    cvController.re_map(index)
                    mainWindow.showResult()
                }
            }
        }
    }
}




