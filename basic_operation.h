#ifndef BASIC_OPERATION_H
#define BASIC_OPERATION_H
#include <opencv2/opencv.hpp>
#include <cmath>
enum class COLOR {RED, GREEN, BLUE, YELLOW, BLACK};
cv::Mat exposure(const cv::Mat& image, const double& alpha, const int& beta);
cv::Mat gamma_(const cv::Mat& image, const double& gammaValue);
cv::Mat color_detection(const cv::Mat& object, const double& erea_threshold, const COLOR& color_);

#endif // BASIC_OPERATION_H
