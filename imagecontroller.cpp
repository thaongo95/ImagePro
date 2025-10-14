#include "imagecontroller.h"
#include <QFileInfo>
cv::Scalar getColor(const int& type){

    cv::RNG randomGen(cv::getTickCount());
    if (type==0)  return cv::Scalar(0,0,255);
    else if (type==1) return cv::Scalar(0, 255, 0);
    else if (type==2) return cv::Scalar(255, 0, 0);
    else if (type==3) return cv::Scalar(0, 255, 255);
    else return cv::Scalar(randomGen.uniform(0, 256), randomGen.uniform(0, 256), randomGen.uniform(0, 256));
}
ImageController::ImageController(CvImageProvider* provider, QObject* parent)
    : QObject(parent), m_provider(provider) {}

void ImageController::loadImage(const QString &filePath) {
    m_original = cv::imread(filePath.toStdString(), cv::IMREAD_COLOR);
    if (m_original.empty()) {
        qWarning() << "Failed to load image:" << filePath;
        return;
    }
    QFileInfo fi(filePath);
    image_info = "";
    image_info += QString("Name: %1 | ").arg(fi.fileName());
    image_info += QString("Size: %1 x %2 | ").arg(m_original.cols).arg(m_original.rows);
    image_info += QString("Channels: %1 | ").arg(m_original.channels());
    image_info += QString("Image size: %1 bytes | ").arg(m_original.total() * m_original.elemSize());
    image_info += QString("Total pixels: %1").arg(m_original.total());
    m_current = m_original.clone();
    if (m_provider)
        m_provider->updateQImage(m_current);
}

QString ImageController::getIntensity(int x, int y)
{
    if (m_original.empty()) {return "";}
    if (x < 0 || y < 0 || x >= m_original.cols || y >= m_original.rows) {return "";}
    QString result = QString("");
    if (m_original.channels()==1){
        int val = m_original.at<uchar>(y,x);
        result = QString("Gray: %1").arg(val);
    }
    else if(m_original.channels()==3){
        cv::Vec3b intensity = m_original.at<cv::Vec3b>(y, x);
        int blue = intensity[0];
        int green = intensity[1];
        int red = intensity[2];
        result = QString("R:%1 G:%2 B:%3 ")
                     .arg(red).arg(green).arg(blue);
    }
    return result;
}

