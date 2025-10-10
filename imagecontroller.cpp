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

void ImageController::saveImage(const QString &filePath)
{
    if (m_provider)
        m_provider->saveImage(filePath);
}

void ImageController::scaleImage(double factor) {
    if (m_provider)
        m_provider->scaleImage(factor);
}

void ImageController::basicExposure(const double &alpha, const int &beta)
{
    if (m_provider)
        m_provider->basicExposure(alpha, beta);
}

void ImageController::gammaCorrect(const double &gammaValue)
{
    if (m_provider)
        m_provider->gammaCorrect(gammaValue);
}

void ImageController::smoothing(const int &blurType, const int& size)
{
    if (m_provider)
        m_provider->smoothing(blurType, size);
}

void ImageController::erosion(const int &type, const int &size)
{
    if (m_provider)
        m_provider->erosion(type, size);
}

void ImageController::dilation(const int &type, const int &size)
{
    if (m_provider)
        m_provider->dilation(type, size);
}

void ImageController::morphology(const int &morph_operator, const int &morph_type, const int &morph_size)
{
    if (m_provider)
        m_provider->morphology(morph_operator, morph_type, morph_size);
}

void ImageController::convertImg(const int &index)
{
    if (m_provider)
        m_provider->convertImg(index);
}

void ImageController::extractLines(const int &type, const double &size)
{
    if (m_provider)
        m_provider->extractLines(type, size);
}

void ImageController::basicThreshold(const int &threType, const int &threValue)
{
    if (m_provider)
        m_provider->extractLines(threType, threValue);
}

void ImageController::inrangeHSV(const int &h_low, const int &h_high, const int &s_low, const int &s_high, const int &v_low, const int &v_high)
{
    if (m_provider)
        m_provider->inrangeHSV(h_low, h_high, s_low, s_high, v_low, v_high);
}

void ImageController::enumColorDetect(const int &colorType, const double &thre_erea)
{
    if (m_provider)
        m_provider->enumColorDetect(colorType, thre_erea);
}

void ImageController::customColorDetect(const double &thre_erea, const int &h_low, const int &h_high, const int &s_low, const int &s_high, const int &v_low, const int &v_high)
{
    if (m_provider)
        m_provider->customColorDetect(thre_erea, h_low, h_high, s_low, s_high, v_low, v_high);
}

void ImageController::filter_2d(const int &kernelType)
{
    if (m_provider)
        m_provider->filter_2d(kernelType);
}

void ImageController::sobel(const int &ksize, const double &scale, const int &delta)
{
    if (m_provider)
        m_provider->sobel(ksize, scale, delta);
}

void ImageController::laplace(const int &ksize, const double &scale, const int &delta)
{
    if (m_provider)
        m_provider->laplace(ksize, scale, delta);
}

void ImageController::cannyDetect(const double &low_threshold, const double &high_threshold, const int &ksize, const bool &l2)
{
    if (m_provider)
        m_provider->cannyDetect(low_threshold, high_threshold, ksize, l2);
}

void ImageController::houghline(const int &thres)
{
    if (m_provider)
        m_provider->houghline(thres);
}

void ImageController::houghline_p(const int &thres, const double &minLength, const int &maxGap)
{
    if (m_provider)
        m_provider->houghline_p(thres, minLength, maxGap);
}

void ImageController::hough_circle(const int &minRadius, const int &maxRadius, const double &distance)
{
    if (m_provider)
        m_provider->hough_circle(minRadius, maxRadius, distance);
}

void ImageController::re_map(const int &type)
{
    if (m_provider)
        m_provider->re_map(type);
}

void ImageController::rotation(const int &angle, const float &scale)
{
    if (m_provider)
        m_provider->rotation(angle, scale);
}

void ImageController::affine(const double &p1x, const double &p1y, const double &p2x, const double &p2y, const double &p3x, const double &p3y)
{
    if (m_provider)
        m_provider->affine(p1x, p1y, p2x, p2y, p3x, p3y);
}

void ImageController::guil_ballard(const int &matchingType, const QString &tempPath)
{
    if (m_provider)
        m_provider->guil_ballard(matchingType, tempPath);
}

void ImageController::harris_corner(const int &thresh)
{
    if (m_provider)
        m_provider->harris_corner(thresh);
}

void ImageController::shitomasi(const int &maxc_corners)
{
    if (m_provider)
        m_provider->shitomasi(maxc_corners);
}

void ImageController::custom(const int &type, const int &level)
{
    if (m_provider)
        m_provider->custom(type, level);
}

QString ImageController::subpix(const int &max_corners)
{
    if (m_provider)
        return m_provider->subpix(max_corners);
    else return "";
}

void ImageController::siftDetection(const int &nfeatures, const double &contrastThre)
{
    if (m_provider)
        m_provider->siftDetection(nfeatures, contrastThre);
}

void ImageController::orbDetection(const int &nfeatures, const double &fastThre)
{
    if (m_provider)
        m_provider->orbDetection(nfeatures, fastThre);
}

void ImageController::his_equalization(const int &imgType, const float &scale)
{
    if (m_provider)
        m_provider->his_equalization(imgType, scale);
}

void ImageController::tempMatching(const int &matchingType, const QString &tempPath)
{
    if (m_provider)
        m_provider->tempMatching(matchingType, tempPath);
}

void ImageController::drawEgde(const int &cannyThres, const int &colorType)
{
    if (m_provider)
        m_provider->drawEgde(cannyThres, colorType);
}

void ImageController::convexHull(const int &cannyThres, const int &colorType)
{
    if (m_provider)
        m_provider->convexHull(cannyThres, colorType);
}

void ImageController::drawShape(const int &cannyThres, const int &colorType, const int &shapeID)
{
    if (m_provider)
        m_provider->drawShape(cannyThres, colorType, shapeID);
}

void ImageController::moment(const int &cannyThres, const int &colorType)
{
    if (m_provider)
        m_provider->moment(cannyThres, colorType);
}

void ImageController::point_test(const int &cannyThres, const int &contour)
{
    if (m_provider)
        m_provider->point_test(cannyThres, contour);
}

QString ImageController::showInfo()
{
    if (m_provider)
        return m_provider->showInfo();
    else return "";
}
