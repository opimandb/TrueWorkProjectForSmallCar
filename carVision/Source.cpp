#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv2\opencv.hpp>
#include <opencv2\videoio.hpp>
#include <stdlib.h>
#include <stdio.h>
using namespace cv;
using namespace std;

IplImage* image = 0;
IplImage* gray = 0;
IplImage* bin = 0;
IplImage* dst = 0;
IplImage* frame = 0;

inline bool isBlack(int x, uint8_t *row)
{
	int r = row[3 * x + 2];
	int g = row[3 * x + 1];
	int b = row[3 * x + 0];

	return ((b*0.0722 + g*0.7152 + r*0.2126) < 60);
}

int main(int argc, char* argv[])
{
	int step = 1;
	//p17.avi    p3 p6 p7 p10 p11
	char* filename = argc >= 2 ? argv[1] : "C:\\Users\\osipp\\Desktop\\videos\\n14.avi";  
	cvNamedWindow("original", CV_WINDOW_AUTOSIZE);

	CvCapture* capture = cvCreateFileCapture(filename);
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = 0;
	//canny
	Mat edge, gray2, img2, draw;
	int x = argc >= 3 ? atoi(argv[2]) : 240;
	int y = argc >= 4 ? atoi(argv[3]) : 370;
	int width = argc >= 5 ? atoi(argv[4]) : 150;
	int height = argc >= 6 ? atoi(argv[5]) : 130;
	int add = argc >= 7 ? atoi(argv[6]) : 400;
	Mat orig;
	int blDotX = 0, blDotX2 = 0, blDotX3 = 0, blDotX4 = 0, Xco = 0, Xco2 = 0, count = 0;
	uint8_t *row, *row2;
	while (1) {
		frame = cvQueryFrame(capture);
		if (!frame) {
			char c = cvWaitKey(0);
			break;
		}
		dst = cvCloneImage(frame);
		cvSetImageROI(dst, cvRect(x, y, width, height));
		cvSetImageROI(frame, cvRect(x, y, width, height));
		cvAddS(frame, cvScalar(add), frame);
		cvResetImageROI(frame);
		orig = cvarrToMat(dst);
		//imshow("orig", orig);
		row = (uint8_t*)orig.ptr<uint8_t>(5);
		row2 = (uint8_t*)orig.ptr<uint8_t>(105);
		blDotX = 0;
		blDotX2 = 149;
		blDotX3 = 0;
		blDotX4 = 149;
		while (!isBlack(blDotX, row))
		{
			blDotX++;
		}
		while (!isBlack(blDotX2, row))
		{
			blDotX2--;
		}
		while (!isBlack(blDotX3, row2))
		{
			blDotX3++;
		}
		while (!isBlack(blDotX4, row2))
		{
			blDotX4--;
		}
		Xco = (blDotX + blDotX2)/2;
		Xco2 = (blDotX3 + blDotX4) / 2;
		Xco = (Xco + Xco2) / 2;
		if (count == 0) {
			x += (int)Xco-57; count++;
		}
		else x += (((int)Xco)-57);
		gray2 = cvarrToMat(gray);
		img2 = cvarrToMat(dst);
		cvtColor(img2, gray2, CV_BGR2GRAY);
		Canny(gray2, edge, 50, 150, 3);
		edge.convertTo(draw, CV_8U);
		gray = cvCloneImage(&(IplImage)draw);
		bin = cvCloneImage(&(IplImage)draw);
		int contoursCont = cvFindContours(bin, storage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
		if (contours)
		{
			contours = cvApproxPoly(contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, 3, 1);

			for (CvSeq* seq0 = contours; seq0 != 0; seq0 = seq0->h_next) {
				double perim = cvContourPerimeter(seq0);
				//cout << "perim:" << perim << endl;
				if (perim < 180 && perim>40) {
					cout << "PREPYATSTVIE" << endl << endl; cvWaitKey(0);
				}
			}
		}
		char q = cvWaitKey(10);
		cvShowImage("original", frame);
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("original");
	return 0;
}
