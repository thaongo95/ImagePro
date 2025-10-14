import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Rectangle {
    id: _root
    color: "transparent"
    property string iconPath: ""
    property int red: 255
    property int green: 255
    property int blue: 255
    Button {
        id: createBt
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: parent.width/4
        text: "Load Icon"
        onClicked: fileDialog.open()
    }
    FileDialog {
        id: fileDialog
        title: "Choose an image"
        nameFilters: ["Images (*.png *.jpg *.jpeg *.bmp)"]
        onAccepted: {
            //loadBt.visible = false
            console.log("Selected:", selectedFile)
            var localPath = selectedFile.toString().replace("file://", "")
            //console.log("Local:", localPath)

            _root.iconPath = localPath
            _icon.source = ""
            _icon.source = fileDialog.selectedFile
        }
    }

    Image{
        id: _icon
        source: ""
        cache: false
        fillMode: Image.PreserveAspectFit
        anchors.right: parent.right
        anchors.left: parent.horizontalCenter
        anchors.margins: parent.width/5
        height: width*implicitHeight/implicitWidth
    }
    GridLayout {
        anchors.centerIn: parent
        columns: 2                // 2 columns: label + input
        rowSpacing: 10
        columnSpacing: 20
        Label { text: "Red:" }
        Slider {
            id: redSlider
            from: 0; to: 255; value: 128
            stepSize: 1
            onValueChanged: {
                _root.red = value
                cvController.iconColor(_root.iconPath, _root.blue, _root.green, _root.red)
                mainWindow.showResult()
            }
        }
        Label { text: "Green:" }
        Slider {
            id: greenSlider
            from: 0; to: 255; value: 128
            stepSize: 1
            onValueChanged: {
                _root.green = value
                cvController.iconColor(_root.iconPath, _root.blue, _root.green, _root.red)
                mainWindow.showResult()
            }
        }

        Label { text: "Blue:" }
        Slider {
            id: blueSlider
            from: 0; to: 255; value: 128
            stepSize: 1
            onValueChanged: {
                _root.blue = value
                cvController.iconColor(_root.iconPath, _root.blue, _root.green, _root.red)
                mainWindow.showResult()
            }
        }
    }

}
