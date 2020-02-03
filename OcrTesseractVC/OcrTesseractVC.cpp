#include <iostream>
#include <stdlib.h>

/////////////////////////////////////////////////////////////////////////////////////////////
#include <memory>
#include <cstdlib>
#include <restbed>

//using namespace std;
using namespace restbed;
/////////////////////////////////////////////////////////////////////////////////////////////

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
using namespace tesseract;
class TessAPI : public TessBaseAPI { public: void PrintRects(int len); };

/////////////////////////////////////////////////////////////////////////////////////////////

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

int tcp_client_SendText(string text, char* s_ip_address, char* s_port)
{
	WSADATA              wsaData;
	SOCKET               SendingSocket;
	// Server/receiver address
	SOCKADDR_IN          ServerAddr, ThisSenderInfo;
	// Server/receiver port to connect to
	unsigned int         Port = 7171;
	int  RetCode;
	// Be careful with the array bound, provide some checking mechanism...
	char sendbuf[1024] = "This is a test string from sender";
	int BytesSent, nlen;


	// Initialize Winsock version 2.2
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	printf("Client: Winsock DLL status is %s.\n", wsaData.szSystemStatus);


	// Create a new socket to make a client connection.
	// AF_INET = 2, The Internet Protocol version 4 (IPv4) address family, TCP protocol
	SendingSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (SendingSocket == INVALID_SOCKET)
	{
		printf("Client: socket() failed! Error code: %ld\n", WSAGetLastError());
		// Do the clean up
		WSACleanup();
		// Exit with error
		return -1;
	}
	else
		printf("Client: socket() is OK!\n");


	// Set up a SOCKADDR_IN structure that will be used to connect
	// to a listening server on port 5150. For demonstration
	// purposes, let's assume our server's IP address is 127.0.0.1 or localhost

	// IPv4
	ServerAddr.sin_family = AF_INET;

	// Port no.
	ServerAddr.sin_port = htons(Port);

	// The IP address
	PCWSTR ip = (PCWSTR)"127.0.0.1";
	//ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	InetPton(AF_INET, ip, &ServerAddr.sin_addr.s_addr);

	// Make a connection to the server with socket SendingSocket.
	RetCode = connect(SendingSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));

	if (RetCode != 0)
	{
		printf("Client: connect() failed! Error code: %ld\n", WSAGetLastError());
		// Close the socket
		closesocket(SendingSocket);
		// Do the clean up
		WSACleanup();
		// Exit with error
		return -1;
	}
	else
	{
		printf("Client: connect() is OK, got connected...\n");
		printf("Client: Ready for sending and/or receiving data...\n");
	}

	// At this point you can start sending or receiving data on
	// the socket SendingSocket.

	// Some info on the receiver side...
	getsockname(SendingSocket, (SOCKADDR*)&ServerAddr, (int*)sizeof(ServerAddr));

	//printf("Client: Receiver IP(s) used: %s\n", inet_ntoa(ServerAddr.sin_addr));
	printf("Client: Receiver port used: %d\n", htons(ServerAddr.sin_port));

	// Sends some data to server/receiver...
	BytesSent = send(SendingSocket, sendbuf, strlen(sendbuf), 0);

	if (BytesSent == SOCKET_ERROR)
		printf("Client: send() error %ld.\n", WSAGetLastError());
	else
	{
		printf("Client: send() is OK - bytes sent: %ld\n", BytesSent);
		// Some info on this sender side...
		// Allocate the required resources
		memset(&ThisSenderInfo, 0, sizeof(ThisSenderInfo));
		nlen = sizeof(ThisSenderInfo);
		getsockname(SendingSocket, (SOCKADDR*)&ThisSenderInfo, &nlen);

		//printf("Client: Sender IP(s) used: %s\n", inet_ntoa(ThisSenderInfo.sin_addr));
		printf("Client: Sender port used: %d\n", htons(ThisSenderInfo.sin_port));
		printf("Client: Those bytes represent: \"%s\"\n", sendbuf);
	}

	if (shutdown(SendingSocket, SD_SEND) != 0)
		printf("Client: Well, there is something wrong with the shutdown(). The error code : % ld\n", WSAGetLastError());
	else
		printf("Client: shutdown() looks OK...\n");

	// When you are finished sending and receiving data on socket SendingSocket,
	// you should close the socket using the closesocket API. We will
	// describe socket closure later in the chapter.

	if (closesocket(SendingSocket) != 0)
		printf("Client: Cannot close \"SendingSocket\" socket. Error code: %ld\n", WSAGetLastError());
	else
		printf("Client: Closing \"SendingSocket\" socket...\n");

	// When your application is finished handling the connection, call WSACleanup.

	if (WSACleanup() != 0)
		printf("Client: WSACleanup() failed!...\n");
	else
		printf("Client: WSACleanup() is OK...\n");
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

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
	//printf("\nFound %d textline image components.\n", boxes->n);

	string text = "[\n";
	int len = boxes->n;
	for (int i = 0; i < len; i++) {
		BOX* box = boxaGetBox(boxes, i, L_CLONE);
		//api->SetRectangle(box->x, box->y, box->w, box->h);
		//int conf = api->MeanTextConf();		
		//const char* ocrResult = api->GetUTF8Text();
		//fprintf(stdout, "Box[%d]: x=%d, y=%d, w=%d, h=%d, confidence: %d, text: %s", i, box->x, box->y, box->w, box->h, conf, ocrResult);
		//delete[] ocrResult;

		//fprintf(stdout, "\nBox[%d]: x=%d, y=%d, w=%d, h=%d\n", i, box->x, box->y, box->w, box->h);

		text += "[" + std::to_string(i) + "," + std::to_string(box->x) + "," + std::to_string(box->y) + "," + std::to_string(box->w) + "," + std::to_string(box->h) + "]";
		if (i == len - 1)
			text += "\n";
		else
			text += ",\n";

		boxDestroy(&box);
	}

	text += "\n]";

	std::cout << text;

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

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

