#include "imagecontroller.h"

ImageController::ImageController(CvImageProvider* provider, QObject* parent)
    : QObject(parent), m_provider(provider) {}

void ImageController::loadImage(const QString &filePath) {
    if (m_provider)
        m_provider->loadImage(filePath);
}

QString ImageController::getIntensity(int x, int y)
{
    QString result;
    if (m_provider)
        result = m_provider->getIntensity(x,y);
    return result;
}

void ImageController::createImage(int width, int height, int red, int green, int blue)
{
    if (m_provider)
        m_provider->createImage(width, height, red, green, blue);
}

void ImageController::scaleImage(double factor) {
    if (m_provider)
        m_provider->scaleImage(factor);
}

void ImageController::cannyDetect(double low_threshold, double high_threshold) {
    if (m_provider)
        m_provider->cannyDetect(low_threshold, high_threshold);
}
