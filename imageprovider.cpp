#include "imageprovider.h"
#include "image_function.h"
#include <QDebug>
#include <QFileInfo>
#include "imageinfo.h"
#include "basic_operation.h"
#include "sstream"

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
    QFileInfo fi(filePath);
    image_info = "";
    image_info += QString("Name: %1 | ").arg(fi.fileName());
    image_info += QString("Size: %1 x %2 | ").arg(original.cols).arg(original.rows);
    image_info += QString("Channels: %1 | ").arg(original.channels());
    image_info += QString("Image size: %1 bytes | ").arg(original.total() * original.elemSize());
    image_info += QString("Total pixels: %1").arg(original.total());
    updateQImage();

    //scaleImage(1.0); // initial scale
}

QString CvImageProvider::getIntensity(const int &x, const int &y)
{
    if (original.empty()) {return "";}
    if (x < 0 || y < 0 || x >= original.cols || y >= original.rows) {return "";}
    QString result;
    if (original.channels()==1){
        int val = original.at<uchar>(y,x);
        result = QString("Gray: %1").arg(val);
    }
    else if(original.channels()==3){
        cv::Vec3b intensity = original.at<cv::Vec3b>(y, x);
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
    original = cv::Mat(height, width, CV_8UC3, cv::Scalar(red, green, blue));
    m_image = QImage(original.data, original.cols, original.rows, original.step, QImage::Format_RGB888).copy();
}

void CvImageProvider::saveImage(const QString &filePath)
{
    if (current.empty()) {
        return;
    }
    cv::imwrite(filePath.toStdString(), current);
}


void CvImageProvider::updateQImage()
{
    if (original.channels()==1){
        m_image = QImage(original.data, original.cols, original.rows, original.step, QImage::Format_Grayscale8).copy();
    }
    else if(original.channels()==3){
        cv::Mat tempImg;
        cv::cvtColor(original, tempImg, cv::COLOR_BGR2RGB);
        m_image = QImage(tempImg.data, tempImg.cols, tempImg.rows, tempImg.step, QImage::Format_RGB888).copy();
    }
    else return;
}
void CvImageProvider::scaleImage(double factor) {
    if (original.empty()) return;

    int newWidth = static_cast<int>(original.cols * factor);
    int newHeight = static_cast<int>(original.rows * factor);

    cv::Mat resized;
    cv::resize(original, resized, cv::Size(newWidth, newHeight));
    current = resized.clone();
    cv::cvtColor(resized, resized, cv::COLOR_BGR2RGB);

    m_image = QImage(resized.data, resized.cols, resized.rows, resized.step, QImage::Format_RGB888).copy();
}
void CvImageProvider::basicExposure(const double &alpha, const int &beta)
{
    if (original.empty()) {
        return;
    }
    cv::Mat expoImg;
    expoImg = exposure(original, alpha, beta);


    cv::cvtColor(expoImg, expoImg, cv::COLOR_BGR2RGB);
    m_image = QImage(expoImg.data, expoImg.cols, expoImg.rows, expoImg.step, QImage::Format_RGB888).copy();
    current = expoImg.clone();
}
void CvImageProvider::gammaCorrect(const double& gammaValue)
{
    if (original.empty()) {
        return;
    }
    cv::Mat gammaImg;
    gammaImg = gammaCorrection(original, gammaValue);
    current = gammaImg.clone();
    cv::cvtColor(gammaImg, gammaImg, cv::COLOR_BGR2RGB);
    m_image = QImage(gammaImg.data, gammaImg.cols, gammaImg.rows, gammaImg.step, QImage::Format_RGB888).copy();
}

void CvImageProvider::smoothing(const int &blurType, const int& size)
{
    if (original.empty()) {
        return;
    }
    cv::Mat blurImg;
    if (blurType==0) cv::blur(original, blurImg, cv::Size(size,size));
    else if (blurType==1) cv::boxFilter(original, blurImg, -1, cv::Size(size,size), cv::Point(-1,-1), true);
    else if (blurType==2) cv::GaussianBlur(original, blurImg, cv::Size(size,size), 1.5);
    else if (blurType==3) cv::medianBlur(original, blurImg, size);
    else cv::bilateralFilter(original, blurImg, 9, 75, 75);
    cv::cvtColor(blurImg, blurImg, cv::COLOR_BGR2RGB);
    m_image = QImage(blurImg.data, blurImg.cols, blurImg.rows, blurImg.step, QImage::Format_RGB888).copy();
}

void CvImageProvider::erosion(const int &type, const int &size)
{
    if (original.empty()) {
        return;
    }
    int erosion_type = 0;
    if( type == 0 ){ erosion_type = cv::MORPH_RECT; }
    else if( type == 1 ){ erosion_type = cv::MORPH_CROSS; }
    else if( type == 2) { erosion_type = cv::MORPH_ELLIPSE; }
    cv::Mat element = cv::getStructuringElement(erosion_type, cv::Size(2*size+1, 2*size+1), cv::Point(size,size));
    cv::Mat erodeImg;
    cv::erode(original, erodeImg, element);
    current = erodeImg.clone();
    cv::cvtColor(erodeImg, erodeImg, cv::COLOR_BGR2RGB);
    m_image = QImage(erodeImg.data, erodeImg.cols, erodeImg.rows, erodeImg.step, QImage::Format_RGB888).copy();

}

void CvImageProvider::dilation(const int &type, const int &size)
{
    if (original.empty()) {
        return;
    }
    int dilation_type = 0;
    if( type == 0 ){ dilation_type = cv::MORPH_RECT; }
    else if( type == 1 ){ dilation_type = cv::MORPH_CROSS; }
    else if( type == 2) { dilation_type = cv::MORPH_ELLIPSE; }
    cv::Mat element = cv::getStructuringElement(type, cv::Size(2*size+1, 2*size+1), cv::Point(size,size));
    cv::Mat dilateImg;
    cv::dilate(original, dilateImg, element);
    current = dilateImg.clone();
    cv::cvtColor(dilateImg, dilateImg, cv::COLOR_BGR2RGB);
    m_image = QImage(dilateImg.data, dilateImg.cols, dilateImg.rows, dilateImg.step, QImage::Format_RGB888).copy();

}

void CvImageProvider::morphology(const int &morph_operator, const int &morph_type, const int &morph_size)
{
    if (original.empty()) {
        return;
    }
    int _type = 0;
    if( morph_type == 0 ){ _type = cv::MORPH_RECT; }
    else if( morph_type == 1 ){ _type = cv::MORPH_CROSS; }
    else if( morph_type == 2) { _type = cv::MORPH_ELLIPSE; }
    cv::Mat element = cv::getStructuringElement(_type, cv::Size(morph_size*2+1, morph_size*2+1), cv::Point(morph_size, morph_size));
    cv::Mat morphImg;
    cv::morphologyEx(original, morphImg, morph_operator+2, element);
    current = morphImg.clone();
    cv::cvtColor(morphImg, morphImg, cv::COLOR_BGR2RGB);
    m_image = QImage(morphImg.data, morphImg.cols, morphImg.rows, morphImg.step, QImage::Format_RGB888).copy();
}

void CvImageProvider::convertImg(const int &index)
{
    if (original.empty()) {
        return;
    }
    cv::Mat result;
    if( index == 0 ){
        cv::cvtColor(original, result, cv::COLOR_BGR2GRAY);
        cv::cvtColor(result, result, cv::COLOR_GRAY2RGB);
    }
    else if( index == 1 ){
        cv::cvtColor(original, result, cv::COLOR_BGR2HSV);
        cv::cvtColor(result, result, cv::COLOR_HSV2RGB);
    }
    else if( index == 2) {
        cv::cvtColor(original, result, cv::COLOR_BGR2Lab);
        cv::cvtColor(result, result, cv::COLOR_Lab2RGB);
    }
    else if( index == 3) {
        cv::cvtColor(original, result, cv::COLOR_BGR2YCrCb);
        cv::cvtColor(result, result, cv::COLOR_YCrCb2RGB);
    }
    current = result.clone();
    m_image = QImage(result.data, result.cols, result.rows, result.step, QImage::Format_RGB888).copy();
}

void CvImageProvider::extractLines(const int &type, const double &size)
{
    if (original.empty()) {
        return;
    }
    cv::Mat gray;
    cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
    cv::adaptiveThreshold(gray, gray, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 15, -2);
    cv::Mat result = gray.clone();
    if (type==0){
        int h_size = result.cols * size;
        cv::Mat h_structure = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(h_size, 1));
        cv::erode(result, result, h_structure);
        cv::dilate(result, result, h_structure);
    }
    else
    {
        int v_size = result.rows * size;
        cv::Mat v_structure = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, v_size));
        cv::erode(result, result, v_structure);
        cv::dilate(result, result, v_structure);
    }
    current = result.clone();
    m_image = QImage(result.data, result.cols, result.rows, result.step, QImage::Format_Grayscale8).copy();
}

