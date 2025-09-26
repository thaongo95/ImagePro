import QtQuick
import QtQuick.Controls
Item{
    id: _basicTools
    ListView{
        id: basicToolsColumn
        anchors.fill: parent
        orientation: ListView.Horizontal
        spacing: 10
        clip: true
        model: [{label: "Doc-Scan", source: "assets/images/home.png"},
        {label: "S-estimate", source: "assets/images/letter-p.png"},
        {label: "Snitching", source: "assets/images/letter-p.png"},]
        delegate: Rectangle{
            height: parent.height*2/3
            anchors.verticalCenter: parent.verticalCenter
            width: parent.height
            radius: width/8
            color: "transparent"
            Image{
                id: _icon
                anchors.top: parent.top
                anchors.topMargin: parent.height/10
                anchors.horizontalCenter: parent.horizontalCenter
                height: parent.height*2/3
                width: height
                source: modelData.source
            }
            Text{
                anchors.horizontalCenter: _icon.horizontalCenter
                anchors.top: _icon.bottom
                anchors.topMargin: 2
                text: modelData.label
                font.pixelSize: 14
                color: modelData.color
            }
        }
    }
}
