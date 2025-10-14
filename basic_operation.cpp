#include "basic_operation.h"

cv::Mat exposure(const cv::Mat &image, const double &alpha, const int &beta)
{
    cv::Mat newImage = cv::Mat::zeros(image.size(), image.type());
    // for (int y=0; y<image.rows; y++){
    //     for (int x=0; x< image.cols; x++){
    //         for (int c=0; c< image.channels(); c++){
    //             newImage.at<cv::Vec3b>(y,x)[c] =
    //                 cv::saturate_cast<uchar>(alpha*image.at<cv::Vec3b>(y,x)[c]+beta);
    //         }
    //     }
    // }

    cv::Mat lookupTable(1,256, CV_8U);
    uchar* p = lookupTable.ptr();
    for (int i=0; i< 256; i++)
        p[i] = cv::saturate_cast<uchar>(alpha*i+beta);
    cv::LUT(image, lookupTable, newImage);
    return newImage;
}
cv::Mat gamma_(const cv::Mat& image, const double& gammaValue)
{
    cv::Mat newImage = cv::Mat::zeros(image.size(), image.type());
    // for (int y=0; y<image.rows; y++){
    //     for (int x=0; x< image.cols; x++){
    //         for (int c=0; c< image.channels(); c++){
    //             newImage.at<cv::Vec3b>(y,x)[c] =
    //                 cv::saturate_cast<uchar>(alpha*image.at<cv::Vec3b>(y,x)[c]+beta);
    //         }
    //     }
    // }

    cv::Mat lookupTable(1,256, CV_8U);
    uchar* p = lookupTable.ptr();
    for (int i=0; i< 256; i++)
        p[i] = cv::saturate_cast<uchar>(pow(i/255.0, gammaValue)*255.0);
    cv::LUT(image, lookupTable, newImage);
    return newImage;
}

cv::Mat color_detection(const cv::Mat& object, const double& erea_threshold, const COLOR& color_){
    cv::Mat img, hsv_img;
    object.copyTo(img);
    cv::cvtColor(object, hsv_img, cv::COLOR_BGR2HSV);
    cv::Mat mask;
    switch(color_){
    case COLOR::RED:{
        cv::Scalar lower_red1(0, 100, 100), upper_red1(10, 255, 255);
        cv::Scalar lower_red2(160, 100, 100), upper_red2(179, 255, 255);
        cv::Mat mask1, mask2;
        cv::inRange(hsv_img, lower_red1, upper_red1, mask1);
        cv::inRange(hsv_img, lower_red2, upper_red2, mask2);
        cv::bitwise_or(mask1, mask2, mask);
        break;}
    case COLOR::GREEN:{
        cv::Scalar lower_green(35, 100, 100);
        cv::Scalar upper_green(85, 255, 255);
        cv::inRange(hsv_img, lower_green, upper_green, mask);
        break;}
    case COLOR::BLUE:{
        cv::Scalar lower_blue(100, 100, 100);
        cv::Scalar upper_blue(130, 255, 255);
        cv::inRange(hsv_img, lower_blue, upper_blue, mask);

        break;}
    case COLOR::YELLOW:{
        cv::Scalar lower_yellow(20, 100, 100);
        cv::Scalar upper_yellow(30, 255, 255);
        cv::inRange(hsv_img, lower_yellow, upper_yellow, mask);

        break;}
    case COLOR::BLACK:{
        cv::Scalar lower_black(0, 0, 0);
        cv::Scalar upper_black(180, 255, 50);
        cv::inRange(hsv_img, lower_black, upper_black, mask);
        break;}
    }
    cv::erode(mask, mask, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));
    cv::dilate(mask, mask, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    for (size_t i=0; i< contours.size(); i++){
        double erea = cv::contourArea(contours[i]);
        if (erea > erea_threshold){
            cv::Rect bounding_rect = cv::boundingRect(contours[i]);
            cv::rectangle (img, bounding_rect, cv::Scalar(0,255,0), 2);
        }
    }
    return img;
}

