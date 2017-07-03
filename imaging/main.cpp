//
//  main.cpp
//  imaging
//
//  Created by Joakim Hansson on 2017-07-03.
//  Copyright © 2017 docr. All rights reserved.
//

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include "imgutils.hpp"




int main(int argc, const char * argv[]) {
    std::vector<std::vector<cv::Point> > squares;
    cv::Mat image = cv::imread(argv[1]);
    
    ImgUtils imgUtils;
    
    imgUtils.findSquares(image, squares);
    
    cv::waitKey();
    return 0;
}
