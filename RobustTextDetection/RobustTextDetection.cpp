#include <stdio.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
//#include <opencv2/xfeatures2d.hpp>
#include <opencv2/features2d.hpp>

using namespace cv;
//using namespace cv::xfeatures2d;

#define MIN_MATCH_COUNT 10

int main()
{
    Mat img_object = imread("./2.PNG", IMREAD_GRAYSCALE);
    Mat img_scene = imread("./1.PNG", IMREAD_GRAYSCALE);

    Ptr<ORB> detector = ORB::create();
    std::vector<KeyPoint> keypoints_object, keypoints_scene;
    Mat descriptors_object, descriptors_scene;
    detector->detectAndCompute(img_object, cv::Mat(), keypoints_object, descriptors_object);
    detector->detectAndCompute(img_scene, cv::Mat(), keypoints_scene, descriptors_scene);


    std::cout << descriptors_scene.row(0) << "\n";
    std::cout << descriptors_scene.cols << "\n";


    std::vector<Point2f> keypoints_scene_points_;
    for (int i = 0; i < keypoints_scene.size(); i++) {
        keypoints_scene_points_.push_back(keypoints_scene[i].pt);
    }
    Mat keypoints_scene_points(keypoints_scene_points_);

    Mat labels;
    int estimate_cluster_count = 3; // estimated ??????????
    kmeans(keypoints_scene_points, estimate_cluster_count, labels, TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0), 3, KMEANS_RANDOM_CENTERS);

    std::cout << "==================================111111\n";

    Mat descriptors_scene_clusters[3] = { Mat(descriptors_scene.rows, descriptors_scene.cols, CV_8U, Scalar(0)) };

    std::cout << "==================================111111------\n";

    for (int i = 0; i < labels.rows; i++) {
        int clusterIndex = labels.at<int>(i);
        Point2f pt = keypoints_scene_points.at<Point2f>(i);
        descriptors_scene_clusters[clusterIndex].at<uchar>(pt) = descriptors_scene.at<uchar>(pt);  // ?????? error
    }

    std::cout << descriptors_scene_clusters[0] << "\n";
    std::cout << "==================================22222222\n";
    // return 0;

    Mat img_matches = img_scene;
    std::vector<DMatch> all_good_matches;
    for (int i = 0; i < estimate_cluster_count; i++) {
        std::cout << "==================================33333\n";

        Ptr<flann::IndexParams> indexParams = makePtr<flann::KDTreeIndexParams>(5);
        Ptr<flann::SearchParams> searchParams = makePtr<flann::SearchParams>(50);
        Ptr<FlannBasedMatcher> matcher = makePtr<FlannBasedMatcher>(indexParams, searchParams);
        // BFMatcher matcher;
        std::vector<std::vector<DMatch>> matches;

        std::cout << "==================================444444\n";
        matcher->knnMatch(descriptors_object, descriptors_scene_clusters[i], matches, 2);
        std::cout << "==================================555555\n";
        std::vector<DMatch> good_matches;

        for (auto& match : matches) {
            if (match[0].distance < 0.7 * match[1].distance) {
                good_matches.push_back(match[0]);
            }
        }

        all_good_matches.insert(all_good_matches.end(), good_matches.begin(), good_matches.end());

        std::cout << "==================================66666\n";

        if (good_matches.size() > MIN_MATCH_COUNT) {

            //-- Localize the object
            std::vector<Point2f> obj;
            std::vector<Point2f> scene;

            for (auto& match : good_matches) {
                //-- Get the keypoints from the good matches
                obj.push_back(keypoints_object[match.queryIdx].pt);
                scene.push_back(keypoints_scene[match.trainIdx].pt);
            }

            Mat H = findHomography(obj, scene, RANSAC);

            //-- Get the corners from the image_1 ( the object to be "detected" )
            std::vector<Point2f> obj_corners(4);
            obj_corners[0] = cv::Point(0, 0);
            obj_corners[1] = cv::Point(img_object.cols, 0);
            obj_corners[2] = cv::Point(img_object.cols, img_object.rows);
            obj_corners[3] = cv::Point(0, img_object.rows);
            std::vector<Point2f> scene_corners(4);

            perspectiveTransform(obj_corners, scene_corners, H);

            //-- Draw lines between the corners (the mapped object in the scene - image_2 )
            line(img_matches, scene_corners[0] + Point2f(img_object.cols, 0),
                scene_corners[1] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
            line(img_matches, scene_corners[1] + Point2f(img_object.cols, 0),
                scene_corners[2] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
            line(img_matches, scene_corners[2] + Point2f(img_object.cols, 0),
                scene_corners[3] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
            line(img_matches, scene_corners[3] + Point2f(img_object.cols, 0),
                scene_corners[0] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);

            print(scene_corners);
        }
    }

    drawMatches(img_object, keypoints_object, img_scene, keypoints_scene,
        all_good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
        std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);


    //-- Show detected matches
    imshow("Good Matches & Object detection", img_matches);

    waitKey(0);
    return 0;
}