void CvImageProvider::basicThreshold(const int &threType, const int &threValue)
{
    if (original.empty()) {
        return;
    }
    cv::Mat gray;
    cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
    current = gray.clone();
    cv::threshold(gray, gray, threValue, 255, threType);
    m_image = QImage(gray.data, gray.cols, gray.rows, gray.step, QImage::Format_Grayscale8).copy();
}

void CvImageProvider::inrangeHSV(const int &h_low, const int &h_high, const int &s_low, const int &s_high, const int &v_low, const int &v_high)
{
    if (original.empty()) {return;}
    cv::Mat hsvImg, result;
    cv::cvtColor(original, hsvImg, cv::COLOR_BGR2HSV);
    cv::inRange(hsvImg, cv::Scalar(h_low, s_low, v_low), cv::Scalar(h_high, s_high, v_high), result);
    current = result.clone();
    m_image = QImage(result.data, result.cols, result.rows, result.step, QImage::Format_Grayscale8).copy();
}

void CvImageProvider::enumColorDetect(const int &colorType, const double& thre_erea)
{
    if (original.empty()) {return;}
    COLOR _color;
    if (colorType ==0) _color = COLOR::RED;
    else if (colorType ==1) _color = COLOR::GREEN;
    else if (colorType ==2) _color = COLOR::BLUE;
    else if (colorType ==3) _color = COLOR::YELLOW;
    else if (colorType ==4) _color = COLOR::BLACK;
    cv::Mat result = original.clone();
    result = color_detection(original, thre_erea, _color);
    current = result.clone();
    cv::cvtColor(result, result, cv::COLOR_BGR2RGB);
    m_image = QImage(result.data, result.cols, result.rows, result.step, QImage::Format_RGB888).copy();
}

