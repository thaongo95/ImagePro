import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import QtQuick.Window 2.15
ApplicationWindow {
    id: mainWindow
    width: 1920
    height: 1080
    visible: true
    title: qsTr("IMAGE PRO")
    function getParamState(value){
        if (value===1) _toolbar.state = "basic"
        else if (value===2) _toolbar.state = "transformations"
        else if (value===3) _toolbar.state = "histograms"
        else if (value===4) _toolbar.state = "contours"
        else if (value===5) _toolbar.state = "functions"
        else _toolbar.state = "advanced"
    }
    MainMenu{
        id: _menu
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.height/30
    }
    MainToolBar{
        id: _toolbar
        anchors.top: _menu.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        height: parent.height/10
    }
    function getToolsInput(value){
        contentLoader.source = value
    }
    LeftView{
        id: _leftview
        anchors.left: parent.left
        anchors.top: _toolbar.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.horizontalCenter
        anchors.margins: 10
        Rectangle{
            id: paramField
            anchors.top: loadBt.bottom
            anchors.bottom: parent.verticalCenter
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.margins: 5
            radius: height/10
            color: Qt.lighter(_leftview.color, 1.1)
            Loader{
                id: contentLoader
                anchors.fill: parent
                //source: "transformations/Canny.qml"
            }
        }
        Button {
            id: loadBt
            anchors.top: parent.top
            anchors.right: createBt.left
            anchors.margins: 10
            text: "Load Image..."
            onClicked: fileDialog.open()
        }
        Button {
            id: createBt
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.margins: 10
            text: "Create Image..."
            onClicked: createImg.open()
        }
        Button {
            id: saveBt
            anchors.top: parent.top
            anchors.left: createBt.right
            anchors.margins: 10
            text: "Save Image..."
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

                cvController.loadImage(localPath)
                originImg.source = ""
                originImg.source = fileDialog.selectedFile

                result.source = ""
                result.source = "image://cv/any"
            }
        }
        Popup{
            id: createImg
            anchors.centerIn: parent
            width: parent.width/2
            height: parent.height/3
            modal: true
            focus: true
            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
            Rectangle{
                anchors.fill: parent
                anchors.margins: 10
                color: Qt.lighter("lightblue", 1.2)
                Label {
                    anchors.top: parent.top
                    anchors.topMargin: 2
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Create RGB IMAGE"
                    font.pixelSize: 16
                }
                GridLayout {
                    anchors.centerIn: parent
                    columns: 2                // 2 columns: label + input
                    rowSpacing: 10
                    columnSpacing: 20
                    Label {
                        text: "Width:"
                        font.pixelSize: 16
                    }
                    TextField {
                        id: widthInput
                        width: 100
                        height: 30
                        font.pixelSize: 16
                        color: "black"
                        inputMethodHints: Qt.ImhDigitsOnly   // only numbers
                        placeholderText: "Enter width"
                        background: Rectangle {
                                radius: 6
                                border.color: "gray"
                                color: "white"
                            }
                    }
                    Label {
                        text: "Height:"
                        font.pixelSize: 16
                    }
                    TextField {
                        id: heightInput
                        width: 100
                        height: 30
                        font.pixelSize: 16
                        color: "black"
                        inputMethodHints: Qt.ImhDigitsOnly
                        placeholderText: "Enter height"
                        background: Rectangle {
                                radius: 6
                                border.color: "gray"
                                color: "white"
                            }
                    }
                    Label { text: "Red:" }
                    Slider {
                        id: redSlider
                        from: 0; to: 255; value: 128
                        stepSize: 1
                    }

                    Label { text: "Green:" }
                    Slider {
                        id: greenSlider
                        from: 0; to: 255; value: 128
                        stepSize: 1
                    }

                    Label { text: "Blue:" }
                    Slider {
                        id: blueSlider
                        from: 0; to: 255; value: 128
                        stepSize: 1
                    }
                }
                Rectangle{
                    id: runCreate
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.margins: 5
                    width: createLabel.implicitWidth*1.2
                    height: createLabel.implicitHeight*1.2
                    color: Qt.darker("lightblue", 1.2)
                    Label {
                        id: createLabel
                        text: "Create"
                        font.pixelSize: 16
                        anchors.centerIn: parent
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked:{
                            cvController.createImage(parseInt(widthInput.text),
                                                     parseInt(heightInput.text),
                                                     redSlider.value,
                                                     greenSlider.value,
                                                     blueSlider.value)
                            showResult()
                            createImg.close()
                        }
                    }
                }
                Rectangle{
                    id: quitCreate
                    anchors.right: runCreate.left
                    anchors.bottom: parent.bottom
                    anchors.margins: 5
                    width: quitLabel.implicitWidth*1.2
                    height: quitLabel.implicitHeight*1.2
                    color: Qt.darker("lightblue", 1.2)
                    Label {
                        id: quitLabel
                        text: "Quit"
                        font.pixelSize: 16
                        anchors.centerIn: parent
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked:{createImg.close()}
                    }
                }
            }
        }
        Image {
            id: originImg
            source: ""
            cache: false
            fillMode: Image.PreserveAspectFit
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height/2
            width: height*implicitWidth/implicitHeight

        }
    }
    function showResult(){
        result.source = ""
        result.source = "image://cv/any"
    }
    RightView{
        id: _rightview
        anchors.left: parent.horizontalCenter
        anchors.top: _toolbar.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 10
        Label{
            id: mousePos
            font.pixelSize: 16
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 2
            color: "black"
        }
        Label{
            id: mousePosIntensity
            font.pixelSize: 16
            anchors.top: parent.top
            anchors.left: mousePos.right
            anchors.margins: 2
            color: "black"
        }
        Image {
            id: result
            source: ""
            cache: false
            //fillMode: Image.PreserveAspectFit
            anchors.centerIn: parent
            width: implicitWidth>_rightview.width ? _rightview.width : implicitWidth
            height: width*implicitHeight/implicitWidth
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onPositionChanged: function(mouse){
                    let x = mouse.x
                    let y = mouse.y
                    mousePos.text = "(" + x.toFixed(0) + "," + y.toFixed(0)+")"
                    // console.log("Intensity:", cvController.getIntensity(x, y))
                    mousePosIntensity.text = "(" + cvController.getIntensity(x, y) + ")"
                }
            }
        }
    }

}
