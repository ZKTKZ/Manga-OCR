#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

std::vector<cv::Rect> detectLetters(cv::Mat img, std::ofstream& uzn_file)
{
        int img_area = img.cols * img.rows;
        std::vector<cv::Rect> boundingBoxes;
        cv::Mat img_gray, img_sobel, img_threshold, kernel;
        cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
        //cv::imshow("img_gray: ", img_gray); cv::waitKey(0);
        cv::Sobel(img_gray, img_sobel, CV_8U, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
        //cv::imshow("img_sobel: ", img_sobel); cv::waitKey(0);
        cv::threshold(img_sobel, img_threshold, 0, 250, cv::THRESH_OTSU+cv::THRESH_BINARY);
        //cv::imshow("img_threshold: ", img_threshold); cv::waitKey(0);
        int x_axis = 10/2-2;
        int y_axis = 25-10;
        kernel = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(x_axis, y_axis) );
        cv::morphologyEx(img_threshold, img_threshold, cv::MORPH_CLOSE, kernel); //Does the trick
        //cv::imshow("img_threshold" + file_name, img_threshold); cv::waitKey(0);
        std::vector< std::vector< cv::Point> > contours;
        cv::findContours(img_threshold, contours, 0, 1); 
        std::vector<std::vector<cv::Point> > contours_poly( contours.size() );

        //cv::imshow("img_threshold_" + std::to_string(x_axis) + "_" + std::to_string(y_axis) + "_" + file_name + ".jpg", img_threshold);
        //cv::waitKey(0);
        
        for(int i = 0; i < contours.size(); i++)
                if(contours[i].size()>100 && contours[i].size()<10000)
                { 
                        cv::approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 3, true );
                        cv::Rect appRect( boundingRect( cv::Mat(contours_poly[i]) ));
                        int bb_area = (appRect.x - appRect.width) * (appRect.y - appRect.height);   
                        if (bb_area < 0) 
                                bb_area *= -1;  
                        if (bb_area * 100 < 40 * img_area){
                                uzn_file << appRect.x << " ";
                                uzn_file << appRect.y << " "; 
                                uzn_file << appRect.width << " "; 
                                uzn_file << appRect.height << " ";
                                uzn_file << "Text/English\n";
                                boundingBoxes.push_back(appRect);
                        }
                }
        uzn_file.close();
        return boundingBoxes;
}

int main(int argc,char** argv)
{
        std::string series_name = "akame-ga-kiru";
        int thisManyChapters = 2;

        for(int i=0; i<thisManyChapters; i++){
                std::string chapter="";
                if (i+1<10)
                       chapter+="0";
                if (i+1<100)
                       chapter+="0";
                chapter+=std::to_string(i+1);
                std::string chapter_path = series_name+"/"+series_name+"-"+chapter;
                std::size_t found = chapter_path.find_last_of("/");
                std::string chapter_name = chapter_path.substr(found+1);
                //pls try; catch for ".jpg" extension
                for(auto& page: fs::directory_iterator(chapter_path)){
                        std::string path = page.path();
                        std::string file_name = path.substr(path.find_last_of("/")+1);
                        cv::Mat img=cv::imread(path);
                        std::string path_root = path.substr(0, path.find(".jpg"));
                        std::cout << path_root << std::endl;
                        std::ofstream uzn_file(path_root + ".uzn", std::ofstream::out);
                        std::vector<cv::Rect> letterbboxes=detectLetters(img, uzn_file);
                        for(int j=0; j<letterbboxes.size(); j++)
                                cv::rectangle(img,letterbboxes[j],cv::Scalar(0,255,0),3,8,0);
                   //     cv::imshow(file_name + "_bb.jpg", img);
                   //     cv::waitKey(0);
                }
        }
        return 0;
}

/*
   optimizeElement(std::vector<int> lastAreaVector, int lastX, int lastY)
   {
   kernel = ellipticalStructuringElement(x, y)
   morphologyEx(MORPH_CLOSE)
   define array of 2d points - contours
   findContours


   for ith contour
   if contour[i].size > min && contour[i].size < 10000
   approxPolyDP
   compute area 
   push to areaVector
   sort (areaVector)

   if (lastAreaVector.size > this.areaVector.size)
   end = this.areaVector.size
   else
   end = lastAreaVector.size

   int i = 0
   while (lastAreaVector[i] != this.AreaVector[i])
   i++;

   smallerAreaVector = returnSmaller(lastAreaVector[i], this.AreaVector[i])


   optimizeElement(smaller.x+1, smaller.y)
   optimizeElement(smaller.x-1, smaller.y)
   optimizeElement(smaller.x, smaller.y+1)
   optimizeElement(smaller.x, smaller.y-1)

   }

 */
