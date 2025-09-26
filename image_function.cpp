
#include "image_function.h"
#include <cmath>
#include <opencv2/core/mat.hpp>

cv::Mat canny_detection(const cv::Mat& image){
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    cv::Mat blur;
    cv::GaussianBlur(gray, blur, cv::Size(5,5), 1.5);

    cv::Mat canny;
    double low_threshold = 100;
    double high_threshold = low_threshold*3;
    //Canny recommended a upper:lower ratio between 2:1 and 3:1.
    cv::Canny(blur, canny, low_threshold, high_threshold);

    return canny;
}
cv::Mat edge_detection(const cv::Mat& image, const double& area_threshold){
    cv::Mat edges, img;
    edges = canny_detection(image);
    image.copyTo(img);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (const auto& contour:contours){
        double area = cv::contourArea(contour);
        if (area < area_threshold) continue;

        std::vector<cv::Point> approx;
        cv::approxPolyDP(contour, approx, 0.02 * cv::arcLength(contour, true), true);
        cv::drawContours(img, std::vector<std::vector<cv::Point>>{approx}, -1, {0,255,0}, 2);
        std::string shapeType;
        int v = approx.size();
        if (v == 3) shapeType = "Triangle";
        else if (v == 4) {
            cv::Rect r = cv::boundingRect(approx);
            float aspect = (float)r.width / r.height;
            shapeType = (aspect > 0.95 && aspect < 1.05) ? "Square" : "Rectangle";
        }
        else if (v > 6) shapeType = "Circle";

        // Draw label
        cv::putText(img, shapeType, approx[0], cv::FONT_HERSHEY_SIMPLEX, 0.6, {0,0,255}, 2);
    }
    return img;
}


//std::pair<std::vector<cv::KeyPoint>, cv::Mat>
auto orb_detection(const cv::Mat& image,const int& nfeatures){ //input the gray image and the nfeatures
    cv::Ptr<cv::ORB> orb_detector = cv::ORB::create(nfeatures);
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;
    orb_detector->detectAndCompute(image,cv::noArray(), keypoints, descriptors);
    std::cout << "Found " << keypoints.size() << " keypoints" << std::endl;
    std::cout << "Descriptor matrix size: " << descriptors.rows << " x " << descriptors.cols << std::endl;
    cv::Mat img_with_kp;
    cv::drawKeypoints(image, keypoints, img_with_kp, cv::Scalar(0, 255, 0), cv::DrawMatchesFlags::DEFAULT);
    //cv::imshow("ORB Keypoints", img_with_kp);
    return std::make_pair(keypoints, descriptors);
}

auto sift_detection(const cv::Mat& image){
    cv::Ptr<cv::SIFT> sift = cv::SIFT::create();
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;
    sift->detectAndCompute(image, cv::noArray(), keypoints, descriptors);
    return std::make_pair(keypoints, descriptors);
}

cv::Mat image_matching(const cv::Mat& object_img,const cv::Mat& scene_img,const int& nfeatures){ //input gray images

    cv::Mat object, scene;
    cv::cvtColor(object_img, object, cv::COLOR_BGR2GRAY);
    cv::cvtColor(scene_img, scene, cv::COLOR_BGR2GRAY);
    auto object_orb = orb_detection(object, nfeatures);
    auto scene_orb = orb_detection(scene, nfeatures);

    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::BRUTEFORCE_HAMMING);

    std::vector<std::vector<cv::DMatch>> knn_matches;
    matcher->knnMatch(object_orb.second, scene_orb.second, knn_matches, 2);

    std::vector<cv::DMatch> good_matches;
    const float ratio_thresh = 0.75f;
    for (size_t i=0; i< knn_matches.size(); i++){
        good_matches.push_back(knn_matches[i][0]);
    }

    std::cout<< "Found " << good_matches.size() << " good matches" << std::endl;

    // Visualize
    cv::Mat img_matches;
    cv::drawMatches(object, object_orb.first, scene, scene_orb.first, good_matches, img_matches,
                    cv::Scalar::all(-1), cv::Scalar::all(-1), std::vector<char>(),
                    cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    if (good_matches.size() >= 4){
        std::vector<cv::Point2f> obj_points;
        std::vector<cv::Point2f> scene_points;

        for(const auto& match : good_matches){
            obj_points.push_back(object_orb.first[match.queryIdx].pt);
            scene_points.push_back(scene_orb.first[match.trainIdx].pt);
        }
        cv::Mat H = cv::findHomography(obj_points, scene_points, cv::RANSAC);

        std::vector<cv::Point2f> obj_corners(4);
        obj_corners[0] = cv::Point2f(0,0);
        obj_corners[1] = cv::Point2f((float)object.cols,0);
        obj_corners[2] = cv::Point2f((float)object.cols,(float)object.rows);
        obj_corners[3] = cv::Point2f(0,(float)object.rows);

        std::vector<cv::Point2f> scene_corners(4);
        cv::perspectiveTransform(obj_corners, scene_corners, H);
        cv::Point2f offset((float)object.cols, 0);
        cv::line(img_matches, scene_corners[0] + offset, scene_corners[1] + offset, cv::Scalar(0, 0, 255), 4);
        cv::line(img_matches, scene_corners[1] + offset, scene_corners[2] + offset, cv::Scalar(0, 0, 255), 4);
        cv::line(img_matches, scene_corners[2] + offset, scene_corners[3] + offset, cv::Scalar(0, 0, 255), 4);
        cv::line(img_matches, scene_corners[3] + offset, scene_corners[0] + offset, cv::Scalar(0, 0, 255), 4);


    }

    //cv::imshow("Result" , img_matches);
    return img_matches;

}