void CvImageProvider::customColorDetect(const double &thre_erea, const int &h_low, const int &h_high, const int &s_low, const int &s_high, const int &v_low, const int &v_high)
{
    if (original.empty()) {return;}
    cv::Mat mask, result;
    cv::cvtColor(original, mask, cv::COLOR_BGR2HSV);
    cv::Mat img = original.clone();
    cv::Scalar lower_(h_low, s_low, v_low);
    cv::Scalar upper_(h_high, s_high, v_high);
    cv::inRange(mask, lower_, upper_, result);

    cv::erode(result, result, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));
    cv::dilate(result, result, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(result, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    for (size_t i=0; i< contours.size(); i++){
        double erea = cv::contourArea(contours[i]);
        if (erea > thre_erea){
            cv::Rect bounding_rect = cv::boundingRect(contours[i]);
            cv::rectangle (img, bounding_rect, cv::Scalar(0,255,0), 2);
        }
    }
    current = img.clone();
    cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
    m_image = QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888).copy();
}

void CvImageProvider::filter_2d(const int &kernelType)
{
    if (original.empty()) {return;}
    cv::Mat kernel;
    if (kernelType==0)    kernel = (cv::Mat_<float>(3,3) <<
                                                        0, -1,  0,
                                                        -1,  5, -1,
                                                        0, -1,  0);

    else if (kernelType==1)    kernel = (cv::Mat_<float>(3,3) <<
                                                        -1, -1, -1,
                                                        -1,  9, -1,
                                                        -1, -1, -1);
    else if (kernelType==2)    kernel = (cv::Mat_<float>(3,3) <<
                                                        -1, 0, 1,
                                                        -2, 0, 2,
                                                        -1, 0, 1);
    else if (kernelType==3)    kernel = (cv::Mat_<float>(3,3) <<
                                                        -1, -2, -1,
                                                        0,  0,  0,
                                                        1,  2,  1);
    else if (kernelType==4)    kernel = (cv::Mat_<float>(3,3) <<
                                                        0,  1,  0,
                                                        1, -4,  1,
                                                        0,  1,  0);
    else if (kernelType==5)    kernel = (cv::Mat_<float>(3,3) <<
                                                        1,  1,  1,
                                                        1, -8,  1,
                                                        1,  1,  1);
    else if (kernelType==6)    kernel = (cv::Mat_<float>(3,3) <<
                                                        1,  1,  1,
                                                        1, -8,  1,
                                                        1,  1,  1);
    else if (kernelType==7)    kernel = (cv::Mat_<float>(3,3) <<
                                                          1,  1,  1,
                                                      1, -8,  1,
                                                      1,  1,  1);
    else if (kernelType==8)     kernel = (cv::Mat_<float>(3,3) <<
                                                          -1, -1, -1,
                                                      -1,  8, -1,
                                                      -1, -1, -1);
    else {
        kernel = cv::Mat::zeros(5, 5, CV_32F)/(float)(5*5);
    }
    cv::Mat result;
    cv::filter2D(original, result, -1, kernel, cv::Point(-1,-1), 0, cv::BORDER_DEFAULT);
    current = result.clone();
    cv::cvtColor(result, result, cv::COLOR_BGR2RGB);
    m_image = QImage(result.data, result.cols, result.rows, result.step, QImage::Format_RGB888).copy();
}

void CvImageProvider::sobel(const int &ksize, const double &scale, const int &delta)
{
    if (original.empty()) {return;}
    cv::Mat gray, grad;
    cv::GaussianBlur(original, gray, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT);
    cv::cvtColor(gray, gray, cv::COLOR_BGR2GRAY);

    cv::Mat grad_x, grad_y, abs_grad_x, abs_grad_y;
    cv::Sobel(gray, grad_x, CV_16S, 1, 0, ksize, scale, delta, cv::BORDER_DEFAULT);
    cv::Sobel(gray, grad_y, CV_16S, 0, 1, ksize, scale, delta, cv::BORDER_DEFAULT);

    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);

    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0 , grad);
    current = grad.clone();
    m_image = QImage(grad.data, grad.cols, grad.rows, grad.step, QImage::Format_Grayscale8).copy();

}

void CvImageProvider::laplace(const int &ksize, const double &scale, const int &delta)
{
    if (original.empty()) {return;}
    cv::Mat gray;
    cv::GaussianBlur(original, gray, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT);
    cv::cvtColor(gray, gray, cv::COLOR_BGR2GRAY);

    cv::Mat grad, abs_grad;
    cv::Laplacian(gray, grad, CV_16S, ksize, scale, delta, cv::BORDER_DEFAULT);

    cv::convertScaleAbs(grad, abs_grad);
    current = abs_grad.clone();
    m_image = QImage(abs_grad.data, abs_grad.cols, abs_grad.rows, abs_grad.step, QImage::Format_Grayscale8).copy();
}

void CvImageProvider::cannyDetect(const double& low_threshold, const double& high_threshold, const int& ksize, const bool& l2) {
    if (original.empty()) {return;}
    cv::Mat gray;
    cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5,5), 1.5);
    cv::Canny(gray, gray, low_threshold, high_threshold, ksize, l2);
    current = gray.clone();
    m_image = QImage(gray.data, gray.cols, gray.rows, gray.step, QImage::Format_Grayscale8).copy();
}

