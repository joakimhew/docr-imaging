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
    static void findSquares(cv::Mat& image, std::vector<std::vector<cv::Point> >& squares);
    
};

#endif /* imgutils_hpp */