bool replace___(std::string& str, const std::string& from, const std::string& to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

string line_get_XYWH(string language, string fileName, bool hasText) {
	Pix* image = pixRead(fileName.c_str());

	tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
	//api->Init(NULL, "eng");
	// Initialize tesseract-ocr with English, without specifying tessdata path
	if (api->Init(NULL, language.c_str())) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}

	api->SetImage(image);
	api->SetVariable("lstm_choice_mode", "2");

	Boxa* boxes = api->GetComponentImages(tesseract::RIL_TEXTLINE, true, NULL, NULL);
	//printf("\nFound %d textline image components.\n", boxes->n);

	string text = "[\n";
	int len = boxes->n;
	for (int i = 0; i < len; i++) {
		BOX* box = boxaGetBox(boxes, i, L_CLONE);
		//api->SetRectangle(box->x, box->y, box->w, box->h);
		//int conf = api->MeanTextConf();		
		//const char* ocrResult = api->GetUTF8Text();
		//fprintf(stdout, "Box[%d]: x=%d, y=%d, w=%d, h=%d, confidence: %d, text: %s", i, box->x, box->y, box->w, box->h, conf, ocrResult);
		//delete[] ocrResult;

		//fprintf(stdout, "\nBox[%d]: x=%d, y=%d, w=%d, h=%d\n", i, box->x, box->y, box->w, box->h);

		if (hasText) {
			/*api->SetRectangle(box->x, box->y, box->w, box->h);
			const char* ocrResult = api->GetUTF8Text();
			text += "[\"" + std::to_string(i) + "\",\"" + std::to_string(box->x) + "\",\"" + std::to_string(box->y) + "\",\"" + std::to_string(box->w) + "\",\"" + std::to_string(box->h) + "\",\"" + ocrResult + "\"]";
			delete[] ocrResult;*/
		}
		else
			text += "[" + std::to_string(i) + "," + std::to_string(box->x) + "," + std::to_string(box->y) + "," + std::to_string(box->w) + "," + std::to_string(box->h) + "]";

		if (i == len - 1)
			text += "\n";
		else
			text += ",\n";

		boxDestroy(&box);
	}

	text += "\n]";

	//std::cout << text;

	// Destroy used object and release memory
	api->End();
	delete api;
	pixDestroy(&image);

	return text;
}

