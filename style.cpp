#include "style.h"

Style::Style(QObject *parent)
    : QObject{parent}
{}
void Style::setToolbarColor(QStringList value){

    _toolBarColor = value;
    emit toolbarColorChanged();
}
void Style::setTooltripColor(QStringList value){
    _toolTripColor = value;
    emit tooltripColorChanged();
}
