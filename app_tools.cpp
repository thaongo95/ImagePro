#include "app_tools.h"
#include <iostream>



void imgInfo(const cv::Mat& img){
    std::cout << "Dimension: (cols and rows): " << img.cols << "x" << img.rows << std::endl;
    std::cout << "Size [W x H]:               " << img.size() << std::endl;
    std::cout << "Channels:                   " << img.channels() << std::endl;
    std::cout << "Mat type (enum):            " << img.type() << std::endl;
    std::cout << "Total pixels:               " << img.total() << std::endl;
    std::cout << "Is continuous:              " << (img.isContinuous() ? "Yes" : "No") << std::endl;
    std::cout << "Step (byte per row):        " << img.step << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;

}


cv::Mat read_img(const std::string& name){
    cv::Mat img = cv::imread(image_path+name, cv::IMREAD_COLOR);
    if (img.empty()) {
        std::cout << "Could not read the image: " << image_path << std::endl;
    }
    return img;
}
void save_img(const std::string& name, const cv::Mat& image){
    bool success_ = cv::imwrite(save_image_path+name, image);
    if (success_) {
        std::cout << "Save image " << name << " to the path:  \"" << save_image_path <<  "\"" << std::endl;
    }
    else {
        std::cout << "Fail to save image" << std::endl;
    }
}

cv::Mat flip(const cv::Mat& image, const std::string& type){
    cv::Mat result = image.clone();
    if (type == "horizontal"){
        for (size_t i = 0; i<image.rows; i++){
            for(size_t j=0; j< image.cols;j++){
                for (size_t c=0; c< image.channels(); c++){
                    result.at<cv::Vec3b>(i,j)[c] = image.at<cv::Vec3b>(image.rows-i-1,j)[c];
                }

            }
        }
    }
    else if (type == "vertical"){
        for (size_t i = 0; i<image.rows; i++){
            for(size_t j=0; j< image.cols;j++){
                result.at<cv::Vec3b>(i,j) = image.at<cv::Vec3b>(i,image.cols-j-1);
            }
        }
    }
    return result;
}
cv::Mat draw_shape(const cv::Mat& image){
    cv::Mat result = image.clone();
    cv::circle(result, cv::Point(result.cols/2, result.rows/2), 10, cv::Scalar(0, 255, 255), cv::FILLED, cv::LINE_8);
    cv::circle(result, cv::Point(result.cols/2, result.rows/2), 50, cv::Scalar(0, 255, 255), 2.0, cv::LINE_8);
    cv::ellipse(result, cv::Point(result.cols/2, result.rows/2),cv::Size(50,100), 45, 0, 360, cv::Scalar(0, 0, 255), 2.0, cv::LINE_AA);
    cv::ellipse(result, cv::Point(result.cols/2, result.rows/2),cv::Size(50,100), 90, 0, 360, cv::Scalar(0, 0, 255), 2.0, cv::LINE_AA);
    cv::ellipse(result, cv::Point(result.cols/2, result.rows/2),cv::Size(50,100), 135, 0, 360, cv::Scalar(0, 0, 255), 2.0, cv::LINE_AA);
    cv::ellipse(result, cv::Point(result.cols/2, result.rows/2),cv::Size(50,100), 180, 0, 360, cv::Scalar(0, 0, 255), 2.0, cv::LINE_AA);
    return result;
}
