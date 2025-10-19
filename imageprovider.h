#pragma once
#include <QQuickImageProvider>
#include <opencv2/opencv.hpp>

class CvImageProvider : public QQuickImageProvider {
public:
    CvImageProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

    void updateQImage(const cv::Mat& image);

private:
    QImage m_image;
};