void CvImageProvider::houghline(const int &thres)
{
    if (original.empty()) {return;}
    cv::Mat gray;
    cv::Mat result = original.clone();
    cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5,5), 1.5);
    cv::Canny(gray, gray, 50, 200, 3);
    std::vector<cv::Vec2f> lines;
    cv::HoughLines(gray, lines, 1, CV_PI/180, thres, 0, 0);
    for (size_t i =0; i< lines.size(); i++){
        float rho = lines[i][0], theta = lines[i][1];
        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        cv::line( result, pt1, pt2, cv::Scalar(0,0,255), 3, cv::LINE_AA);
    }
    current = result.clone();
    cv::cvtColor(result, result, cv::COLOR_BGR2RGB);
    m_image = QImage(result.data, result.cols, result.rows, result.step, QImage::Format_RGB888).copy();
}

void CvImageProvider::houghline_p(const int &thres, const double &minLength, const int &maxGap)
{
    if (original.empty()) {return;}
    cv::Mat gray;
    cv::Mat result = original.clone();
    cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5,5), 1.5);
    cv::Canny(gray, gray, 50, 200, 3);
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(gray, lines, 1, CV_PI/180, thres, minLength, maxGap);
    for (size_t i =0; i< lines.size(); i++){
        cv::Vec4i l = lines[i];
        cv::line(result, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 3, cv::LINE_AA);
    }
    current = result.clone();
    cv::cvtColor(result, result, cv::COLOR_BGR2RGB);
    m_image = QImage(result.data, result.cols, result.rows, result.step, QImage::Format_RGB888).copy();
}

void CvImageProvider::hough_circle(const int& minRadius, const int& maxRadius, const double& distance){
    if (original.empty()) {return;}
    cv::Mat gray;
    cv::Mat result = original.clone();
    cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
    std::vector<cv::Vec3f> circles;
    cv::medianBlur(gray, gray, 5);
    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1,
                     distance,
                     100, 30, minRadius, maxRadius);
    for (size_t i=0; i< circles.size(); i++){
        cv::circle(result, cv::Point(circles[i][0], circles[i][1]), circles[i][2],
                   cv::Scalar(0,0,255), 3, cv::LINE_AA);
    }
    current = result.clone();
    cv::cvtColor(result, result, cv::COLOR_BGR2RGB);
    m_image = QImage(result.data, result.cols, result.rows, result.step, QImage::Format_RGB888).copy();
}

void CvImageProvider::re_map(const int &type)
{
    if (original.empty()) {return;}
    cv::Mat result(original.size(), original.type());
    cv::Mat map_x(original.size(), CV_32FC1);
    cv::Mat map_y(original.size(), CV_32FC1);
    for( int i = 0; i < map_x.rows; i++ )
    {
        for( int j = 0; j < map_x.cols; j++ )
        {
            switch( type )
            {
            case 0:
                map_x.at<float>(i, j) = (float)j;
                map_y.at<float>(i, j) = (float)(map_x.rows - i);
                break;
            case 1:
                map_x.at<float>(i, j) = (float)(map_x.cols - j);
                map_y.at<float>(i, j) = (float)i;
                break;
            case 2:
                map_x.at<float>(i, j) = (float)(map_x.cols - j);
                map_y.at<float>(i, j) = (float)(map_x.rows - i);
                break;
            default:
                break;
            } // end of switch
        }
    }
    cv::remap(original, result, map_x, map_y, cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0) );
    current = result.clone();
    cv::cvtColor(result, result, cv::COLOR_BGR2RGB);
    m_image = QImage(result.data, result.cols, result.rows, result.step, QImage::Format_RGB888).copy();
}

void CvImageProvider::rotation(const int &angle, const float &scale)
{
    if (original.empty()) {return;}
    cv::Point center = cv::Point(original.cols/2, original.rows/2);
    cv::Mat rotate_mat = cv::getRotationMatrix2D(center, angle, scale);
    cv::Mat result = cv::Mat::zeros(original.size(), original.type());
    cv::warpAffine(original, result, rotate_mat, result.size());
    current = result.clone();
    cv::cvtColor(result, result, cv::COLOR_BGR2RGB);
    m_image = QImage(result.data, result.cols, result.rows, result.step, QImage::Format_RGB888).copy();

}

void CvImageProvider::affine(const double &p1x, const double &p1y, const double &p2x, const double &p2y, const double &p3x, const double &p3y)
{
    if (original.empty()) {return;}
    cv::Point2f srcTri[3];
    srcTri[0] = cv::Point2f(0.f,0.f);
    srcTri[1] = cv::Point2f(original.cols - 1.f,0.f);
    srcTri[2] = cv::Point2f(0.f,original.rows - 1.f);

    cv::Point2f dstTri[3];
    dstTri[0] = cv::Point2f(p1x,original.cols*p1y);
    dstTri[1] = cv::Point2f(original.cols*p2x ,original.cols*p2y);
    dstTri[2] = cv::Point2f(original.cols*p3x ,original.cols*p3y);

    cv::Mat warp_ = cv::getAffineTransform(srcTri, dstTri);
    cv::Mat result = cv::Mat::zeros(original.size(), original.type());
    cv::warpAffine(original, result, warp_, result.size());
    current = result.clone();
    cv::cvtColor(result, result, cv::COLOR_BGR2RGB);
    m_image = QImage(result.data, result.cols, result.rows, result.step, QImage::Format_RGB888).copy();
}