cv::Mat color_detection(const cv::Mat& object, const double& erea_threshold, const Color& color_){
    cv::Mat img, hsv_img;
    object.copyTo(img);
    cv::cvtColor(object, hsv_img, cv::COLOR_BGR2HSV);
    cv::Mat mask;
    switch(color_){
    case Color::RED:{
        cv::Scalar lower_red1(0, 100, 100), upper_red1(10, 255, 255);
        cv::Scalar lower_red2(160, 100, 100), upper_red2(179, 255, 255);
        cv::Mat mask1, mask2;
        cv::inRange(hsv_img, lower_red1, upper_red1, mask1);
        cv::inRange(hsv_img, lower_red2, upper_red2, mask2);
        cv::bitwise_or(mask1, mask2, mask);
        break;}
    case Color::GREEN:{
        cv::Scalar lower_green(35, 100, 100);
        cv::Scalar upper_green(85, 255, 255);
        cv::inRange(hsv_img, lower_green, upper_green, mask);
        break;}
    case Color::BLUE:{
        cv::Scalar lower_blue(100, 100, 100);
        cv::Scalar upper_blue(130, 255, 255);
        cv::inRange(hsv_img, lower_blue, upper_blue, mask);

        break;}
    case Color::YELLOW:{
        cv::Scalar lower_yellow(20, 100, 100);
        cv::Scalar upper_yellow(30, 255, 255);
        cv::inRange(hsv_img, lower_yellow, upper_yellow, mask);

        break;}
    case Color::BLACK:{
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

cv::Mat sharpen_image(const cv::Mat& image){
    cv::Mat copy;
    image.copyTo(copy);
    for (size_t i=1; i <image.rows-1; i++){
        for (size_t j = 1; j<image.cols-1; j++){
            if (image.at<uchar>(i,j) <= 255){
                uchar value1 = image.at<uchar>(i-1,j);
                uchar value2 = image.at<uchar>(i+1,j);
                uchar value3 = image.at<uchar>(i,j-1);
                uchar value4 = image.at<uchar>(i,j+1);
                copy.at<uchar>(i,j) = image.at<uchar>(i,j)*5 -
                                       (value1+value2+value3+value4);
            }
        }
    }
    return copy;
}
// cv::addWeighted
cv::Mat image_add(const cv::Mat& image1, const cv::Mat& image2, const double& alpha){
    cv::Mat result = cv::Mat::zeros(image1.size(), image1.type());
    for (size_t i=0; i <image1.rows; i++){
        for (size_t j = 0; j<image1.cols; j++){
            result.at<cv::Vec3b>(i, j) = (1-alpha)*image1.at<cv::Vec3b>(i,j)+alpha*image2.at<cv::Vec3b>(i,j);
            //result.at<cv::Vec3b>(i, j)[1] = (1-alpha)*image1.at<cv::Vec3b>(i,j)[1]+alpha*image2.at<cv::Vec3b>(i,j)[1];
            //result.at<cv::Vec3b>(i, j)[2] = (1-alpha)*image1.at<cv::Vec3b>(i,j)[2]+alpha*image2.at<cv::Vec3b>(i,j)[2];
        }
    }
    return result;
}

cv::Mat image_change(const cv::Mat& image, const double& alpha, const int& beta){
    cv::Mat result = cv::Mat::zeros(image.size(), image.type());
    for (size_t i=0; i <image.rows; i++){
        for (size_t j = 0; j<image.cols; j++){
            for (size_t c=0; c< image.channels(); c++){
                result.at<cv::Vec3b>(i, j)[c] = cv::saturate_cast<uchar>(alpha*image.at<cv::Vec3b>(i,j)[c]+beta);
            }
        }
    }
    return result;
}

cv::Mat gammaCorrection(const cv::Mat& img,const double& gamma){
    CV_Assert(gamma>=0);
    cv::Mat lookUpTable(1, 256, CV_8U);
    uchar* p = lookUpTable.ptr();

    for (int i=0; i<256; ++i){
        p[i] = cv::saturate_cast<uchar>(pow(i/255.0, gamma)*255.0);
    }
    cv::Mat res;
    cv::LUT(img, lookUpTable, res);
    return res;
}


cv::Mat sober_filter_3x3(const cv::Mat& image){
    cv::Mat gray, grad;
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::Mat horizontal = (cv::Mat_<float>(3,3) <<
                              -1, 0, 1,
                          -2, 0, 2,
                          -1, 0, 1);
    cv::Mat vertical = (cv::Mat_<float>(3,3) <<
                            -1, -2, -1,
                        0, 0, 0,
                        1, 2, 1);
    cv::filter2D(gray, grad_x, -1, horizontal, cv::Point(-1,-1));
    cv::filter2D(gray, grad_y, -1, vertical, cv::Point(-1,-1));
    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
    return grad;
}
cv::Mat sober_filter_5x5(const cv::Mat& image){
    cv::Mat gray, grad;
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::Mat horizontal = (cv::Mat_<float>(5,5) <<
                              -3, -1, 0, 1, 3,
                          -10, -2, 0, 2, 10,
                          -10, -2, 0, 2, 10,
                          -10, -2, 0, 2, 10,
                          -3, -1, 0, 1, 3);
    cv::Mat vertical = (cv::Mat_<float>(5,5) <<
                            -3, -10, -10, -10, -3,
                        -1, -2, -2, -2, -1,
                        0, 0, 0, 0, 0,
                        1, 2, 2, 2, 1,
                        3, 10, 10, 10, 3);
    cv::filter2D(gray, grad_x, -1, horizontal, cv::Point(-1,-1));
    cv::filter2D(gray, grad_y, -1, vertical, cv::Point(-1,-1));
    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
    return grad;
}
cv::Mat scharr_filter_3x3(const cv::Mat& image){
    cv::Mat gray, grad;
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::Mat horizontal = (cv::Mat_<float>(3,3) <<
                              -3, 0, 3,
                          -10, 0, 10,
                          -3, 0, 3);
    cv::Mat vertical = (cv::Mat_<float>(3,3) <<
                            -3, -10, -3,
                        0, 0, 0,
                        3, 10, 3);
    cv::filter2D(gray, grad_x, -1, horizontal, cv::Point(-1,-1));
    cv::filter2D(gray, grad_y, -1, vertical, cv::Point(-1,-1));
    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
    return grad;
}


cv::Mat hough_line(const cv::Mat& image){
    cv::Mat canny = canny_detection(image);
    cv::Mat std_hl, pos_hl;
    cv::cvtColor(canny, std_hl, cv::COLOR_GRAY2BGR);
    pos_hl = std_hl.clone();
    std::cout << "isgood" << std::endl;
    std::vector<cv::Vec2f> lines;
    cv::HoughLines(canny, lines, 1, CV_PI/180, 150, 0, 0);

    for (size_t i=0; i< lines.size(); i++){
        float rho = lines[i][0], theta = lines[i][1];
        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        cv::line(std_hl,  pt1, pt2, cv::Scalar(0,0,255), 1, cv::LINE_AA);
    }

    std::vector<cv::Vec4i> linesP;
    cv::HoughLinesP(canny, linesP, 1, CV_PI/180, 50, 50, 10);

    for (size_t i=0; i< linesP.size(); i++){
        cv::Vec4i l = linesP[i];
        cv::line(pos_hl, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 1, cv::LINE_AA);
    }

    imshow("Source", image);
    imshow("Detected Lines (in red) - Standard Hough Line Transform", std_hl);
    imshow("Detected Lines (in red) - Probabilistic Line Transform", pos_hl);
    return pos_hl;
}

cv::Mat hough_circle(const cv::Mat& image){
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    std::vector<cv::Vec3f> circles;
    cv::medianBlur(gray, gray, 5);

    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1,
                     gray.rows/16,
                     100, 30, 1, 300);
    for (size_t i=0; i< circles.size(); i++){
        cv::circle(image, cv::Point(circles[i][0], circles[i][1]), circles[i][2],
                   cv::Scalar(0,255,0), 3, cv::LINE_AA);
        std::cout << "hhh" << std::endl;
    }
    //cv::imshow("Image", image);
    return image;
}


cv::Mat hough_detect(const cv::Mat& image, const cv::Mat& templ){
    cv::Mat gray, templ_gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(templ, templ_gray, cv::COLOR_BGR2GRAY);
    if (templ_gray.type() != CV_8UC1) {
        templ_gray.convertTo(templ_gray, CV_8UC1);
    }
    std::vector<cv::Vec4f> positionBallard, positionGuil;

    int templ_w = templ.cols;
    int templ_h = templ.rows;
    std::cout << "->Loaded image! " << std::endl;

    cv::Ptr<cv::GeneralizedHoughBallard> ballard = cv::createGeneralizedHoughBallard();
    ballard->setMinDist(10);
    ballard->setLevels(360);
    ballard->setDp(2);
    ballard->setMaxBufferSize(1000);
    ballard->setVotesThreshold(40);

    ballard->setCannyLowThresh(30);
    ballard->setCannyHighThresh(110);
    ballard->setTemplate(templ_gray);
    std::cout << "->Created Generalized Hough Ballard" << std::endl;

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
    guil->setTemplate(templ_gray);
    std::cout << "->Created Generalized Hough Guil" << std::endl;

    std::cout << "Ballard detecting.." << std::endl;
    ballard->detect(gray, positionBallard);
    std::cout << "->Detected" << std::endl;
    std::cout << "Guil detecting.." << std::endl;
    guil->detect(gray, positionGuil);
    std::cout << "->Detected" << std::endl;
    for (auto iter = positionBallard.begin(); iter!=positionBallard.end(); iter++){
        cv::RotatedRect rrect = cv::RotatedRect(cv::Point2f((*iter)[0],(*iter)[1]),
                                                cv::Size2f(templ_w*(*iter)[2], templ_h*(*iter)[2]), (*iter)[3]);
        cv::Point2f vertices[4];
        rrect.points(vertices);

        for (int i=0; i<4; i++){
            cv::line(image, vertices[i], vertices[(i+1)%4], cv::Scalar(0,255,0), 2);
        }
    }
    for (auto iter = positionGuil.begin(); iter!=positionGuil.end(); iter++){
        cv::RotatedRect rrect = cv::RotatedRect(cv::Point2f((*iter)[0],(*iter)[1]),
                                                cv::Size2f(templ_w*(*iter)[2], templ_h*(*iter)[2]), (*iter)[3]);
        cv::Point2f vertices[4];
        rrect.points(vertices);

        for (int i=0; i<4; i++){
            cv::line(image, vertices[i], vertices[(i+1)%4], cv::Scalar(255,0,0), 2);
        }
    }
    return image;
}

cv::Mat warp_(const cv::Mat& image){
    cv::Point2f srcTri[3];
    srcTri[0] = cv::Point2f(0.f,0.f);
    srcTri[1] = cv::Point2f(image.cols - 1.f,0.f);
    srcTri[2] = cv::Point2f(0.f,image.rows - 1.f);

    cv::Point2f dstTri[3];
    dstTri[0] = cv::Point2f(0.f,image.cols*0.33f);
    dstTri[1] = cv::Point2f(image.cols*0.85f ,image.cols*0.25f);
    dstTri[2] = cv::Point2f(image.cols*0.15f ,image.cols*0.7f);

    cv::Mat warp_mat = cv::getAffineTransform(srcTri, dstTri);
    std::cout << warp_mat << std::endl;
    cv::Mat warp_dst = cv::Mat::zeros(image.size(), image.type());
    cv::warpAffine(image, warp_dst, warp_mat, warp_dst.size());
    return warp_dst;
}
cv::Mat rotation_(const cv::Mat& image, const double& angle, const double& scale){
    cv::Point center = cv::Point(image.cols/2, image.rows/2);
    cv::Mat rotate_mat = cv::getRotationMatrix2D(center, angle, scale);
    std::cout << rotate_mat << std::endl;
    cv::Mat rotate_dst = cv::Mat::zeros(image.size(), image.type());
    cv::warpAffine(image, rotate_dst, rotate_mat, rotate_dst.size());
    return rotate_dst;
}

cv::Mat changeIcon(const std::string& data_path){
    cv::Mat img = cv::imread(image_path+data_path, cv::IMREAD_UNCHANGED);
    if (img.channels() < 4) {
        std::cerr << "Image does not have alpha channel!" << std::endl;
    }
    std::vector<cv::Mat> channels;
    cv::split(img, channels);
    cv::Mat b = channels[0];
    cv::Mat g = channels[1];
    cv::Mat r = channels[2];
    cv::Mat alpha = channels[3];

    cv::Scalar newColor(255, 255, 255);
    cv::Mat recolored(img.size(), CV_8UC3, newColor);
    cv::Mat output;
    cv::merge(std::vector<cv::Mat>{recolored, alpha}, output);
    return output;
}

