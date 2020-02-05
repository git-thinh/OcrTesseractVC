//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <iostream>

#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

const char* p_file = "C:\\ocr-images\\1.1.jpg";
//const char* p_file = "C:\\ocr-images\\1.jpg";

int main(int argc, char** argv)
{
   /* if (argc != 2)
    {
        cout << " Usage: thisprogramname.exe ImageToLoadAndDisplay" << endl;
        return -1;
    }*/

    Mat image;
    //image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file
    image = imread(p_file, IMREAD_COLOR);   // Read the file

    if (!image.data)                              // Check for invalid input
    {
        cout << "Could not open or find the image" << std::endl;
        return -1;
    }

    namedWindow("Display window", WINDOW_AUTOSIZE);// Create a window for display.
    imshow("Display window", image);                   // Show our image inside it.

    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}