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
    original = cv::imread(filePath.toStdString(), cv::IMREAD_COLOR);
    if (original.empty()) {
        qWarning() << "Failed to load image:" << filePath;
        return;
    }

    //m_image = QImage(original.data, original.cols, original.rows, original.step, QImage::Format_RGB888).copy();
    //scaleImage(1.0); // initial scale
}

void CvImageProvider::createImage(int width, int height, int red, int green, int blue)
{
    cv::Mat whiteImage(height, width, CV_8UC3, cv::Scalar(red, green, blue));
    original = whiteImage;
    m_image = QImage(whiteImage.data, whiteImage.cols, whiteImage.rows, whiteImage.step, QImage::Format_Grayscale8).copy();

}
void CvImageProvider::cannyDetect(double low_threshold, double high_threshold) {
    cv::Mat gray;
    cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5,5), 1.5);
    cv::Canny(gray, gray, low_threshold, high_threshold);
    m_image = QImage(gray.data, gray.cols, gray.rows, gray.step, QImage::Format_Grayscale8).copy();
    //scaleImage(1.0); // initial scale
}
void CvImageProvider::scaleImage(double factor) {
    if (original.empty()) return;

    int newWidth = static_cast<int>(original.cols * factor);
    int newHeight = static_cast<int>(original.rows * factor);

    cv::Mat resized;
    cv::resize(original, resized, cv::Size(newWidth, newHeight));
    cv::cvtColor(resized, resized, cv::COLOR_BGR2RGB);

    m_image = QImage(resized.data, resized.cols, resized.rows, resized.step, QImage::Format_RGB888).copy();
}
