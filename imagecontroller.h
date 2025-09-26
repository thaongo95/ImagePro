#pragma once
#include <QObject>
#include "imageprovider.h"

class ImageController : public QObject {
    Q_OBJECT
public:
    explicit ImageController(CvImageProvider* provider, QObject* parent = nullptr);

    Q_INVOKABLE void loadImage(const QString &filePath);
    Q_INVOKABLE void createImage(int width, int height, int red, int green, int blue);
    Q_INVOKABLE void scaleImage(double factor);
    Q_INVOKABLE void cannyDetect(double low_threshold, double high_threshold);

private:
    CvImageProvider* m_provider;
};
