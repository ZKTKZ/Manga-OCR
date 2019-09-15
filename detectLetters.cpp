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


void detectLetters(cv::Mat img, std::string img_name, tesseract::TessBaseAPI *api)
{
        //std::ofstream uzn_file(img_name + ".uzn", std::ofstream::out);
        
        cv::SimpleBlobDetector::Params params;
        params.filterByColor = false;
        params.filterByConvexity = false;
        params.filterByCircularity = false;
        params.filterByInertia = false;
        params.filterByArea = true;
        params.minArea = 1000 * 5;
        params.maxArea = 100000;
        params.minDistBetweenBlobs = 10 * 5;
        
        cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);
        vector<cv::KeyPoint> keypoints;
        detector->detect( img, keypoints);
 
        cv::Mat img_blurred, img_usm, img_resized;
        
        cv::GaussianBlur(img, img_blurred, cv::Size(9, 9), 0, 0, cv::BORDER_DEFAULT);
        img_usm = 4 * img - 2 * img_blurred;
        cv::resize(img_usm, img_resized, cv::Size(), 400.0/img.cols, 400.0/img.rows);
        //cv::imshow("img_resized", img_resized); cv::waitKey(0);
        
        for (int i = 0; i < keypoints.size(); i++){
                float x = keypoints[i].pt.x, y = keypoints[i].pt.y;
                float radius = keypoints[i].size;
              
                //Minimum bounding rect
                float top, left, right, bottom;
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
                
                cv::Rect bbox = cv::Rect(left, top, right - left, bottom - top);
                cv::Mat roi = img(bbox);
                //cv::imshow(img_name + " " + std::to_string(i), roi); cv::waitKey(0);
                //bbox.release();
                //roi.release();
        }
        
        api->SetImage((uchar*)img_resized.data, img_resized.size().width, img_resized.size().height, img_resized.channels(), img_resized.step1());
        char* ocr_text = api->GetUTF8Text();
        std::cout << ocr_text << endl;
       
        /*
        cv::Mat img_with_keypoints;
        drawKeypoints( img, keypoints, img_with_keypoints, cv::Scalar(0,0,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
        cv::imshow("keypoints.jpg", img_with_keypoints ); cv::waitKey(0);
        */
        
        //OpenCV memory realloc 
        img.release();
        //img_gray.release();
        img_blurred.release();
        img_usm.release();
        img_resized.release();
        detector.release();

        //Tesseract mem realloc
        delete ocr_text;
        ocr_text = nullptr;
        api->Clear();
}

int main(int argc,char** argv)
{
        tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
        api->Init(NULL, "eng");
        //api->Set(tesseract::OEM_TESSERACT_ONLY);
        api->SetPageSegMode(tesseract::PSM_SINGLE_BLOCK); 
         
        //OMP_THREAD_LIMIT = 1;
        std::string series_name = "dragon-ball";
        int num_chapters = 197;

        for(int i=196; i<num_chapters; i++){
                std::string chapter="";
                if (i+1<10)
                       chapter+="0";
                if (i+1<100)
                       chapter+="0";
                //ugly af
                chapter+=std::to_string(i+1);
                std::string chapter_path = "manga/"+series_name+"/"+series_name+"-"+chapter;
                std::size_t found = chapter_path.find_last_of("/");
                std::string chapter_name = chapter_path.substr(found+1);
                
                for(auto& page: std::filesystem::directory_iterator(chapter_path)){
                        std::string path = page.path();
                        std::string file_name = path.substr(path.find_last_of("/")+1);
                        std::string path_root = path.substr(0, path.find(".jpg"));
                        if (path.find_last_of("jpg") == path.size()-1){ 
                                cv::Mat img = cv::imread(path, cv::IMREAD_GRAYSCALE); 
                                //bug - when recursively going through subdir,
                                //matches 'true' for '.jpg. for dir folder itself.
                                detectLetters(img,path_root, api);
                                /*
                                for(int j=0; j<letterbboxes.size(); j++)
                                        cv::rectangle(img,letterbboxes[j],cv::Scalar(0,255,0),3,8,0);
                                //cv::imshow(file_name + "_bb.jpg", img); //cv::waitKey(0);
                                */
                                }
                }
        }
        api->End();
        return 0;
}

