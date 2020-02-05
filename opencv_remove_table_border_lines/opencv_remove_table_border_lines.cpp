#include <iostream>

#include <opencv2/text.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace cv::text;
using namespace std;

string fileName, p_file, path_out;
int main()
{
    bool isWriteFile = true;

    fileName = "IYecc";
    //fileName = "1";
    //fileName = "1.1";
    //fileName = "8.1";
    ////fileName = "8.2";
    p_file = "C:\\ocr-images\\" + fileName + ".jpg";
    path_out = "C:\\TEMP\\" + fileName + "_";

    Mat gray;
    Mat src = imread(p_file);
    cvtColor(src, gray, COLOR_BGR2GRAY);
    //if(isWriteFile) imwrite(path_out + "1-COLOR_BGR2GRAY.jpg", gray);

    //----------------------------------------------------------------------------------------
    // Apply adaptiveThreshold at the bitwise_not of gray, notice the ~ symbol
    Mat bw;
    adaptiveThreshold(~gray, bw, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
    if (isWriteFile) imwrite(path_out + "2-adaptiveThreshold.jpg", bw);

    // Create the images that will use to extract the horizontal and vertical lines
    Mat horizontal = bw.clone();
    Mat vertical = bw.clone();

    //---------------------------------------------------------------------------------------- 
    int scale = 20; // change this variable in order to increase/decrease the amount of lines to be detected

    // Specify size on horizontal axis
    int horizontalsize = horizontal.cols / scale;
    // Create structure element for extracting horizontal lines through morphology operations
    Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(horizontalsize, 1));
    // Apply morphology operations, erosion removes white noises, but it also shrinks our object
    //So we dilate it, since noise is gone, they won't come back, but our object area increases
    erode(horizontal, horizontal, horizontalStructure, Point(-1, -1));
    dilate(horizontal, horizontal, horizontalStructure, Point(-1, -1));
    //if (isWriteFile) imwrite(path_out + "3-horizontal.jpg", horizontal);

    //---------------------------------------------------------------------------------------- 
    // Specify size on vertical axis
    int verticalsize = vertical.rows / scale;
    // Create structure element for extracting vertical lines through morphology operations
    Mat verticalStructure = getStructuringElement(MORPH_RECT, Size(1, verticalsize));
    // Apply morphology operations
    erode(vertical, vertical, verticalStructure, Point(-1, -1));
    dilate(vertical, vertical, verticalStructure, Point(-1, -1));
    //if (isWriteFile) imwrite(path_out + "4-vertical.jpg", vertical);

    //---------------------------------------------------------------------------------------- 
    // create a mask which includes the tables
    Mat mask = horizontal + vertical;

    // find the joints between the lines of the tables, we will use this information in order
    // to descriminate tables from pictures
    Mat joints;
    bitwise_and(horizontal, vertical, joints);
    //if (isWriteFile) imwrite(path_out + "5-hor-ver.jpg", joints);

    // Find external contours from the mask, which most probably will belong to tables or to images
    vector<Vec4i> hierarchy;
    vector<vector<cv::Point> > contours; 
    cv::findContours(mask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
    

    //---------------------------------------------------------------------------------------- 
    vector<vector<Point> > contours_poly(contours.size());
    vector<Rect> boundRect(contours.size());
    vector<Mat> rois;

    ////for (size_t i = 0; i < contours.size(); i++) {

    ////    // find the area of each contour
    ////    double area = contourArea(contours[i]);

    ////    // filter individual lines of blobs that might exist and they do not represent a table
    ////    if (area < 100) // value is randomly chosen, you will need to find that by yourself with trial and error procedure
    ////        continue;

    ////    approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
    ////    boundRect[i] = boundingRect(Mat(contours_poly[i]));

    ////    // find the number of joints that each table has
    ////    Mat roi = joints(boundRect[i]);

    ////    vector<vector<Point> > joints_contours;
    ////    findContours(roi, joints_contours, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

    ////    // if the number is not more than 5 then most likely it not a table
    ////    if (joints_contours.size() <= 4)
    ////        continue;

    ////    rois.push_back(src(boundRect[i]).clone());

    ////    ////drawContours( rsz, contours, i, Scalar(0, 0, 255), CV_FILLED, 8, vector<Vec4i>(), 0, Point() );
    ////    rectangle(src, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 1, 8, 0);
    ////}

    ////for (size_t i = 0; i < rois.size(); ++i) {
    ////    imshow("roi", rois[i]);
    ////    //endToEndSceneTextDetectionAndRecognition(rois[i]);
    ////    waitKey();
    ////}

    //waitKey();
    return 0;
}