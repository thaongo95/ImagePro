import QtQuick

Rectangle {
    property string _color: systemStyle.tooltripColor
    color: Qt.lighter(_color, 1.2)

}
