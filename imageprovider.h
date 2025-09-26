#pragma once
#include <QQuickImageProvider>
#include <opencv2/opencv.hpp>
#include <vector>

class CvImageProvider : public QQuickImageProvider {
public:
    CvImageProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

    Q_INVOKABLE void loadImage(const QString &filePath);
    Q_INVOKABLE QString getIntensity(const int& x, const int& y);
    Q_INVOKABLE void createImage(int width, int height, int red, int green, int blue);
    Q_INVOKABLE void scaleImage(double factor);
    Q_INVOKABLE void cannyDetect(double low_threshold, double high_threshold);

    void updateQImage();

private:
    cv::Mat current;
    QImage m_image;
    std::vector<cv::Mat> history;
};