void CvImageProvider::guil_ballard(const int &matchingType, const QString &tempPath)
{
    cv::Mat temp = cv::imread(tempPath.toStdString(), cv::IMREAD_GRAYSCALE);
    if (temp.empty()) {return;}
    if (original.empty()) {return;}
    cv::Mat gray, temp_gray;
    cv::Mat result = original.clone();
    cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
    int w = temp.cols, h = temp.rows;
    std::vector<cv::Vec4f> positionBallard, positionGuil;
    if (matchingType){
        cv::Ptr<cv::GeneralizedHoughGuil> guil = cv::createGeneralizedHoughGuil();
        guil->setMinDist(10);
        guil->setLevels(360);
        guil->setDp(3);
        guil->setMaxBufferSize(1000);

        guil->setMinAngle(0);
        guil->setMaxAngle(360);
        guil->setAngleStep(1);
        guil->setAngleThresh(1500);

        guil->setMinScale(0.5);
        guil->setMaxScale(2.0);
        guil->setScaleStep(0.05);
        guil->setScaleThresh(50);

        guil->setPosThresh(10);

        guil->setCannyLowThresh(30);
        guil->setCannyHighThresh(110);

        guil->setTemplate(temp);
        guil->detect(gray, positionGuil);
        for (std::vector<cv::Vec4f>::iterator iter = positionGuil.begin(); iter != positionGuil.end(); ++iter) {
            cv::RotatedRect rRect = cv::RotatedRect(cv::Point2f((*iter)[0], (*iter)[1]),
                                            cv::Size2f(w * (*iter)[2], h * (*iter)[2]),
                                            (*iter)[3]);
            cv::Point2f vertices[4];
            rRect.points(vertices);
            for (int i = 0; i < 4; i++)
                cv::line(result, vertices[i], vertices[(i + 1) % 4], cv::Scalar(0, 255, 0), 2);
        }
        current = result.clone();
        cv::cvtColor(result, result, cv::COLOR_BGR2RGB);
        m_image = QImage(result.data, result.cols, result.rows, result.step, QImage::Format_RGB888).copy();
    }
    else{
        cv::Ptr<cv::GeneralizedHoughBallard> ballard = cv::createGeneralizedHoughBallard();
        ballard->setMinDist(10);
        ballard->setLevels(360);
        ballard->setDp(2);
        ballard->setMaxBufferSize(1000);
        ballard->setVotesThreshold(40);

        ballard->setCannyLowThresh(30);
        ballard->setCannyHighThresh(110);
        ballard->setTemplate(temp);
        ballard->detect(gray, positionBallard);
        for (std::vector<cv::Vec4f>::iterator iter = positionBallard.begin(); iter != positionBallard.end(); ++iter) {
            cv::RotatedRect rRect = cv::RotatedRect(cv::Point2f((*iter)[0], (*iter)[1]),
                                            cv::Size2f(w * (*iter)[2], h * (*iter)[2]),
                                            (*iter)[3]);
            cv::Point2f vertices[4];
            rRect.points(vertices);
            for (int i = 0; i < 4; i++)
                cv::line(result, vertices[i], vertices[(i + 1) % 4], cv::Scalar(255, 0, 0), 6);
        }
        current = result.clone();
        cv::cvtColor(result, result, cv::COLOR_BGR2RGB);
        m_image = QImage(result.data, result.cols, result.rows, result.step, QImage::Format_RGB888).copy();
    }


}

void CvImageProvider::harris_corner(const int &thresh)
{
    if (original.empty()) {return;}
    cv::Mat gray;
    cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
    cv::Mat dst = cv::Mat::zeros(original.size(), CV_32FC1);
    cv::cornerHarris( gray, dst, 2, 3, 0.04 );

    cv::Mat dst_norm, dst_norm_scaled;
    cv::normalize( dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat() );
    cv::convertScaleAbs( dst_norm, dst_norm_scaled );
    for( int i = 0; i < dst_norm.rows ; i++ )
    {
        for( int j = 0; j < dst_norm.cols; j++ )
        {
            if( (int) dst_norm.at<float>(i,j) > thresh )
            {
                circle( dst_norm_scaled, cv::Point(j,i), 5,  cv::Scalar(0), 2, 8, 0 );
            }
        }
    }
    m_image = QImage(dst_norm_scaled.data, dst_norm_scaled.cols, dst_norm_scaled.rows, dst_norm_scaled.step, QImage::Format_Grayscale8).copy();
}

void CvImageProvider::shitomasi(const int &max_corners)
{
    if (original.empty()) {return;}
    cv::Mat gray;
    cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
    cv::Mat result = original.clone();
    std::vector<cv::Point2f> corners;

    cv::goodFeaturesToTrack(gray,
                            corners,
                            max_corners,
                            0.01,
                            10);

    int radius = 4;
    for( size_t i = 0; i < corners.size(); i++ )
    {
        cv::circle(result, corners[i], radius, cv::Scalar(0,255,0), cv::FILLED );
    }
    cv::cvtColor(result, result, cv::COLOR_BGR2RGB);
    m_image = QImage(result.data, result.cols, result.rows, result.step, QImage::Format_RGB888).copy();

}

