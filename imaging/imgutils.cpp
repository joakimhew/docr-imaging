//
//  imgutils.cpp
//  imaging
//
//  Created by Joakim Hansson on 2017-07-02.
//  Copyright © 2017 docr. All rights reserved.
//

#include "imgutils.hpp"
#include <vector>
#include <climits>
#include <opencv2/highgui/highgui.hpp>

static double angle( cv::Point pt1, cv::Point pt2, cv::Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

int thresh = 50, N = 1;

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
    
    cv::blur(src, blurred, cv::Size(2, 2));
    cv::cvtColor(blurred, gray, cv::COLOR_BGR2GRAY);
    cv::Canny(gray, cannyOutput, firstThreshold, secondThreshold);
};

std::vector<cv::Point> ImgUtils::getLargestSquare(std::vector<std::vector<cv::Point> > &squares)
{
    int largestSquare = 0, largestSquareIndex = 0;
    
    for(int i = 0; i < squares.size(); i++)
    {
        double area = cv::contourArea(squares[i]);
        
        if(area > largestSquare)
        {
            largestSquare = area;
            largestSquareIndex = i;
        }
    }
    
    return squares[largestSquareIndex];
}

void orderPoints(std::vector<cv::Point> &points)
{
    
}


void ImgUtils::transform(cv::Mat& src, cv::Mat& dst, std::vector<cv::Point> &orgPerspective)
{
    if(orgPerspective.size() == 0)
        return;
    
    
    cv::RotatedRect box = cv::minAreaRect(cv::Mat(orgPerspective));
    cv::Point2f pts[4];
    
    box.points(pts);
    
    cv::Point2f src_vertices[3];
    src_vertices[0] = pts[0];
    src_vertices[1] = pts[2];
    src_vertices[2] = pts[1];
    src_vertices[3] = pts[3];
    
    cv::Point2f dst_vertices[3];
    dst_vertices[0] = cv::Point(0, 0);
    dst_vertices[1] = cv::Point(box.boundingRect().width-1, 0); // Bug was: had mistakenly switched these 2 parameters
    dst_vertices[2] = cv::Point(0, box.boundingRect().height-1);
    dst_vertices[3] = cv::Point(box.boundingRect().width-1, box.boundingRect().height-1);
    
    
    cv::Mat warpAffineMatrix = cv::getAffineTransform(src_vertices, dst_vertices);
    
    cv::Mat rotated;
    cv::Size size(box.boundingRect().width, box.boundingRect().height);
    cv::warpAffine(src, rotated, warpAffineMatrix, size, CV_INTER_LINEAR, cv::BORDER_CONSTANT);
    
    cv::imshow("", rotated);
//
//    cv::Rect boundRect = cv::boundingRect(orgPerspective);
//    
//    quad_pts.push_back(cv::Point2f(orgPerspective[0].x,orgPerspective[0].y));
//    quad_pts.push_back(cv::Point2f(orgPerspective[1].x,orgPerspective[1].y));
//    quad_pts.push_back(cv::Point2f(orgPerspective[3].x,orgPerspective[3].y));
//    quad_pts.push_back(cv::Point2f(orgPerspective[2].x,orgPerspective[2].y));
//    squre_pts.push_back(cv::Point2f(boundRect.x,boundRect.y));
//    squre_pts.push_back(cv::Point2f(boundRect.x,boundRect.y+boundRect.height));
//    squre_pts.push_back(cv::Point2f(boundRect.x+boundRect.width,boundRect.y));
//    squre_pts.push_back(cv::Point2f(boundRect.x+boundRect.width,boundRect.y+boundRect.height));
//
//    cv::Mat transmtx = cv::getPerspectiveTransform(quad_pts,squre_pts);
//    cv::Mat transformed = cv::Mat::zeros(src.rows, src.cols, CV_8UC3);
//    cv::warpPerspective(src, transformed, transmtx, src.size());
//    
//    cv::imshow("", transformed);
}


void ImgUtils::drawSquare(cv::Mat &image, std::vector<cv::Point> &square)
{
    const cv::Point* p = &square[0];
    int n = (int)square.size();
    
    cv::polylines(image, &p, &n, 1, true, cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
    cv::imshow("square", image);
}

void ImgUtils::drawSquares(cv::Mat& image, const std::vector<std::vector<cv::Point> >& squares )
{
    for( size_t i = 0; i < squares.size(); i++ )
    {
        const cv::Point* p = &squares[i][0];
        int n = (int)squares[i].size();
        polylines(image, &p, &n, 1, true, cv::Scalar(0,255,0), 3, cv::LINE_AA);
    }
    
    cv::imshow("squares", image);
}


void ImgUtils::findSquares(cv::Mat &image, std::vector<std::vector<cv::Point> > &squares)
{
    squares.clear();
    
    cv::Mat pyr, timg, gray0(image.size(), CV_8U), gray;
    
    // down-scale and upscale the image to filter out the noise
    cv::pyrDown(image, pyr, cv::Size(image.cols/2, image.rows/2));
    cv::pyrUp(pyr, timg, image.size());
    
    std::vector<std::vector<cv::Point> > contours;
    
    // find squares in every color plane of the image
    for(int c = 0; c < 3; c++)
    {
        int ch[] = {c, 0};
        cv::mixChannels(&timg, 1, &gray0, 1, ch, 1);
        
        // try for several threshold levels
        for(int l = 0; l < N; l++)
        {
            // hack: use Canny instead of zero threshold level.
            // Canny helps to catch squares with gradient shading
            if(l == 0)
            {
                cv::Mat blurred;
                
                cv::blur(gray0, blurred, cv::Size(6, 6));
                
                // apply Canny. Take the upper threshold from slider
                // and set the lower to 0 (which forces edges merging)
                cv::Canny(blurred, gray, 0, thresh, 5);
                
                // dilate canny output to remove potential
                // holes between edge segments
                //TODO: Do we really want to dilate the image? Check further David :3
                cv::dilate(gray, gray, cv::Mat(), cv::Point(-1, -1));
            }
            else
            {
                // apply threshold if l!=0
                //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
                gray = gray0 >= (l+1)*255/N;
            }
            
            // find contours and store them all as a list
            findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
            
            std::vector<cv::Point> approx;
            
            // test each contour
            for( size_t i = 0; i < contours.size(); i++ )
            {
                // approximate contour with accuracy proportional
                // to the contour perimeter
                approxPolyDP(cv::Mat(contours[i]), approx, arcLength(cv::Mat(contours[i]), true)*0.02, true);
                
                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                if( approx.size() == 4 &&
                   fabs(contourArea(cv::Mat(approx))) > 1000 &&
                   isContourConvex(cv::Mat(approx)) )
                {
                    double maxCosine = 0;
                    
                    for( int j = 2; j < 5; j++ )
                    {
                        // find the maximum cosine of the angle between joint edges
                        double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }
                    
                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
                    if( maxCosine < 0.3 )
                        squares.push_back(approx);
                }
            }
        }
    }
}

