import QtQuick

Rectangle{
    id: _tools
    color: systemStyle.tooltripColor
    property alias currentState: _tools.state
    Loader{
        id: contentLoader
        anchors.fill: parent
        source: "FileTools.qml"
    }
    states: [
        State {
            name: "file"
            PropertyChanges { target: contentLoader; source: "FileTools.qml" }
        },
        State {
            name: "basic"
            PropertyChanges { target: contentLoader; source: "BasicTools.qml" }
        },
        State {
            name: "transformations"
            PropertyChanges { target: contentLoader; source: "TransformationsTools.qml" }
        },
        State {
            name: "histograms"
            PropertyChanges { target: contentLoader; source: "HistogramsTools.qml" }
        },
        State {
            name: "contours"
            PropertyChanges { target: contentLoader; source: "ContoursTools.qml" }
        },
        State {
            name: "functions"
            PropertyChanges { target: contentLoader; source: "FunctionsTools.qml" }
        }
    ]
}