void CvImageProvider::custom(const int &type, const int &level)
{
    if (original.empty()) {return;}
    cv::Mat gray, dst, Mc;
    cv::Mat result = original.clone();
    int max_Level =100;
    double myHarris_minVal, myHarris_maxVal;
    double myShiTomasi_minVal, myShiTomasi_maxVal;
    cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
    if(type==0){
        cv::cornerEigenValsAndVecs(gray, dst, 3, 3);
        Mc = cv::Mat(gray.size(), CV_32FC1);
        for( int i = 0; i < gray.rows; i++ )
        {
            for( int j = 0; j < gray.cols; j++ )
            {
                float lambda_1 = dst.at<cv::Vec6f>(i, j)[0];
                float lambda_2 = dst.at<cv::Vec6f>(i, j)[1];
                Mc.at<float>(i, j) = lambda_1*lambda_2 - 0.04f*((lambda_1 + lambda_2) * (lambda_1 + lambda_2));
            }
        }
        cv::minMaxLoc( Mc, &myHarris_minVal, &myHarris_maxVal );
        for( int i = 0; i < gray.rows; i++ )
        {
            for( int j = 0; j < gray.cols; j++ )
            {
                if( Mc.at<float>(i,j) > myHarris_minVal + ( myHarris_maxVal - myHarris_minVal )*level/max_Level )
                {
                    circle( result, cv::Point(j,i), 4, cv::Scalar(0, 255, 0), cv::FILLED );
                }
            }
        }
    }
    else{
        cv::cornerMinEigenVal( gray, dst, 3, 3);
        cv::minMaxLoc( dst, &myShiTomasi_minVal, &myShiTomasi_maxVal );
        for( int i = 0; i < gray.rows; i++ )
        {
            for( int j = 0; j < gray.cols; j++ )
            {
                if( dst.at<float>(i,j) > myShiTomasi_minVal + ( myShiTomasi_maxVal - myShiTomasi_minVal )*level/max_Level )
                {
                    circle( result, cv::Point(j,i), 4, cv::Scalar(0, 255, 0), cv::FILLED );
                }
            }
        }
    }
    cv::cvtColor(result, result, cv::COLOR_BGR2RGB);
    m_image = QImage(result.data, result.cols, result.rows, result.step, QImage::Format_RGB888).copy();
}

QString CvImageProvider::subpix(const int &max_corners)
{
    if (original.empty()) {return "";}
    cv::Mat gray;
    cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
    cv::Mat result = original.clone();
    std::vector<cv::Point2f> corners;

    cv::goodFeaturesToTrack(gray,
                            corners,
                            max_corners,
                            0.01,
                            10);

    int radius = 4;
    for( size_t i = 0; i < corners.size(); i++ )
    {
        cv::circle(result, corners[i], radius, cv::Scalar(0,255,0), cv::FILLED );
    }
    cv::cvtColor(result, result, cv::COLOR_BGR2RGB);
    m_image = QImage(result.data, result.cols, result.rows, result.step, QImage::Format_RGB888).copy();
    cv::TermCriteria criteria = cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 40, 0.001);
    cv::cornerSubPix(gray, corners, cv::Size(5,5), cv::Size(-1,-1), criteria);
    std::stringstream ss;
    for (size_t i=0; i<corners.size();i++){
        ss << "Corner [" << i << "]  (" << corners[i].x << "," << corners[i].y << ")      ";
        if(i%3==0)   ss << std::endl;
    }
    return QString::fromStdString(ss.str());
}

void CvImageProvider::siftDetection(const int &nfeatures, const double &contrastThre)
{
    cv::Ptr<cv::SIFT> sift_detector = cv::SIFT::create();
    sift_detector->setContrastThreshold(contrastThre);
    std::vector<cv::KeyPoint> keypoints;cv::Mat descriptors;
    cv::Mat result = original.clone();
    sift_detector->detectAndCompute(result, cv::noArray(), keypoints, descriptors);
    cv::drawKeypoints(result, keypoints, result, cv::Scalar(0, 255, 0), cv::DrawMatchesFlags::DEFAULT);
    cv::cvtColor(result, result, cv::COLOR_BGR2RGB);
    m_image = QImage(result.data, result.cols, result.rows, result.step, QImage::Format_RGB888).copy();
}


void CvImageProvider::orbDetection(const int &nfeatures, const double &fastThre)
{
    cv::Ptr<cv::ORB> orb_detector = cv::ORB::create(nfeatures);
    orb_detector->setFastThreshold(fastThre);
    std::vector<cv::KeyPoint> keypoints;cv::Mat descriptors;
    cv::Mat result = original.clone();
    orb_detector->detectAndCompute(result,cv::noArray(), keypoints, descriptors);
    cv::drawKeypoints(result, keypoints, result, cv::Scalar(0, 255, 0), cv::DrawMatchesFlags::DEFAULT);
    cv::cvtColor(result, result, cv::COLOR_BGR2RGB);
    m_image = QImage(result.data, result.cols, result.rows, result.step, QImage::Format_RGB888).copy();
}


void CvImageProvider::his_equalization(const int &imgType, const float &scale)
{
    if (original.empty()) {return;}

    if (imgType==0){
        cv::Mat gray, equalized, blended;
        cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, equalized);
        cv::addWeighted(equalized, scale, gray, 1-scale, 0, blended);
        current = blended.clone();
        m_image = QImage(blended.data, blended.cols, blended.rows, blended.step, QImage::Format_Grayscale8).copy();
    }
    else{
        cv::Mat ycrcb, result;
        cv::cvtColor(original, ycrcb, cv::COLOR_BGR2YCrCb);
        std::vector<cv::Mat> channels;
        cv::split(ycrcb, channels);
        cv::equalizeHist(channels[0], channels[0]);
        cv::merge(channels, ycrcb);
        cv::cvtColor(ycrcb, ycrcb, cv::COLOR_YCrCb2BGR);
        cv::addWeighted(ycrcb, scale, original, 1 - scale, 0, result);
        current = result.clone();
        cv::cvtColor(result, result, cv::COLOR_BGR2RGB);
        m_image = QImage(result.data, result.cols, result.rows, result.step, QImage::Format_RGB888).copy();
    }
}

