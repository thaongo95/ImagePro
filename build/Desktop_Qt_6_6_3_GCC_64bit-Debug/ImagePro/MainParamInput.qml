import QtQuick

Rectangle{
    id: _paramInput
    property string paramColor: systemStyle.tooltripColor
    color: Qt.lighter(paramColor, 1.2)
    property alias currentState: _paramInput.state
    Loader{
        id: contentLoader
        anchors.fill: parent
    }
    states: [
        State {
            name: "file"
            PropertyChanges { target: contentLoader; source: "FileParam.qml" }
        },
        State {
            name: "tools"
            PropertyChanges { target: contentLoader; source: "ToolsParam.qml" }
        },
        State {
            name: "image"
            PropertyChanges { target: contentLoader; source: "ImageParam.qml" }
        },
        State {
            name: "video"
            PropertyChanges { target: contentLoader; source: "VideoParam.qml" }
        },
        State {
            name: "function"
            PropertyChanges { target: contentLoader; source: "FunctionParam.qml" }
        }
    ]
}
