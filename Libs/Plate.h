/*****************************************************************************
*   Number Plate Recognition using SVM and Neural Networks
******************************************************************************
*   by David Millلn Escrivل, 5th Dec 2012
*   http://blog.damiles.com
******************************************************************************
*   Ch5 of the book "Mastering OpenCV with Practical Computer Vision Projects"
*   Copyright Packt Publishing 2012.
*   http://www.packtpub.com/cool-projects-with-opencv/book
*****************************************************************************/

#ifndef Plate_h
#define Plate_h

#include <string.h>
#include <vector>

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv/cvaux.h"

using namespace std;
using namespace cv;

class Plate{
    public:
        Plate();
        Plate(Mat img, RotatedRect pos);
        
		//string str();
        
		// مستطیل اطراف پلاک
		RotatedRect position;
		RotatedRect lastPosition;

        Mat img;
        Mat imgWhole;
		Mat imgCapture;
		double score;
		//کد مربوط به 8 کاراکتر موجود در پلاک را مشخص می کند
		int number[8];

		//برای پلاک های غیر استاندارد
		//سطر هر کاراکتر را مشخص می کند
		int charRows[8]; //"0","1","-1"			0->standard

		//score of each character
		double charScores[8];

		//
		wstring plateString;
		wstring plateStringShow;
		string plateString0;

		//number of plate characters		->8 for standard
		int chars;

		//for tracking -> no plate detected
		int unseenFrames;

		//current plate is store or not
		bool isStored;


		//flag for ui to save plate image
		bool flgStore;

		//save the time variables
		int time_h;
		int time_m;
		int time_s;
		int year;
		int month;
		int day;

		//
		float scale;

		//
		Plate clone( void );
};

#endif