void CvImageProvider::tempMatching(const int &matchingType, const QString &tempPath)
{
    cv::Mat temp = cv::imread(tempPath.toStdString(), cv::IMREAD_COLOR);
    if (temp.empty()) {return;}
    if (original.empty()) {return;}
    cv::Mat image = original.clone();
    cv::Mat result;
    result.create(image.cols-temp.cols+1,image.rows-temp.rows+1, CV_32FC1);
    cv::matchTemplate(image, temp, result, matchingType);
    cv::normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());
    double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());
    cv::rectangle(image, maxLoc, cv::Point(maxLoc.x + temp.cols , maxLoc.y + temp.rows ), cv::Scalar(0,255,0), 2, 8, 0);
    current = image.clone();
    cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
    m_image = QImage(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888).copy();
}
cv::Scalar getColor(const int& type){

    cv::RNG randomGen(cv::getTickCount());
    if (type==0)  return cv::Scalar(0,0,255);
    else if (type==1) return cv::Scalar(0, 255, 0);
    else if (type==2) return cv::Scalar(255, 0, 0);
    else if (type==3) return cv::Scalar(0, 255, 255);
    else return cv::Scalar(randomGen.uniform(0, 256), randomGen.uniform(0, 256), randomGen.uniform(0, 256));
}
void CvImageProvider::drawEgde(const int &cannyThres, const int &colorType)
{
    cv::Mat gray;cv::RNG randomColor;
    cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5,5), 1.5);
    cv::Canny(gray, gray, cannyThres, cannyThres*2);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(gray, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    cv::Mat drawing = cv::Mat::zeros(original.size(), CV_8UC3);
    for (int i=0; i< contours.size(); i ++){
        cv::Scalar _color =getColor(colorType);
        cv::drawContours(drawing, contours, i, _color, 2, cv::LINE_8, hierarchy, 0);
    }
    current = drawing.clone();
    cv::cvtColor(drawing, drawing, cv::COLOR_BGR2RGB);
    m_image = QImage(drawing.data, drawing.cols, drawing.rows, drawing.step, QImage::Format_RGB888).copy();
}

void CvImageProvider::convexHull(const int &cannyThres, const int &colorType)
{
    cv::Mat gray;
    cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5,5), 1.5);
    cv::Canny(gray, gray, cannyThres, cannyThres*2);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(gray, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    cv::Mat drawing = cv::Mat::zeros(original.size(), CV_8UC3);
    std::vector<std::vector<cv::Point>> hull(contours.size());
    for( size_t i = 0; i < contours.size(); i++ )
    {
        cv::convexHull(contours[i], hull[i] );
    }
    for (int i=0; i< contours.size(); i ++){
        cv::Scalar _color =getColor(colorType);
        cv::drawContours(drawing, hull, i, _color, 2, cv::LINE_8, hierarchy, 0);
    }
    current = drawing.clone();
    cv::cvtColor(drawing, drawing, cv::COLOR_BGR2RGB);
    m_image = QImage(drawing.data, drawing.cols, drawing.rows, drawing.step, QImage::Format_RGB888).copy();
}

void CvImageProvider::drawShape(const int &cannyThres, const int &colorType, const int &shapeID)
{
    cv::Mat gray;cv::RNG randomColor;
    cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5,5), 1.5);
    cv::Canny(gray, gray, cannyThres, cannyThres*2);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(gray, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    cv::Mat result = cv::Mat::zeros( original.size(), CV_8UC3 );
    if (shapeID==0){
        std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
        std::vector<cv::Rect> boundRect( contours.size() );
        for( size_t i = 0; i < contours.size(); i++ )
        {
            cv::approxPolyDP( contours[i], contours_poly[i], 3, true );
            boundRect[i] = boundingRect( contours_poly[i] );
        }

        for( size_t i = 0; i< contours.size(); i++ )
        {
            cv::Scalar _color =getColor(colorType);
            cv::drawContours( result, contours_poly, (int)i, _color );
            cv::rectangle( result, boundRect[i].tl(), boundRect[i].br(), _color, 2 );
        }
    }
    else if (shapeID==1){
        std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
        std::vector<cv::Point2f> centers( contours.size() );
        std::vector<float> radius( contours.size() );
        for( size_t i = 0; i < contours.size(); i++ )
        {
            cv::approxPolyDP( contours[i], contours_poly[i], 3, true );
            cv::minEnclosingCircle( contours_poly[i], centers[i], radius[i] );
        }

        for( size_t i = 0; i< contours.size(); i++ )
        {
            cv::Scalar _color =getColor(colorType);
            cv::drawContours( result, contours_poly, (int)i, _color );
            cv::circle( result, centers[i], (int)radius[i], _color, 2 );
        }
    }
    else if (shapeID==2){
        std::vector<cv::RotatedRect> minRect(contours.size());
        for (size_t i=0; i< contours.size(); i++){
            minRect[i] = cv::minAreaRect(contours[i]);
        }
        for (size_t i=0; i< contours.size(); i++){
            cv::Scalar _color =getColor(colorType);
            drawContours( result, contours, (int)i, getColor((colorType+1)%4));
            cv::Point2f rect_points[4];
            minRect[i].points(rect_points);
            for(int j=0;j<4;j++) cv::line(result, rect_points[j], rect_points[(j+1)%4], _color);
        }

    }
    else if (shapeID==3){
        std::vector<cv::RotatedRect> minEllipse(contours.size());
        for (size_t i=0; i< contours.size(); i++){
            if( contours[i].size() > 5 )
            {
                minEllipse[i] = cv::fitEllipse( contours[i] );
            }
        }
        for (size_t i=0; i< contours.size(); i++){
            cv::Scalar _color =getColor(colorType);
            drawContours( result, contours, (int)i, getColor((colorType+1)%4) );
            cv::ellipse( result, minEllipse[i], _color, 2 );
        }
    }
    current = result.clone();
    cv::cvtColor(result, result, cv::COLOR_BGR2RGB);
    m_image = QImage(result.data, result.cols, result.rows, result.step, QImage::Format_RGB888).copy();
}

