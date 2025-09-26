import QtQuick

Rectangle{
    id: _tools
    property string toolsColor: systemStyle.tooltripColor
    color: Qt.darker("#800000", 1.5)
    property alias currentState: _tools.state
    Loader{
        id: contentLoader
        anchors.fill: parent
        source: "BasicTools.qml"
    }
    states: [
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
        },
        State {
            name: "advanced"
            PropertyChanges { target: contentLoader; source: "AdvancedTools.qml" }
        }
    ]
}
