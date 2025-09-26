import QtQuick
import QtQuick.Controls
Flickable{
    id: _fileTools
    Column{
        id: fileToolsColumn
        anchors.fill: parent
        Repeater{
            model: [{label: "Open Image", source: "qrc:/assets/images/home.png"},
                {label: "Open Video", source: "qrc:/assets/images/road.png"},
                {label: "Save Image", source: "qrc:/assets/images/letter-r.png"},
                {label: "Open Video", source: "qrc:/assets/images/road.png"},
                {label: "Save Image", source: "qrc:/assets/images/letter-r.png"},
                {label: "Open Video", source: "qrc:/assets/images/road.png"},
                {label: "Create Image", source: "qrc:/assets/images/letter-p.png"},
                {label: "Save Image", source: "qrc:/assets/images/letter-r.png"},
                {label: "Save Video", source: "qrc:/assets/images/letter-y.png"}]
            delegate: Rectangle{
                width: parent.width
                height: parent.width*1.5
                Rectangle{
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.verticalCenter
                    Text {
                        anchors.centerIn: parent
                        text: modelData.label
                        font.pixelSize: 24
                        color: "white"

                    }
                }
                Rectangle{
                    anchors.top: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    Image{
                        source: modelData.source
                        anchors.fill: parent
                    }
                }
            }
        }

    }
}
