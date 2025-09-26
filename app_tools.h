#ifndef APP_TOOLS_H
#define APP_TOOLS_H

#include <opencv2/opencv.hpp>

const std::string image_path = "assets/images/";
const std::string video_path = "assets/videos/";
const std::string save_image_path = "results/images/";
const std::string save_video_path = "results/videos/";
enum class Color {RED, GREEN, BLUE, YELLOW, BLACK};

void imgInfo(const cv::Mat& img);
cv::Mat read_img(const std::string& name);
void save_img(const std::string& name, const cv::Mat& image);
cv::Mat flip(const cv::Mat& image, const std::string& type);
cv::Mat draw_shape(const cv::Mat& image);

#endif
