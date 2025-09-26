import QtQuick
import QtQuick.Controls
Item{
    id: _fileTools
    ListView{
        id: fileToolsColumn
        anchors.fill: parent
        spacing: 10
        clip: true
        model: [{label: "Open Image", source: "assets/images/home.png"},
        {label: "Open Video", source: "assets/images/road.png"},
        {label: "Create Image", source: "assets/images/letter-p.png"},
        {label: "Save Image", source: "assets/images/letter-r.png"},
        {label: "Save Video", source: "assets/images/letter-y.png"}]
        delegate: Rectangle{
            width: parent.width
            height: parent.width
            color: "yellow"
            Rectangle{
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: parent.height*3/4
                Image{
                    source: modelData.source
                    anchors.centerIn: parent
                }

            }
            Rectangle{
                height: parent.height/4
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                Text {
                    anchors.centerIn: parent
                    text: modelData.label
                    font.pixelSize: 18
                    color: "black"
                }
            }
        }
    }
}
