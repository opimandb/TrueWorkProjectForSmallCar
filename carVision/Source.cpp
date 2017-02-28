#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv2\opencv.hpp>
#include <opencv2\videoio.hpp>
#include <opencv2\imgproc.hpp>
#include <stdlib.h>
#include <stdio.h>
using namespace cv;
using namespace std;

IplImage* image = 0;
IplImage* bin = 0;
IplImage* dst = 0;
IplImage* frame = 0;
IplImage* cont = 0;

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

	//p17.avi    p3 p6 p7 p10 p11  p14                      p13???!
	char names[26][15]{ "videos\\p1.avi", "videos\\p2.avi", "videos\\p3.avi", "videos\\p4.avi", "videos\\p5.avi", "videos\\p6.avi", "videos\\p7.avi", "videos\\p8.avi", "videos\\p9.avi", "videos\\p10.avi", "videos\\p11.avi", "videos\\p12.avi", "videos\\p13.avi", "videos\\p14.avi", "videos\\p15.avi", "videos\\p16.avi", "videos\\p17.avi", "videos\\p18.avi", "videos\\p19.avi", "videos\\p20.avi", "videos\\p21.avi", "videos\\p22.avi", "videos\\p23.avi", "videos\\p24.avi", "videos\\p25.avi", "videos\\p26.avi" };
	int u = 4;
	int asdssdsds;
