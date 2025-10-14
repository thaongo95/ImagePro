#include "imageprovider.h"

#include <QDebug>
#include <QFileInfo>


CvImageProvider::CvImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image) {}

QImage CvImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
    if (size) *size = m_image.size();
    return m_image;
}



void CvImageProvider::updateQImage(const cv::Mat& image)
{
    if (image.channels()==1){
        m_image = QImage(image.data, image.cols, image.rows, image.step, QImage::Format_Grayscale8).copy();
    }
    else if(image.channels()==3){
        cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
        m_image = QImage(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888).copy();
    }
    else if(image.channels()==4){
        m_image = QImage(image.data, image.cols, image.rows, image.step, QImage::Format_ARGB32).copy();
    }
    else return;
}


