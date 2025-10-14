import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Dialogs

Rectangle {
    id: _root
    property int matchType: 0
    property string tempPath: ""
    property int thresh: 1000
    Button {
        id: createBt
        anchors.top: parent.top
        anchors.right: typeSelect.horizontalCenter
        anchors.margins: 10
        text: "Load TemPlate"
        onClicked: fileDialog.open()
    }
    Button {
        id: result
        anchors.top: parent.top
        anchors.left: typeSelect.horizontalCenter
        anchors.margins: 10
        text: "Result"
        onClicked: {
            cvController.featuresDetecting(_root.matchType, _root.tempPath, _root.thresh)
            mainWindow.showResult()
        }
    }
    Column {
        id: typeSelect
        anchors.left: parent.left
        anchors.top: createBt.bottom
        anchors.margins: 20
        width: parent.width/3
        height: parent.height*2/3
        spacing: 10
        Repeater{
            model: [{name: "SIFT"},{name: "ORB"},{name: "AKAZE"}]
            delegate: Button{
                text: modelData.name
                font.pixelSize: 12
                background: Rectangle {
                            implicitWidth: 80
                            implicitHeight: 40
                            radius: 8
                            color: index === _root.matchType ? "#44aaff" : "white"
                            border.color: "#222"
                            border.width: 1
                        }
                height: (parent.height-20)/3
                width: parent.width
                onClicked:{
                    _root.matchType = index
                }
            }
        }
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

            _root.tempPath = localPath
            template.source = ""
            template.source = fileDialog.selectedFile
        }
    }
    Slider {
        id: _thresh
        anchors.right: parent.horizontalCenter
        anchors.top: typeSelect.bottom
        anchors.topMargin: 10
        from: 100;to: 3000;value: 1000;stepSize: 10
        width: parent.width/3
        onValueChanged: {
            _root.thresh = value
        }
    }
    Text {
        anchors.verticalCenter: _thresh.verticalCenter
        anchors.right: _thresh.left
        anchors.rightMargin: 5
        text: "Threshold: "
        font.pointSize: 12
    }
    Text {
        anchors.verticalCenter: _thresh.verticalCenter
        anchors.left: _thresh.right
        anchors.leftMargin: 5
        text: _thresh.value.toFixed(0)
        font.pointSize: 12
    }
    Image{
        id: template
        source: ""
        cache: false
        fillMode: Image.PreserveAspectFit
        anchors.right: parent.right
        anchors.left: parent.horizontalCenter
        width: parent.width-10
        height: width*implicitHeight/implicitWidth
    }
}
