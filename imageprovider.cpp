#include "imageprovider.h"
#include "image_function.h"
#include <QDebug>
#include "imageinfo.h"

CvImageProvider::CvImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image) {}

QImage CvImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
    if (size) *size = m_image.size();
    return m_image;
}

void CvImageProvider::loadImage(const QString &filePath) {
    current = cv::imread(filePath.toStdString(), cv::IMREAD_COLOR);
    if (current.empty()) {
        qWarning() << "Failed to load image:" << filePath;
        return;
    }
    updateQImage();

    //scaleImage(1.0); // initial scale
}

QString CvImageProvider::getIntensity(const int &x, const int &y)
{
    if (current.empty()) {return "";}
    if (x < 0 || y < 0 || x >= current.cols || y >= current.rows) {return "";}
    QString result;
    if (current.channels()==1){
        int val = current.at<uchar>(y,x);
        result = QString("Gray: %1").arg(val);
    }
    else if(current.channels()==3){
        cv::Vec3b intensity = current.at<cv::Vec3b>(y, x);
        int blue = intensity[0];
        int green = intensity[1];
        int red = intensity[2];
        result = QString("R:%1 G:%2 B:%3 ")
                     .arg(red).arg(green).arg(blue);
    }
    return result;

}

void CvImageProvider::createImage(int width, int height, int red, int green, int blue)
{
    current = cv::Mat(height, width, CV_8UC3, cv::Scalar(red, green, blue));
    m_image = QImage(current.data, current.cols, current.rows, current.step, QImage::Format_RGB888).copy();
}
void CvImageProvider::cannyDetect(double low_threshold, double high_threshold) {
    if (current.empty()) {return;}
    cv::Mat gray;
    cv::cvtColor(current, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5,5), 1.5);
    cv::Canny(gray, gray, low_threshold, high_threshold);
    //current = gray.clone();
    m_image = QImage(gray.data, gray.cols, gray.rows, gray.step, QImage::Format_Grayscale8).copy();
    //updateQImage();
    //scaleImage(1.0); // initial scale
}

void CvImageProvider::updateQImage()
{
    if (current.channels()==1){
        m_image = QImage(current.data, current.cols, current.rows, current.step, QImage::Format_Grayscale8).copy();
    }
    else if(current.channels()==3){
        cv::Mat tempImg;
        cv::cvtColor(current, tempImg, cv::COLOR_BGR2RGB);
        m_image = QImage(tempImg.data, tempImg.cols, tempImg.rows, tempImg.step, QImage::Format_RGB888).copy();
    }
    else return;
}
void CvImageProvider::scaleImage(double factor) {
    if (current.empty()) return;

    int newWidth = static_cast<int>(current.cols * factor);
    int newHeight = static_cast<int>(current.rows * factor);

    cv::Mat resized;
    cv::resize(current, resized, cv::Size(newWidth, newHeight));
    cv::cvtColor(resized, resized, cv::COLOR_BGR2RGB);

    m_image = QImage(resized.data, resized.cols, resized.rows, resized.step, QImage::Format_RGB888).copy();
}
