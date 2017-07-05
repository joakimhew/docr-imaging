//
//  imgutils.hpp
//  imaging
//
//  Created by Joakim Hansson on 2017-07-02.
//  Copyright © 2017 docr. All rights reserved.
//

#ifndef imgutils_hpp
#define imgutils_hpp

#include <stdio.h>
#include <opencv2/imgproc/imgproc.hpp>

class ImgUtils
{
public:
    void findEdges(cv::Mat &src, cv::Mat &cannyOutput, double firstThreshold, double secondThreshold);
    void resize(cv::Mat &src, cv::Mat &dst, double width, double height);
    void transform(cv::Mat& src, cv::Mat& dst, std::vector<cv::Point> &orgPerspective);
    static void findSquares(cv::Mat& image, std::vector<std::vector<cv::Point> >& squares);
    static void drawSquares(cv::Mat& image, const std::vector<std::vector<cv::Point> >& squares );
    static void drawSquare(cv::Mat& image, std::vector<cv::Point>& square);
    static std::vector<cv::Point> getLargestSquare(std::vector<std::vector<cv::Point> >& squares);
};

#endif /* imgutils_hpp */