newvid:
	cvNamedWindow("orig", CV_WINDOW_AUTOSIZE);
	CvCapture* capture = cvCreateFileCapture(names[u]);
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = 0;
	//canny

	Mat edge, gray2, img2, draw;
	int x = argc >= 3 ? atoi(argv[2]) : 180;
	int y = argc >= 4 ? atoi(argv[3]) : 426;
	int defx = 150, defy = 426;
	int width = argc >= 5 ? atoi(argv[4]) : 225;
	int height = argc >= 6 ? atoi(argv[5]) : 54;
	int add = argc >= 7 ? atoi(argv[6]) : 423;
	int block = 0, oldD1=0, oldD2=0, oldD3=0, oldD4=0;
	int xcheck, ycheck;
	Mat orig;
	int blDotX, blDotX2, blDotX3, blDotX4, countFrame = 0, start=0;
	int centerx[4];
	int sum = 0, cc = 0, inp = 0;
	int Xco, Xco2, count = 0;
	uint8_t *row, *row2, *row3;
	int frameCount = 0;



	while (1) {
		if (x < 0 || x>640) x = defx;
		if (y < 0 || y>480) y = defy;
		frame = cvQueryFrame(capture);
		if (!frame) {
			char verb;
			cin >> verb;
			if (verb == 'e') break;
			if (verb == 'n') { if (u == 26) break; u++; goto newvid; }
			if (verb == 'r') goto newvid;
			
		}
		dst = cvCloneImage(frame);
		cvSetImageROI(dst, cvRect(x, y, width, height));
		cvSetImageROI(frame, cvRect(x, y, width, height));

		cvAddS(frame, cvScalar(add), frame);
		cvResetImageROI(frame);
		orig = cvarrToMat(dst);
		cont = cvCloneImage(&(IplImage)orig);
		row = (uint8_t*)orig.ptr<uint8_t>(1);
		row2 = (uint8_t*)orig.ptr<uint8_t>(53);
		if (start == 1)
		{
			countFrame++;
		}
		if (countFrame == 6)
		{
			oldD1 = defx;
			oldD2 = defx;
			oldD3 = defx;
			oldD4 = defx;
			countFrame = 0;
			start = 0;
		}
		int status1 = 0, status2 = 0;
		blDotX = 0;
		blDotX2 = width - 1;
		blDotX3 = 0;
		blDotX4 = width - 1;

		while (!isBlack(blDotX, row))
		{
			blDotX++;
		}
		if (frameCount>3) if (abs(blDotX - oldD1) > 70) blDotX = oldD1;
		cvCircle(frame, cvPoint(x + blDotX, y + 1), 3, cvScalar(255, 0, 0));
		cout << "dot1: " << blDotX;
		while (status1 == 0)
		{
			if (blDotX2 <= 0) {
				blDotX2 = blDotX; status1 = 1;
			}
			if (isBlack(blDotX2, row))
			{
				if (blDotX2 < 10) break;
				if (blDotX2 - blDotX < 50)
				{
					status1 = 1;
					cout << "dot2: " << blDotX2 << endl;
				}
			}
			if (status1 == 0) blDotX2--;
		}
		if (frameCount>3)if (abs(blDotX2 - oldD2) > 70) blDotX2 = oldD2;
		cvCircle(frame, cvPoint(x + blDotX2, y + 1), 3, cvScalar(155, 155, 155));
		while (!isBlack(blDotX3, row2))
		{
			blDotX3++;

		}
		if (frameCount>3)if (abs(blDotX3 - oldD3) > 70) blDotX3 = oldD3;
		cvCircle(frame, cvPoint(x + blDotX3, y + 48), 3, cvScalar(0, 0, 255));
		cout << "dot3: " << blDotX3;
		while (status2 == 0)
		{
			if (blDotX4 <= 0) {
				blDotX4 = blDotX3; status2 = 1;
			}
			if (isBlack(blDotX4, row))
			{
				if (blDotX4 < 10) break;
				if (blDotX4 - blDotX3 < 70)
				{
					status2 = 1; cout << "dot4: " << blDotX4 << endl;
				}
			}
			if (status2 == 0) blDotX4--;
		}
		if (frameCount>3)if (abs(blDotX4 - oldD4) > 70) blDotX4 = oldD4;

		if (blDotX - blDotX3 > 110) start = 1;
		
		cvCircle(frame, cvPoint(x + blDotX4, y + 48), 3, cvScalar(0, 255, 0));
		Xco = (blDotX + blDotX2) / 2;
		Xco2 = (blDotX3 + blDotX4) / 2;
		Xco = (Xco + Xco2) / 2;
		if (isBlack(blDotX, row)) oldD1 = blDotX;
		if (isBlack(blDotX2, row))oldD2 = blDotX2;
		if (isBlack(blDotX3, row2))oldD3 = blDotX3;
		if (isBlack(blDotX4, row2))oldD4 = blDotX4;
		img2 = cvarrToMat(dst);
		cvtColor(img2, gray2, CV_BGR2GRAY);
		Canny(gray2, edge, 50, 150, 3);
		edge.convertTo(draw, CV_8U);
		bin = cvCloneImage(&(IplImage)draw);
		//cvShowImage("bin", bin);
		int contoursCont = cvFindContours(bin, storage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
		if (contours)
		{
			contours = cvApproxPoly(contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, 3, 1);

			for (CvSeq* seq0 = contours; seq0 != 0; seq0 = seq0->h_next) {
				double perim = cvContourPerimeter(seq0);
				//waitKey();
				cout << "perim:" << perim << endl;
				if (perim < 95 && perim>10) {
					CvPoint* coord = (CvPoint*)cvGetSeqElem(contours, 1);
					xcheck = coord[0].x;
					row3 = (uint8_t*)orig.ptr<uint8_t>(coord[0].y);

					if (frameCount > 10)
					{
						
							block = 1;
							inp = 0;
							if (block == 1) {
								cvDrawContours(cont, seq0, CV_RGB(255, 0, 0), CV_RGB(0, 0, 250), 0, 2, 8);
							}
					}

					cvSetImageROI(frame, cvRect(x, y, width, height));
					cvAddS(cont, cvScalar(add), frame);
					cvResetImageROI(frame);


				}
				cvWaitKey(5);
				cvShowImage(names[u], frame);



			}
		
		}
		else {
			cvShowImage(names[u], frame);
			x = defx;
			y = defy;
		}
		cout << endl << endl;
		cvWaitKey(50);
		if (block == 1)
		{
			cout << "PREPYATSTVIE" << endl << endl;
			block = 0;
			char verb;
			cin >> verb;
			cvDestroyAllWindows();
			if (verb == 'e') break;
			if (verb == 'n') { u++; goto newvid; }
			if (verb == 'r') goto newvid;
		}
		x += (((int)Xco) - 112);
		if (status1 == 0 || status2 == 0)
			x = defx;
		frameCount++;
	}
	cvReleaseCapture(&capture);

	return 0;
}
