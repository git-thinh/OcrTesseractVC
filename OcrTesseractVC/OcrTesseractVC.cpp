// OcrTesseractVC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

using namespace tesseract;

class TessAPI : public TessBaseAPI { public: void PrintRects(int len); };

//const char* p_file = "C:\\Git\\OCR\\OCR_Accord.Tesseract\\data-test\\text\\phototest.tif";
const char* p_file = "C:\\ocr-images\\1.1.jpg";

int test_GetUTF8Text_001(const char* file) {

	char* outText;

	tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
	// Initialize tesseract-ocr with English, without specifying tessdata path
	if (api->Init(NULL, "eng")) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}

	// Open input image with leptonica library
	//Pix* image = pixRead("/usr/src/tesseract/testing/phototest.tif");
	//Pix* image = pixRead("C:\\Git\\OCR\\OCR_Accord.Tesseract\\data-test\\text\\phototest.tif");
	Pix* image = pixRead(file);
	//Pix* image = pixRead("C:\ocr-images\1.1.jpg");

	api->SetImage(image);
	// Get OCR result
	outText = api->GetUTF8Text();
	printf("OCR output:\n%s", outText);

	// Destroy used object and release memory
	api->End();
	delete api;
	delete[] outText;
	pixDestroy(&image);

	return 0;
}

int test_GetUTF8Text_002(const char* file) {
	//Pix* image = pixRead(file);
	//const unsigned char* data;
	//int w0;
	//int h0;
	//int bytes_per_pixel;
	//int stride; // bytes_per_line;

	//int x0;
	//int y0;

	////TessAPI* api = new TessAPI();
	////int res = api->Init(NULL, "rus");
	//////api->SetAccuracyVSpeed(AVS_MOST_ACCURATE);
	////api->SetImage(data, w0, h0, bytes_per_pixel, stride);
	////api->SetRectangle(x0, y0, w0, h0);

	/////*char* text;
	////ETEXT_DESC monitor;
	////api->RecognizeForChopTest(&monitor);
	////text = api->GetUTF8Text();

	////printf("text: %s\n", text);
	////printf("m.count: %s\n", monitor.count);
	////printf("m.progress: %s\n", monitor.progress);*/

	/*api->RecognizeForChopTest(&monitor);
	text = api->GetUTF8Text();
	printf("text: %s\n", text);*/

	////api->End();

	return 0;
}

int test_box_GetComponentImages_001(const char* file) {
	Pix* image = pixRead(file);

	tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
	//api->Init(NULL, "eng");
	// Initialize tesseract-ocr with English, without specifying tessdata path
	if (api->Init(NULL, "eng")) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}

	api->SetImage(image);

	Boxa* boxes = api->GetComponentImages(tesseract::RIL_TEXTLINE, true, NULL, NULL);
	printf("Found %d textline image components.\n", boxes->n);
	for (int i = 0; i < boxes->n; i++) {
		BOX* box = boxaGetBox(boxes, i, L_CLONE);
		api->SetRectangle(box->x, box->y, box->w, box->h);
		char* ocrResult = api->GetUTF8Text();
		int conf = api->MeanTextConf();
		fprintf(stdout, "Box[%d]: x=%d, y=%d, w=%d, h=%d, confidence: %d, text: %s",
			i, box->x, box->y, box->w, box->h, conf, ocrResult);
		boxDestroy(&box);
	}

	// Destroy used object and release memory
	api->End();
	delete api;
	pixDestroy(&image);

	return 0;
}

int test_box_BoundingBox_001(const char* file) {
	Pix* image = pixRead(file);

	tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
	//api->Init(NULL, "eng");
	// Initialize tesseract-ocr with English, without specifying tessdata path
	if (api->Init(NULL, "eng")) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}

	api->SetImage(image);
	api->Recognize(0);
	tesseract::ResultIterator* ri = api->GetIterator();
	tesseract::PageIteratorLevel level = tesseract::RIL_WORD;
	if (ri != 0) {
		do {
			float conf = ri->Confidence(level);
			int x1, y1, x2, y2;
			ri->BoundingBox(level, &x1, &y1, &x2, &y2);
			//const char* word = ri->GetUTF8Text(level); 
			//printf("word: '%s';  \tconf: %.2f; BoundingBox: %d,%d,%d,%d;\n", word, conf, x1, y1, x2, y2);
			//delete[] word;
			printf("conf: %.2f; BoundingBox: %d,%d,%d,%d;\n", conf, x1, y1, x2, y2);
		} while (ri->Next(level));
	}

	// Destroy used object and release memory
	api->End();
	delete api;
	pixDestroy(&image);

	return 0;
}

int main()
{
	//test_GetUTF8Text_001(p_file); // Ok
	//test_GetUTF8Text_002(p_file); // Error

	test_box_GetComponentImages_001(p_file); // Ok
	//test_box_BoundingBox_001(p_file); // Ok

	return 0;
}

















//int main()
//{
//    std::cout << "Hello World!\n";
//}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
