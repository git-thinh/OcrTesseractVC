#include <iostream>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int img_type = 1;
Mat mat_gray, mat_src;
string fileName, p_file, path_out;
void ___init_file_name() {
	fileName = "hoD2b";
	//fileName = "smarties";
	fileName = "1";
	//fileName = "15.1";
	fileName = "16";
	//fileName = "17";

	//img_type = 2;
	//fileName = "3.1";
	//fileName = "1.1";
	//fileName = "8.1";
	//fileName = "8.2";
}
void ___load_file(bool hasResize = false, bool hasConvertGray = false) {
	___init_file_name();

	p_file = "C:\\ocr-images\\" + fileName + ".jpg";
	path_out = "C:\\TEMP\\" + fileName + "_";
	mat_src = imread(p_file);

	if (hasResize) {
		// Resize image has max width and height 
		int max = 1366;
		Size matSize = mat_src.size();
		int w = matSize.width;
		int h = matSize.height;

		if (w > max) {
			int h2 = max * h / w;
			resize(mat_src, mat_src, Size(max, h2));
		}
		else if (h > max) {
			int w2 = max * w / h;
			resize(mat_src, mat_src, Size(w2, max));
		}
	}

	if (hasConvertGray) {
		cvtColor(mat_src, mat_gray, COLOR_BGR2GRAY);
	}
}
//imwrite(path_out + "1-COLOR_BGR2GRAY.jpg", gray);

/////////////////////////////////////////////////////////////////////////////////////////

void test_001() {

	medianBlur(mat_gray, mat_gray, 5);

	vector<Vec3f> circles;
	HoughCircles(mat_gray, circles, HOUGH_GRADIENT, 1,
		mat_gray.rows / 16,  // change this value to detect circles with different distances to each other
		100, 30, 1, 30 // change the last two parameters
		// (min_radius & max_radius) to detect larger circles
	);

	for (size_t i = 0; i < circles.size(); i++)
	{
		Vec3i c = circles[i];
		Point center = Point(c[0], c[1]);
		// circle center
		circle(mat_src, center, 1, Scalar(0, 100, 100), 3, LINE_AA);
		// circle outline
		int radius = c[2];
		circle(mat_src, center, radius, Scalar(255, 0, 255), 3, LINE_AA);
	}

	//imshow("detected circles", gray);
	imshow("detected circles", mat_src);

}

/////////////////////////////////////////////////////////////////////////////////////////

const int max_lowThreshold = 100;
const int kernel_size = 3;
const char* window_name = "Edge Map";

int lowThreshold = 20; //0-100 100 10 20 80
int ratio_ = 3;

void CannyThreshold(int, void*)
{
	int apertureSize = lowThreshold * ratio_;

	Mat dst, detected_edges;

	//blur(mat_gray, detected_edges, Size(7, 7));

	if (img_type == 1) {
		//blur(mat_gray, detected_edges, Size(3, 3));
		//Canny(detected_edges, detected_edges, 20, 60, 3);
		Canny(mat_gray, detected_edges, lowThreshold, apertureSize, kernel_size);
	}
	else if (img_type == 2) {
		blur(mat_gray, detected_edges, Size(3, 3));
		Canny(detected_edges, detected_edges, 50, 100, 3);
	}

	dst.create(mat_src.size(), mat_src.type());
	dst = Scalar::all(0);

	mat_src.copyTo(dst, detected_edges);

	//-------------------------------------------------

	//medianBlur(dst, dst, 3); 

	//-------------------------------------------------

	imshow(window_name, dst);
}

void test_002() {
	namedWindow(window_name, WINDOW_AUTOSIZE);
	createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);
	CannyThreshold(0, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	___load_file(true, true);

	//test_001();
	test_002();

	waitKey();
	return 0;
}