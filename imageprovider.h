#pragma once
#include <QQuickImageProvider>
#include <opencv2/opencv.hpp>

class CvImageProvider : public QQuickImageProvider {
public:
    CvImageProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

    Q_INVOKABLE void loadImage(const QString &filePath);
    Q_INVOKABLE void createImage(int width, int height, int red, int green, int blue);
    Q_INVOKABLE void scaleImage(double factor);
    Q_INVOKABLE void cannyDetect(double low_threshold, double high_threshold);

private:
    cv::Mat original;
    QImage m_image;

};
