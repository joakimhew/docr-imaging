//
//  imgutils.cpp
//  imaging
//
//  Created by Joakim Hansson on 2017-07-02.
//  Copyright © 2017 docr. All rights reserved.
//

#include "imgutils.hpp"

void ImgUtils::resize(cv::Mat &src, cv::Mat &dst, double width, double height) {
    if (width == 0 && height == 0) {
        dst = src;
        return;
    }
    
    double ratio;
    cv::Size size;
    
    if (width == 0) {
        ratio = height / src.size().height;
        double newWidth = src.size().width * ratio;
        
        size = cv::Size(newWidth, height);
    }
    
    else {
        ratio = width / src.size().width;
        double newHeight = src.size().height * ratio;
        
        size = cv::Size(width, newHeight);
    }
    
    cv::resize(src, dst, size);
    return;
    
};

void ImgUtils::findEdges(cv::Mat &src, cv::Mat &cannyOutput, double firstThreshold, double secondThreshold){
    cv::Mat blurred;
    cv::Mat gray;
    
    cv::blur(src, blurred, cv::Size(3, 3));
    cv::cvtColor(blurred, gray, cv::COLOR_BGR2GRAY);
    cv::Canny(gray, cannyOutput, firstThreshold, secondThreshold);
};

