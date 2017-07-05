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
    
    static const char* names[] = {
        "../../testdata/document1.jpg" ,"../../testdata/document2.jpg", "../../testdata/document3.jpg", "../../testdata/document4.jpg", "../../testdata/document5.jpg", "../../testdata/document6.jpg", "../../testdata/receipt1.jpg", "../../testdata/receipt2.jpg","../../testdata/receipt3.jpg","../../testdata/receipt4.jpg", 0};
    
    
    ImgUtils imgUtils;
    
    std::vector<std::vector<cv::Point> > squares;

    
    for( int i = 0; names[i] != 0; i++ )
    {
        cv::Mat image = cv::imread(names[i], 1);
        cv::Mat resized;
        std::vector<cv::Point> largestSquare;
        cv::Mat transformed;
        
        imgUtils.resize(image, resized, 0, 400);
        imgUtils.findSquares(resized, squares);
        largestSquare = imgUtils.getLargestSquare(squares);
        imgUtils.drawSquare(resized, largestSquare);
        imgUtils.transform(resized, transformed, largestSquare);
        
        char c = (char)cv::waitKey();
        if( c == 27 )
            break;
    }
    
    return 0;
}
