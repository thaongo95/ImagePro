#pragma once
#include <QObject>
#include "imageprovider.h"

class ImageController : public QObject {
    Q_OBJECT
public:
    explicit ImageController(CvImageProvider* provider, QObject* parent = nullptr);

    Q_INVOKABLE void loadImage(const QString &filePath);
    Q_INVOKABLE QString getIntensity(int x,  int y);
    Q_INVOKABLE void createImage(int width, int height, int red, int green, int blue);
    Q_INVOKABLE void saveImage(const QString &filePath);
    Q_INVOKABLE void scaleImage(double factor);


    Q_INVOKABLE void basicExposure(const double& alpha, const int& beta);
    Q_INVOKABLE void gammaCorrect(const double& gammaValue);
    Q_INVOKABLE void smoothing(const int& blurType, const int& size);
    Q_INVOKABLE void erosion(const int& type, const int& size);
    Q_INVOKABLE void dilation(const int& type, const int& size);
    Q_INVOKABLE void morphology(const int& morph_operator, const int& morph_type, const int& morph_size);
    Q_INVOKABLE void convertImg(const int& index);
    Q_INVOKABLE void extractLines(const int& type, const double& size);
    Q_INVOKABLE void basicThreshold(const int& threType, const int& threValue);
    Q_INVOKABLE void inrangeHSV(const int& h_low, const int& h_high, const int& s_low, const int& s_high, const int& v_low, const int& v_high);
    Q_INVOKABLE void enumColorDetect(const int& colorType, const double& thre_erea);
    Q_INVOKABLE void customColorDetect(const double& thre_erea, const int& h_low, const int& h_high, const int& s_low, const int& s_high, const int& v_low, const int& v_high);
    Q_INVOKABLE void his_equalization(const int& imgType, const float& scale);
    Q_INVOKABLE void tempMatching(const int& matchingType, const QString &tempPath);

    Q_INVOKABLE void filter_2d(const int& kernelType);
    Q_INVOKABLE void sobel(const int& ksize, const double& scale, const int& delta);
    Q_INVOKABLE void laplace(const int& ksize, const double& scale, const int& delta);
    Q_INVOKABLE void cannyDetect(const double& low_threshold, const double& high_threshold, const int& ksize, const bool& l2);
    Q_INVOKABLE void houghline(const int& thres);
    Q_INVOKABLE void houghline_p(const int& thres, const double& minLength, const int& maxGap);
    Q_INVOKABLE void hough_circle(const int& minRadius, const int& maxRadius, const double& distance);
    Q_INVOKABLE void re_map(const int& type);
    Q_INVOKABLE void rotation(const int& angle, const float& scale);
    Q_INVOKABLE void affine(const double& p1x, const double& p1y, const double& p2x, const double& p2y, const double& p3x, const double& p3y);
    Q_INVOKABLE void guil_ballard(const int& matchingType, const QString &tempPath);

    Q_INVOKABLE void harris_corner(const int& thresh);
    Q_INVOKABLE void shitomasi(const int& maxc_corners);
    Q_INVOKABLE void custom(const int& type, const int& level);
    Q_INVOKABLE QString subpix(const int& max_corners);
    Q_INVOKABLE void siftDetection(const int& nfeatures, const double& contrastThre);
    Q_INVOKABLE void orbDetection(const int& nfeatures, const double& fastThre);

    Q_INVOKABLE void drawEgde(const int& cannyThres, const int& colorType);
    Q_INVOKABLE void convexHull(const int& cannyThres, const int& colorType);
    Q_INVOKABLE void drawShape(const int& cannyThres, const int& colorType, const int& shapeID);
    Q_INVOKABLE void moment(const int& cannyThres, const int& colorType);
    Q_INVOKABLE void point_test(const int& cannyThres, const int& contour);

    Q_INVOKABLE QString showInfo();


private:
    CvImageProvider* m_provider;
};