string word_get_X1Y1X2Y2(string language, string fileName, bool hasText) {
	Pix* image = pixRead(fileName.c_str());

	tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
	//api->Init(NULL, "eng");
	// Initialize tesseract-ocr with English, without specifying tessdata path
	if (api->Init(NULL, language.c_str())) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}

	api->SetImage(image);

	string text = "[\n";

	api->Recognize(0);
	tesseract::ResultIterator* ri = api->GetIterator();
	tesseract::PageIteratorLevel level = tesseract::RIL_WORD;
	if (ri != 0) {
		int k = 0;
		do {
			int x1, y1, x2, y2;
			ri->BoundingBox(level, &x1, &y1, &x2, &y2);

			//float conf = ri->Confidence(level);
			//const char* word = ri->GetUTF8Text(level); 
			//printf("word: '%s';  \tconf: %.2f; BoundingBox: %d,%d,%d,%d;\n", word, conf, x1, y1, x2, y2);
			//delete[] word;
			//printf("conf: %.2f; BoundingBox: %d,%d,%d,%d;\n", conf, x1, y1, x2, y2);

			if (k == 0) {
				if (hasText) {
					const char* word = ri->GetUTF8Text(level);
					text += "[\"" + to_string(k) + "\",\"" + to_string(x1) + "\",\"" + to_string(y1) + "\",\"" + to_string(x2) + "\",\"" + to_string(y2) + "\",\"" + word + "\"]";
					delete[] word;
				}
				else
					text += "[" + to_string(k) + "," + to_string(x1) + "," + to_string(y1) + "," + to_string(x2) + "," + to_string(y2) + "]";
			}
			else {
				if (hasText) {
					const char* word = ri->GetUTF8Text(level);
					text = text + "," + "[\"" + to_string(k) + "\",\"" + to_string(x1) + "\",\"" + to_string(y1) + "\",\"" + to_string(x2) + "\",\"" + to_string(y2) + "\",\"" + word + "\"]";
					delete[] word;
				}
				else
					text = text + "," + "[" + to_string(k) + "," + to_string(x1) + "," + to_string(y1) + "," + to_string(x2) + "," + to_string(y2) + "]";
			}

			k++;
		} while (ri->Next(level));

		//text = replace(text, "][", "],[");
	}

	text += "\n]";

	//std::cout << text;

	// Destroy used object and release memory
	api->End();
	delete api;
	pixDestroy(&image);

	return text;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void post_method_handler(const shared_ptr< Session > session)
{
	const auto request = session->get_request();

	int content_length = request->get_header("Content-Length", 0);

	session->fetch(content_length, [](const shared_ptr< Session > session, const Bytes& body)
		{
			fprintf(stdout, "%.*s\n", (int)body.size(), body.data());
			session->close(OK, "Hello, World!", { { "Content-Length", "13" } });
		});
}

void get_method_handler(const shared_ptr< Session > session)
{
	string file = session->get_request()->get_query_parameter("file", "");
	string lang = session->get_request()->get_query_parameter("lang", "eng");

	file = "C:\\ocr-images\\" + file;
	string text = line_get_XYWH(lang, file, false);

	session->close(OK, text, { { "Content-Length", to_string(text.length()) }, { "Content-Type", "application/json" } });
}

string json_format(string text) {
	/*text = replace___(text, "\n", "^");
	text = replace___(text, "\\", "~");*/
	return text;
}

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

void httpServer_Start(int argc, char** argv) {
	string ip = "127.0.0.1";
	//ip = "192.168.10.54";
	int port = 54321;	

	//std::cout << "22222Have " << argc << " arguments:" << std::endl;
	for (int i = 0; i < argc; ++i) {
		//std::cout << argv[i] << std::endl;
		if (i == 1) {
			long arg = strtol(argv[i], NULL, 10);
			port = (int)arg;

			std::cout << "PORT = " << port << std::endl;
		}
	}

	std::wstring stemp = s2ws("OCR " + ip + ":" + to_string(port));
	LPCWSTR result = stemp.c_str();
	SetConsoleTitle(result);

	auto resource = make_shared< Resource >();
	resource->set_path("/");
	resource->set_method_handler("GET", get_method_handler);

	auto settings = make_shared<Settings>();
	settings->set_port(port);
	settings->set_bind_address(ip);
	settings->set_default_header("Connection", "close");

	Service service;
	service.publish(resource);
	service.start(settings);
}

int main(int argc, char** argv)
{

	//test_GetUTF8Text_001(p_file); // Ok
	//test_GetUTF8Text_002(p_file); // Error

	//test_box_GetComponentImages_001(p_file); // Ok
	//test_box_BoundingBox_001(p_file); // Ok

	//string text = line_get_XYWH(p_file);

	httpServer_Start(argc, argv);
	return EXIT_SUCCESS;
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
