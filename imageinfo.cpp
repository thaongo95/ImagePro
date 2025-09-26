#include "imageinfo.h"

ImageInfo::ImageInfo(QObject *parent)
    : QObject{parent}
{}

void
ImageInfo::setHeight(int value){
    if (m_height==value) return;
    m_height = value;
    emit heightChanged();
}
void
ImageInfo::setWidth(int value){
    if (m_width==value) return;
    m_width = value;
    emit widthChanged();
}

void ImageInfo::setImageUrl(QString value)
{
    if (m_url==value) return;
    m_url = value;
    emit imageUrlChanged();
}
