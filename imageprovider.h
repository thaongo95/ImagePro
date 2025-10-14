#pragma once
#include <QQuickImageProvider>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/features2d.hpp>
#include <vector>

class CvImageProvider : public QQuickImageProvider {
public:
    CvImageProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

    void updateQImage(const cv::Mat& image);

private:
    QImage m_image;
};
