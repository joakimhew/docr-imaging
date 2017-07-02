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
    void resize(cv::Mat &src, cv::Mat &dst, double width, double height);
    void findEdges(cv::Mat &src, cv::Mat &cannyOutput, double firstThreshold, double secondThreshold);
};

#endif /* imgutils_hpp */
