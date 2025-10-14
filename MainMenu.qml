import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
Rectangle{
    id: _root
    color: "#8fbc8f"
    property int selectedIndex: 0
    Row{
        anchors.fill: parent
        spacing: 10
        Repeater{
            model: [{name: "Basic", state: 1},
                {name: "Transformations", state: 2},
                {name: "2D Features", state: 3},
                {name: "Contours", state: 4},
                {name: "Functions", state: 5},
                {name: "Advanced", state: 6}]
            delegate: Rectangle{
                height: parent.height
                width: _text.implicitWidth+10
                color: _root.color
                Text {
                    id: _text
                    text: modelData.name
                    anchors.centerIn: parent
                    font.pixelSize: 18
                    color: index === selectedIndex ? "black" : "white"
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked:{
                        mainWindow.getParamState(modelData.state)
                        selectedIndex = index
                    }
                }
            }
        }
    }
}