void ImageController::createImage(int width, int height, int red, int green, int blue)
{
    m_current = cv::Mat(height, width, CV_8UC3, cv::Scalar(red, green, blue));
    m_temp = m_current.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::saveImage(const QString &filePath)
{
    if (m_temp.empty()) {
        return;
    }
    cv::imwrite(filePath.toStdString(), m_temp);
}


void ImageController::basicExposure(const double &alpha, const int &beta)
{
    if (m_current.empty()) {
        return;
    }
    cv::Mat expoImg;
    expoImg = exposure(m_current, alpha, beta);
    cv::cvtColor(expoImg, expoImg, cv::COLOR_BGR2RGB);
    m_temp = expoImg.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::gammaCorrect(const double &gammaValue)
{
    if (m_current.empty()) {
        return;
    }
    cv::Mat gammaImg;
    gammaImg = gamma_(m_current, gammaValue);
    m_temp = gammaImg.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::smoothing(const int &blurType, const int& size)
{
    if (m_current.empty()) {
        return;
    }
    cv::Mat blurImg;
    if (blurType==0) cv::blur(m_current, blurImg, cv::Size(size,size));
    else if (blurType==1) cv::boxFilter(m_current, blurImg, -1, cv::Size(size,size), cv::Point(-1,-1), true);
    else if (blurType==2) cv::GaussianBlur(m_current, blurImg, cv::Size(size,size), 1.5);
    else if (blurType==3) cv::medianBlur(m_current, blurImg, size);
    else cv::bilateralFilter(m_current, blurImg, 9, 75, 75);
    m_temp = blurImg.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::erosion(const int &type, const int &size)
{
    if (m_current.empty()) {
        return;
    }
    int erosion_type = 0;
    if( type == 0 ){ erosion_type = cv::MORPH_RECT; }
    else if( type == 1 ){ erosion_type = cv::MORPH_CROSS; }
    else if( type == 2) { erosion_type = cv::MORPH_ELLIPSE; }
    cv::Mat element = cv::getStructuringElement(erosion_type, cv::Size(2*size+1, 2*size+1), cv::Point(size,size));
    cv::Mat erodeImg;
    cv::erode(m_current, erodeImg, element);
    m_temp = erodeImg.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::dilation(const int &type, const int &size)
{
    if (m_current.empty()) {
        return;
    }
    int dilation_type=0;
    if( type == 0 ){ dilation_type = cv::MORPH_RECT; }
    else if( type == 1 ){ dilation_type = cv::MORPH_CROSS; }
    else if( type == 2) { dilation_type = cv::MORPH_ELLIPSE; }
    cv::Mat element = cv::getStructuringElement(dilation_type, cv::Size(2*size+1, 2*size+1), cv::Point(size,size));
    cv::Mat dilateImg;
    cv::dilate(m_current, dilateImg, element);
    m_temp = dilateImg.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::morphology(const int &morph_operator, const int &morph_type, const int &morph_size)
{
    if (m_current.empty()) {
        return;
    }
    int _type = 0;
    if( morph_type == 0 ){ _type = cv::MORPH_RECT; }
    else if( morph_type == 1 ){ _type = cv::MORPH_CROSS; }
    else if( morph_type == 2) { _type = cv::MORPH_ELLIPSE; }
    cv::Mat element = cv::getStructuringElement(_type, cv::Size(morph_size*2+1, morph_size*2+1), cv::Point(morph_size, morph_size));
    cv::Mat morphImg;
    cv::morphologyEx(m_current, morphImg, morph_operator+2, element);
    m_temp = morphImg.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::convertImg(const int &index)
{
    if (m_current.empty()) {
        return;
    }
    cv::Mat result;
    if( index == 0 ){
        cv::cvtColor(m_current, result, cv::COLOR_BGR2GRAY);
        cv::cvtColor(result, result, cv::COLOR_GRAY2RGB);
    }
    else if( index == 1 ){
        cv::cvtColor(m_current, result, cv::COLOR_BGR2HSV);
        cv::cvtColor(result, result, cv::COLOR_HSV2RGB);
    }
    else if( index == 2) {
        cv::cvtColor(m_current, result, cv::COLOR_BGR2Lab);
        cv::cvtColor(result, result, cv::COLOR_Lab2RGB);
    }
    else if( index == 3) {
        cv::cvtColor(m_current, result, cv::COLOR_BGR2YCrCb);
        cv::cvtColor(result, result, cv::COLOR_YCrCb2RGB);
    }
    m_temp = result.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::extractLines(const int &type, const double &size)
{
    if (m_current.empty()) {
        return;
    }
    cv::Mat gray;
    cv::cvtColor(m_current, gray, cv::COLOR_BGR2GRAY);
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
    m_temp = result.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::basicThreshold(const int &threType, const int &threValue)
{
    if (m_current.empty()) {
        return;
    }
    cv::Mat gray;
    cv::cvtColor(m_current, gray, cv::COLOR_BGR2GRAY);
    cv::threshold(gray, gray, threValue, 255, threType);
    m_temp = gray.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::inrangeHSV(const int &h_low, const int &h_high, const int &s_low, const int &s_high, const int &v_low, const int &v_high)
{
    if (m_current.empty()) {return;}
    cv::Mat hsvImg, result;
    cv::cvtColor(m_current, hsvImg, cv::COLOR_BGR2HSV);
    cv::inRange(hsvImg, cv::Scalar(h_low, s_low, v_low), cv::Scalar(h_high, s_high, v_high), result);
    m_temp = result.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::enumColorDetect(const int &colorType, const double &thre_erea)
{
    if (m_current.empty()) {return;}
    COLOR _color;
    if (colorType ==0) _color = COLOR::RED;
    else if (colorType ==1) _color = COLOR::GREEN;
    else if (colorType ==2) _color = COLOR::BLUE;
    else if (colorType ==3) _color = COLOR::YELLOW;
    else if (colorType ==4) _color = COLOR::BLACK;
    cv::Mat result = m_current.clone();
    result = color_detection(m_current, thre_erea, _color);
    m_temp = result.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::customColorDetect(const double &thre_erea, const int &h_low, const int &h_high, const int &s_low, const int &s_high, const int &v_low, const int &v_high)
{
    if (m_current.empty()) {return;}
    cv::Mat mask, result;
    cv::cvtColor(m_current, mask, cv::COLOR_BGR2HSV);
    cv::Mat img = m_current.clone();
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
    m_temp = img.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::filter_2d(const int &kernelType)
{
    if (m_current.empty()) {return;}
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
    cv::filter2D(m_current, result, -1, kernel, cv::Point(-1,-1), 0, cv::BORDER_DEFAULT);
    m_temp = result.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::sobel(const int &ksize, const double &scale, const int &delta)
{
    if (m_current.empty()) {return;}
    cv::Mat gray, grad;
    cv::GaussianBlur(m_current, gray, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT);
    cv::cvtColor(gray, gray, cv::COLOR_BGR2GRAY);

    cv::Mat grad_x, grad_y, abs_grad_x, abs_grad_y;
    cv::Sobel(gray, grad_x, CV_16S, 1, 0, ksize, scale, delta, cv::BORDER_DEFAULT);
    cv::Sobel(gray, grad_y, CV_16S, 0, 1, ksize, scale, delta, cv::BORDER_DEFAULT);

    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);

    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0 , grad);
    m_temp = grad.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::laplace(const int &ksize, const double &scale, const int &delta)
{
    if (m_current.empty()) {return;}
    cv::Mat gray;
    cv::GaussianBlur(m_current, gray, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT);
    cv::cvtColor(gray, gray, cv::COLOR_BGR2GRAY);

    cv::Mat grad, abs_grad;
    cv::Laplacian(gray, grad, CV_16S, ksize, scale, delta, cv::BORDER_DEFAULT);

    cv::convertScaleAbs(grad, abs_grad);
    m_temp = abs_grad.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::cannyDetect(const double &low_threshold, const double &high_threshold, const int &ksize, const bool &l2)
{
    if (m_current.empty()) {return;}
    cv::Mat gray;
    cv::cvtColor(m_current, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5,5), 1.5);
    cv::Canny(gray, gray, low_threshold, high_threshold, ksize, l2);
    m_temp = gray.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::houghline(const int &thres)
{
    if (m_current.empty()) {return;}
    cv::Mat gray;
    cv::Mat result = m_current.clone();
    cv::cvtColor(m_current, gray, cv::COLOR_BGR2GRAY);
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
    m_temp = result.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::houghline_p(const int &thres, const double &minLength, const int &maxGap)
{
    if (m_current.empty()) {return;}
    cv::Mat gray;
    cv::Mat result = m_current.clone();
    cv::cvtColor(m_current, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5,5), 1.5);
    cv::Canny(gray, gray, 50, 200, 3);
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(gray, lines, 1, CV_PI/180, thres, minLength, maxGap);
    for (size_t i =0; i< lines.size(); i++){
        cv::Vec4i l = lines[i];
        cv::line(result, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 3, cv::LINE_AA);
    }
    m_temp = result.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::hough_circle(const int &minRadius, const int &maxRadius, const double &distance)
{
    if (m_current.empty()) {return;}
    cv::Mat gray;
    cv::Mat result = m_current.clone();
    cv::cvtColor(m_current, gray, cv::COLOR_BGR2GRAY);
    std::vector<cv::Vec3f> circles;
    cv::medianBlur(gray, gray, 5);
    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1,
                     distance,
                     100, 30, minRadius, maxRadius);
    for (size_t i=0; i< circles.size(); i++){
        cv::circle(result, cv::Point(circles[i][0], circles[i][1]), circles[i][2],
                   cv::Scalar(0,0,255), 3, cv::LINE_AA);
    }
    m_temp = result.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::re_map(const int &type)
{
    if (m_current.empty()) {return;}
    cv::Mat result(m_current.size(), m_current.type());
    cv::Mat map_x(m_current.size(), CV_32FC1);
    cv::Mat map_y(m_current.size(), CV_32FC1);
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
    cv::remap(m_current, result, map_x, map_y, cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0) );
    m_temp = result.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::rotation(const int &angle, const float &scale)
{
    if (m_current.empty()) {return;}
    cv::Point center = cv::Point(m_current.cols/2, m_current.rows/2);
    cv::Mat rotate_mat = cv::getRotationMatrix2D(center, angle, scale);
    cv::Mat result = cv::Mat::zeros(m_current.size(), m_current.type());
    cv::warpAffine(m_current, result, rotate_mat, result.size());
    m_temp = result.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::affine(const double &p1x, const double &p1y, const double &p2x, const double &p2y, const double &p3x, const double &p3y)
{
    if (m_current.empty()) {return;}
    cv::Point2f srcTri[3];
    srcTri[0] = cv::Point2f(0.f,0.f);
    srcTri[1] = cv::Point2f(m_current.cols - 1.f,0.f);
    srcTri[2] = cv::Point2f(0.f,m_current.rows - 1.f);

    cv::Point2f dstTri[3];
    dstTri[0] = cv::Point2f(p1x,m_current.cols*p1y);
    dstTri[1] = cv::Point2f(m_current.cols*p2x ,m_current.cols*p2y);
    dstTri[2] = cv::Point2f(m_current.cols*p3x ,m_current.cols*p3y);

    cv::Mat warp_ = cv::getAffineTransform(srcTri, dstTri);
    cv::Mat result = cv::Mat::zeros(m_current.size(), m_current.type());
    cv::warpAffine(m_current, result, warp_, result.size());
    m_temp = result.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::guil_ballard(const int &matchingType, const QString &tempPath)
{
    cv::Mat temp = cv::imread(tempPath.toStdString(), cv::IMREAD_GRAYSCALE);
    if (temp.empty()) {return;}
    if (m_current.empty()) {return;}
    cv::Mat gray, temp_gray;
    cv::Mat result = m_current.clone();
    cv::cvtColor(m_current, gray, cv::COLOR_BGR2GRAY);
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
    }
    m_temp = result.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::harris_corner(const int &thresh)
{
    if (m_current.empty()) {return;}
    cv::Mat gray;
    cv::cvtColor(m_current, gray, cv::COLOR_BGR2GRAY);
    cv::Mat dst = cv::Mat::zeros(m_current.size(), CV_32FC1);
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
    m_temp = dst_norm_scaled.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::shitomasi(const int &max_corners)
{
    if (m_current.empty()) {return;}
    cv::Mat gray;
    cv::cvtColor(m_current, gray, cv::COLOR_BGR2GRAY);
    cv::Mat result = m_current.clone();
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
    m_temp = result.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::custom(const int &type, const int &level)
{
    if (m_current.empty()) {return;}
    cv::Mat gray, dst, Mc;
    cv::Mat result = m_current.clone();
    int max_Level =100;
    double myHarris_minVal, myHarris_maxVal;
    double myShiTomasi_minVal, myShiTomasi_maxVal;
    cv::cvtColor(m_current, gray, cv::COLOR_BGR2GRAY);
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
    m_temp = result.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

QString ImageController::subpix(const int &max_corners)
{
    if (m_current.empty()) {return "";}
    cv::Mat gray;
    cv::cvtColor(m_current, gray, cv::COLOR_BGR2GRAY);
    cv::Mat result = m_current.clone();
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
    m_temp = result.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
    cv::TermCriteria criteria = cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 40, 0.001);
    cv::cornerSubPix(gray, corners, cv::Size(5,5), cv::Size(-1,-1), criteria);
    std::stringstream ss;
    for (size_t i=0; i<corners.size();i++){
        ss << "Corner [" << i << "]  (" << corners[i].x << "," << corners[i].y << ")      ";
        if(i%3==0)   ss << std::endl;
    }
    return QString::fromStdString(ss.str());
}

void ImageController::siftDetection(const int &nfeatures, const double &contrastThre)
{
    cv::Ptr<cv::SIFT> sift_detector = cv::SIFT::create(nfeatures);
    sift_detector->setContrastThreshold(contrastThre);
    std::vector<cv::KeyPoint> keypoints;cv::Mat descriptors;
    cv::Mat result = m_current.clone();
    // cv::cvtColor(result,result,cv::COLOR_BGR2GRAY);
    sift_detector->detectAndCompute(result, cv::noArray(), keypoints, descriptors);
    cv::drawKeypoints(result, keypoints, result, cv::Scalar(0, 255, 0), cv::DrawMatchesFlags::DEFAULT);
    m_temp = result.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::orbDetection(const int &nfeatures, const double &fastThre)
{
    cv::Ptr<cv::ORB> orb_detector = cv::ORB::create(nfeatures);
    orb_detector->setFastThreshold(fastThre);
    std::vector<cv::KeyPoint> keypoints;cv::Mat descriptors;
    cv::Mat result = m_current.clone();
    // cv::cvtColor(result,result,cv::COLOR_BGR2GRAY);
    orb_detector->detectAndCompute(result,cv::noArray(), keypoints, descriptors);
    cv::drawKeypoints(result, keypoints, result, cv::Scalar(0, 255, 0), cv::DrawMatchesFlags::DEFAULT);
    m_temp = result.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::akazeDetection(const double &thresh)
{
    cv::Ptr<cv::AKAZE> akaze_detector = cv::AKAZE::create();
    akaze_detector->setThreshold(thresh);
    std::vector<cv::KeyPoint> keypoints;cv::Mat descriptors;
    cv::Mat result = m_current.clone();
    // cv::cvtColor(result,result,cv::COLOR_BGR2GRAY);
    akaze_detector->detectAndCompute(result,cv::noArray(), keypoints, descriptors);
    cv::drawKeypoints(result, keypoints, result, cv::Scalar(0, 255, 0), cv::DrawMatchesFlags::DEFAULT);
    m_temp = result.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::featuresMatching(const int &matchingType, const QString &tempPath, const int &thresh)
{
    cv::Mat temp = cv::imread(tempPath.toStdString(), cv::IMREAD_COLOR);
    if (temp.empty()) {return;}
    if (m_current.empty()) {return;}
    cv::Mat scene = m_current.clone();
    cv::cvtColor(temp,temp,cv::COLOR_BGR2GRAY);
    cv::cvtColor(scene,scene,cv::COLOR_BGR2GRAY);
    std::vector<cv::KeyPoint> keypoints, keypoints_temp;
    cv::Mat descriptors, descriptors_temp;
    if (matchingType==0){
        cv::Ptr<cv::SIFT> sift_detector = cv::SIFT::create(thresh);
        sift_detector->detectAndCompute(scene, cv::noArray(), keypoints, descriptors);
        sift_detector->detectAndCompute(temp, cv::noArray(), keypoints_temp, descriptors_temp);
    }
    if (matchingType==1){
        cv::Ptr<cv::ORB> orb_detector = cv::ORB::create(thresh);
        orb_detector->detectAndCompute(scene, cv::noArray(), keypoints, descriptors);
        orb_detector->detectAndCompute(temp, cv::noArray(), keypoints_temp, descriptors_temp);
    }
    if (matchingType==2){
        cv::Ptr<cv::AKAZE> akaze_detector = cv::AKAZE::create();

        akaze_detector->detectAndCompute(scene, cv::noArray(), keypoints, descriptors);
        akaze_detector->detectAndCompute(temp, cv::noArray(), keypoints_temp, descriptors_temp);
    }
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::BRUTEFORCE);
    std::vector< cv::DMatch > matches;
    matcher->match( descriptors, descriptors_temp, matches );

    //-- Draw matches
    cv::Mat result;
    cv::drawMatches( scene, keypoints, temp, keypoints_temp, matches, result );
    m_temp = result.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::featuresDetecting(const int &matchingType, const QString &tempPath, const int &thresh)
{
    cv::Mat temp = cv::imread(tempPath.toStdString(), cv::IMREAD_COLOR);
    if (temp.empty()) {return;}
    if (m_current.empty()) {return;}
    cv::Mat scene = m_current.clone();
    cv::cvtColor(temp,temp,cv::COLOR_BGR2GRAY);
    cv::cvtColor(scene,scene,cv::COLOR_BGR2GRAY);
    std::vector<cv::KeyPoint> keypoints, keypoints_temp;
    cv::Mat descriptors, descriptors_temp;
    if (matchingType==0){
        cv::Ptr<cv::SIFT> sift_detector = cv::SIFT::create(thresh);
        sift_detector->detectAndCompute(scene, cv::noArray(), keypoints, descriptors);
        sift_detector->detectAndCompute(temp, cv::noArray(), keypoints_temp, descriptors_temp);
    }
    if (matchingType==1){
        cv::Ptr<cv::ORB> orb_detector = cv::ORB::create(thresh);
        orb_detector->detectAndCompute(scene, cv::noArray(), keypoints, descriptors);
        orb_detector->detectAndCompute(temp, cv::noArray(), keypoints_temp, descriptors_temp);
    }
    if (matchingType==2){
        cv::Ptr<cv::AKAZE> akaze_detector = cv::AKAZE::create();

        akaze_detector->detectAndCompute(scene, cv::noArray(), keypoints, descriptors);
        akaze_detector->detectAndCompute(temp, cv::noArray(), keypoints_temp, descriptors_temp);
    }
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::BRUTEFORCE);
    std::vector<std::vector<cv::DMatch>> knn_matches;
    matcher->knnMatch( descriptors_temp, descriptors, knn_matches, 2 );
    std::vector<cv::DMatch> good_matches;
    const float ratio_thresh = 0.75f;
    for (size_t i=0; i< knn_matches.size(); i++){
        if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
        {
            good_matches.push_back(knn_matches[i][0]);
        }
    }
    cv::Mat result= m_current.clone();
    cv::drawMatches( temp, keypoints_temp, scene, keypoints, good_matches, result, cv::Scalar::all(-1),
                    cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    std::vector<cv::Point2f> obj_points;
    std::vector<cv::Point2f> scene_points;

    for(const auto& match : good_matches){
        obj_points.push_back(keypoints_temp[match.queryIdx].pt);
        scene_points.push_back(keypoints[match.trainIdx].pt);
    }
    cv::Mat H = cv::findHomography(obj_points, scene_points, cv::RANSAC);

    std::vector<cv::Point2f> obj_corners(4);
    obj_corners[0] = cv::Point2f(0,0);
    obj_corners[1] = cv::Point2f((float)temp.cols,0);
    obj_corners[2] = cv::Point2f((float)temp.cols,(float)temp.rows);
    obj_corners[3] = cv::Point2f(0,(float)temp.rows);

    std::vector<cv::Point2f> scene_corners(4);
    cv::perspectiveTransform(obj_corners, scene_corners, H);
    cv::Point2f offset((float)temp.cols, 0);
    cv::line(result, scene_corners[0] + offset, scene_corners[1] + offset, cv::Scalar(0, 0, 255), 4);
    cv::line(result, scene_corners[1] + offset, scene_corners[2] + offset, cv::Scalar(0, 0, 255), 4);
    cv::line(result, scene_corners[2] + offset, scene_corners[3] + offset, cv::Scalar(0, 0, 255), 4);
    cv::line(result, scene_corners[3] + offset, scene_corners[0] + offset, cv::Scalar(0, 0, 255), 4);
    m_temp = result.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::his_equalization(const int &imgType, const float &scale)
{
    if (m_current.empty()) {return;}

    if (imgType==0){
        cv::Mat gray, equalized, blended;
        cv::cvtColor(m_current, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, equalized);
        cv::addWeighted(equalized, scale, gray, 1-scale, 0, blended);
        m_temp = blended.clone();
        if (m_provider)
            m_provider->updateQImage(m_temp);
    }
    else{
        cv::Mat ycrcb, result;
        cv::cvtColor(m_current, ycrcb, cv::COLOR_BGR2YCrCb);
        std::vector<cv::Mat> channels;
        cv::split(ycrcb, channels);
        cv::equalizeHist(channels[0], channels[0]);
        cv::merge(channels, ycrcb);
        cv::cvtColor(ycrcb, ycrcb, cv::COLOR_YCrCb2BGR);
        cv::addWeighted(ycrcb, scale, m_current, 1 - scale, 0, result);
        m_temp = result.clone();
        if (m_provider)
            m_provider->updateQImage(m_temp);
    }
}

void ImageController::tempMatching(const int &matchingType, const QString &tempPath)
{
    cv::Mat temp = cv::imread(tempPath.toStdString(), cv::IMREAD_COLOR);
    if (temp.empty()) {return;}
    if (m_current.empty()) {return;}
    cv::Mat image = m_current.clone();
    cv::Mat result;
    result.create(image.cols-temp.cols+1,image.rows-temp.rows+1, CV_32FC1);
    cv::matchTemplate(image, temp, result, matchingType);
    cv::normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());
    double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());
    cv::rectangle(image, maxLoc, cv::Point(maxLoc.x + temp.cols , maxLoc.y + temp.rows ), cv::Scalar(0,255,0), 2, 8, 0);
    m_temp = image.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::drawEgde(const int &cannyThres, const int &colorType)
{
    cv::Mat gray;cv::RNG randomColor;
    cv::cvtColor(m_current, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5,5), 1.5);
    cv::Canny(gray, gray, cannyThres, cannyThres*2);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(gray, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    cv::Mat drawing = cv::Mat::zeros(m_current.size(), CV_8UC3);
    for (int i=0; i< contours.size(); i ++){
        cv::Scalar _color =getColor(colorType);
        cv::drawContours(drawing, contours, i, _color, 2, cv::LINE_8, hierarchy, 0);
    }
    m_temp = drawing.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::convexHull(const int &cannyThres, const int &colorType)
{
    cv::Mat gray;
    cv::cvtColor(m_current, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5,5), 1.5);
    cv::Canny(gray, gray, cannyThres, cannyThres*2);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(gray, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    cv::Mat drawing = cv::Mat::zeros(m_current.size(), CV_8UC3);
    std::vector<std::vector<cv::Point>> hull(contours.size());
    for( size_t i = 0; i < contours.size(); i++ )
    {
        cv::convexHull(contours[i], hull[i] );
    }
    for (int i=0; i< contours.size(); i ++){
        cv::Scalar _color =getColor(colorType);
        cv::drawContours(drawing, hull, i, _color, 2, cv::LINE_8, hierarchy, 0);
    }
    m_temp = drawing.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::drawShape(const int &cannyThres, const int &colorType, const int &shapeID)
{
    cv::Mat gray;cv::RNG randomColor;
    cv::cvtColor(m_current, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5,5), 1.5);
    cv::Canny(gray, gray, cannyThres, cannyThres*2);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(gray, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    cv::Mat result = cv::Mat::zeros( m_current.size(), CV_8UC3 );
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
    m_temp = result.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::moment(const int &cannyThres, const int &colorType)
{
    cv::Mat gray;
    cv::cvtColor(m_current, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5,5), 1.5);
    cv::Canny(gray, gray, cannyThres, cannyThres*2);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(gray, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    cv::Mat drawing = cv::Mat::zeros(m_current.size(), CV_8UC3);
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
    m_temp = drawing.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::point_test(const int &cannyThres, const int &contourType)
{
    cv::Mat gray;
    cv::cvtColor(m_current, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5,5), 1.5);
    cv::Canny(gray, gray, cannyThres, cannyThres*2);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(gray, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    cv::Mat drawing = cv::Mat::zeros(m_current.size(), CV_8UC3);
    cv::Mat raw_dist( m_current.size(), CV_32F );
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
    m_temp = drawing.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::iconColor(const QString &path, const int &r, const int &g, const int &b)
{
    cv::Mat img = cv::imread(path.toStdString(), cv::IMREAD_UNCHANGED);
    if (img.channels() < 4) {
        std::cerr << "Image does not have alpha channel!" << std::endl;
    }
    std::vector<cv::Mat> channels;
    cv::split(img, channels);
    cv::Mat blur = channels[0];
    cv::Mat green = channels[1];
    cv::Mat red = channels[2];
    cv::Mat alpha = channels[3];

    cv::Scalar newColor(r,g,b);
    cv::Mat recolored(img.size(), CV_8UC3, newColor);
    cv::Mat output;
    cv::merge(std::vector<cv::Mat>{recolored, alpha}, output);
    m_temp = output.clone();
    if (m_provider)
        m_provider->updateQImage(m_temp);
}

void ImageController::appendImg()
{
    m_current = m_temp.clone();
    history.push_back(m_current);
    if (m_provider)
        m_provider->updateQImage(m_current);
}

void ImageController::refresh()
{
    m_current = m_original.clone();
    history.push_back(m_current);
    if (m_provider)
        m_provider->updateQImage(m_current);
}