void CvImageProvider::moment(const int &cannyThres, const int &colorType)
{
    cv::Mat gray;
    cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5,5), 1.5);
    cv::Canny(gray, gray, cannyThres, cannyThres*2);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(gray, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    cv::Mat drawing = cv::Mat::zeros(original.size(), CV_8UC3);
    std::vector<cv::Moments> mu(contours.size());
    std::vector<cv::Point2f> mc(contours.size());
    for( size_t i = 0; i < contours.size(); i++ )
    {
        mu[i] = cv::moments(contours[i]);
        mc[i] = cv::Point2f(static_cast<float> (mu[i].m10/ (mu[i].m00 + 1e-5)),
                            static_cast<float> (mu[i].m01/ (mu[i].m00 + 1e-5)));
    }
    for (int i=0; i< contours.size(); i ++){
        cv::Scalar _color =getColor(colorType);
        cv::drawContours(drawing, contours, i, getColor((colorType+1)%4), 2, cv::LINE_8, hierarchy, 0);
        cv::circle(drawing, mc[i], 4, _color, -1);

    }
    current = drawing.clone();
    cv::cvtColor(drawing, drawing, cv::COLOR_BGR2RGB);
    m_image = QImage(drawing.data, drawing.cols, drawing.rows, drawing.step, QImage::Format_RGB888).copy();
}

void CvImageProvider::point_test(const int &cannyThres, const int &contourType)
{
    cv::Mat gray;
    cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5,5), 1.5);
    cv::Canny(gray, gray, cannyThres, cannyThres*2);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(gray, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    cv::Mat drawing = cv::Mat::zeros(original.size(), CV_8UC3);
    cv::Mat raw_dist( original.size(), CV_32F );
    int maxIndex =0, minIndex=0;
    double maxArea = 0.0, minArea = 5000;
    for( size_t i = 0; i < contours.size(); i++ )
    {
        double area = cv::contourArea(contours[i]);
        if (area > maxArea){
            maxArea = area;
            maxIndex = i;
        }
        if (area < minArea){
            minArea = area;
            minIndex = i;
        }
    }
    std::vector<cv::Point> contour;
    if (contourType==0)  contour = contours[0];
    else if (contourType==1) contour = contours[contours.size()-1];
    else if (contourType==2) contour = contours[maxIndex];
    else if (contourType==3) contour = contours[minIndex];
    else {
        cv::RNG randomIndex(cv::getTickCount());
        contour = contours[randomIndex.uniform(0, (int)(contours.size()-1))];
    }
    for( int i = 0; i < raw_dist.rows; i++ )
    {
        for( int j = 0; j < raw_dist.cols; j++ )
        {
            raw_dist.at<float>(i,j) = (float)cv::pointPolygonTest( contour, cv::Point2f((float)j, (float)i), true );
        }
    }

    double minVal, maxVal;
    cv::Point maxDistPt;
    cv::minMaxLoc(raw_dist, &minVal, &maxVal, NULL, &maxDistPt);
    minVal = abs(minVal);
    maxVal = abs(maxVal);
    for( int i = 0; i < drawing.rows; i++ )
    {
        for( int j = 0; j < drawing.cols; j++ )
        {
            if( raw_dist.at<float>(i,j) < 0 )
            {
                drawing.at<cv::Vec3b>(i,j)[0] = (uchar)(255 - abs(raw_dist.at<float>(i,j)) * 255 / minVal);
            }
            else if( raw_dist.at<float>(i,j) > 0 )
            {
                drawing.at<cv::Vec3b>(i,j)[2] = (uchar)(255 - raw_dist.at<float>(i,j) * 255 / maxVal);
            }
            else
            {
                drawing.at<cv::Vec3b>(i,j)[0] = 255;
                drawing.at<cv::Vec3b>(i,j)[1] = 255;
                drawing.at<cv::Vec3b>(i,j)[2] = 255;
            }
        }
    }
    cv::circle(drawing, maxDistPt, (int)maxVal, getColor(1));
    current = drawing.clone();
    cv::cvtColor(drawing, drawing, cv::COLOR_BGR2RGB);
    m_image = QImage(drawing.data, drawing.cols, drawing.rows, drawing.step, QImage::Format_RGB888).copy();
}

