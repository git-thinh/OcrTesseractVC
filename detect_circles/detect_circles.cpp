#include <iostream>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

Mat src;
string fileName, p_file, path_out;
void ___init_file_name() {
	fileName = "hoD2b";
	fileName = "smarties";
	//fileName = "1";
	fileName = "15.1";
	//fileName = "16";
	//fileName = "17";

	//fileName = "3.1";
	//fileName = "1.1";
	fileName = "8.1";
	//fileName = "8.2";
}
void ___load_file(bool hasResize = false) {
	___init_file_name();

	p_file = "C:\\ocr-images\\" + fileName + ".jpg";
	path_out = "C:\\TEMP\\" + fileName + "_";
	src = imread(p_file);

	if (hasResize) {
		// Resize image has max width and height 
		int max = 1366;
		Size matSize = src.size();
		int w = matSize.width;
		int h = matSize.height;

		if (w > max) {
			int h2 = max * h / w;
			resize(src, src, Size(max, h2));
		}
		else if (h > max) {
			int w2 = max * w / h;
			resize(src, src, Size(w2, max));
		}
	}
}
//imwrite(path_out + "1-COLOR_BGR2GRAY.jpg", gray);

/////////////////////////////////////////////////////////////////////////////////////////

Mat src_gray;
Mat dst, detected_edges;

const int max_lowThreshold = 100;
const int kernel_size = 3;
const char* window_name = "Edge Map";

int lowThreshold = 20; //0-100 100 10 20 80
int ratio_ = 3;

void CannyThreshold(int, void*)
{
	int apertureSize = lowThreshold * ratio_;

	//blur(src_gray, detected_edges, Size(1, 1));
	blur(src_gray, detected_edges, Size(3, 3));
	//Canny(detected_edges, detected_edges, lowThreshold, apertureSize, kernel_size);
	Canny(detected_edges, detected_edges, 50, 100, 3);

	dst.create(src.size(), src.type());
	dst = Scalar::all(0);

	src.copyTo(dst, detected_edges);
	imshow(window_name, dst);
}


int main()
{
	___load_file(true);

	cvtColor(src, src_gray, COLOR_BGR2GRAY, 0);

	namedWindow(window_name, WINDOW_AUTOSIZE);
	createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);
	CannyThreshold(0, 0);

	waitKey(0);
	return 0;
}