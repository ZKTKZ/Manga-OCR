#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
//#include <ccv.h>

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <omp.h>

using std::cout;
using std::endl;
using std::vector;
using std::string;

#define SHELLSCRIPT "\
#/bin/bash \n\
"

//tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();

vector<cv::Rect> detectLetters(cv::Mat img, std::string img_name)
{

        //std::ofstream uzn_file(img_name + ".uzn", std::ofstream::out);
        int img_area = img.cols * img.rows;
        vector<cv::Rect> boundingBoxes;
        cv::Mat img_gray, img_sobel, img_threshold, kernel;
        cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
        
        cv::SimpleBlobDetector::Params params;
        params.filterByColor = 0;
        params.filterByConvexity = 0;
        params.filterByCircularity = 0;
        params.filterByInertia = 0;
        params.filterByArea = 1;
        params.minArea = 1000 * 5;
        params.maxArea = 100000;
        params.minDistBetweenBlobs = 10 * 5;

        cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);
        vector<cv::KeyPoint> keypoints;
        detector->detect( img_gray, keypoints);
 
        cv::Mat img_blurred;
        cv::GaussianBlur(img_gray, img_blurred, cv::Size(9, 9), 0, 0, cv::BORDER_DEFAULT);
        cv::Mat img_usm = 4 * img_gray - 2 * img_blurred;
       
        cv::imshow("Unsharp Mask", img_usm); cv::waitKey(0); 
        //cv::imwrite(img_name + "_USM.jpg", img_usm); 
        
/*
        api->Init(NULL, "eng");
        api->SetImage((uchar*)img_usm.data, img_usm.size().width, img_usm.size().height, img_usm.channels(), img_usm.step1());
*/
        for (int i = 0; i < keypoints.size(); i++){
                double x = keypoints[i].pt.x, y = keypoints[i].pt.y;
                auto radius = keypoints[i].size;
              
                /*
                dimensions of inner rect were used to find rad/2.
                len(inner_rect) = radius/sqrt(2)
                */

                double top, left, right, bottom;
                left = x - radius/2;
                right = x + radius/2;
                top = y - radius/2;
                bottom = y + radius/2;

                if(left < 0)
                        left = 0;
                else if(right > img.cols)
                        right = img.cols;
                if(top < 0)
                        top = 0;
                else if(bottom > img.rows)
                        bottom = img.rows;
                printf("Top-left: (%3.0f, %3.0f) ", left, top); 
                printf("Bottom-right: (%3.0f, %3.0f)\n", right, bottom);
                
                auto bbox = cv::Rect(left, top, right - left, bottom - top);
                cv::Mat roi = img(bbox);
                system(SHELLSCRIPT);
                //cv::imwrite(img_name + "/" + std::to_string(i) + ".jpg", roi);
                //cv::imshow(img_name + " " + std::to_string(i), roi); cv::waitKey(0);
        }
        
        cv::Mat img_with_keypoints;
        drawKeypoints( img, keypoints, img_with_keypoints, cv::Scalar(0,0,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
        //cv::imshow("keypoints.jpg", img_with_keypoints ); cv::waitKey(0);
        
        return boundingBoxes;
}

int main(int argc,char** argv)
{
        string series_name = "dragon-ball";
        int thisManyChapters = 197;

        for(int i=196; i<thisManyChapters; i++){
                string chapter="";
                if (i+1<10)
                       chapter+="0";
                if (i+1<100)
                       chapter+="0";
                chapter+=std::to_string(i+1);
                string chapter_path = "manga/"+series_name+"/"+series_name+"-"+chapter;
                std::size_t found = chapter_path.find_last_of("/");
                string chapter_name = chapter_path.substr(found+1);
                for(auto& page: std::filesystem::directory_iterator(chapter_path)){
                        string path = page.path();
                        string file_name = path.substr(path.find_last_of("/")+1);
                        cv::Mat img=cv::imread(path);
                        string path_root = path.substr(0, path.find(".jpg"));
                        if (path.find_last_of("jpg") == path.size()-1){ 
                                vector<cv::Rect> letterbboxes=detectLetters(img,path_root);
                                for(int j=0; j<letterbboxes.size(); j++)
                                        cv::rectangle(img,letterbboxes[j],cv::Scalar(0,255,0),3,8,0);
                                //cv::imshow(file_name + "_bb.jpg", img); //cv::waitKey(0);
                                }
                }
        }
        return 0;
}

