#ifndef _CarPlateRecognition_h
#define _CarPlateRecognition_h

//#include <QtSql/QSqlDatabase>
//#include <QtCore/Qdir>

// for parallel processing!
//#include <ppl.h>

#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "opencv2/photo/photo.hpp"
//#include <opencv/highgui.h>
//#include <opencv/cv.h>


#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>

//#include "stdafx.h"
#include <fstream>
#include "dirent.h"
//#include <direct.h>

#include "Plate.h"

#include "Feature.h"

#include "CameraParams.h"

using namespace std;
using namespace cv;

#define showDetection			0 /*0 or 1*/
#define showRecognition			0 /*0 or 1*/
#define showPlatePreprocessing	0 /*0 or 1*/
#define showShadowRemoving		0 /*0 or 1*/

//
#define levelsLetter		11 /*12*/
#define scaleLetter			0.94
#define initLetter			1/0.7

#define levelsDigit			4 /*8*/
#define scaleDigit			0.95
#define initDigit			1.1 /*1.1*/

#define levelsDigit2		4 /*4*/
#define scaleDigit2			0.95
#define initDigit2			1.0 /*1.0*/

//#define MatchTemplateType	CV_TM_SQDIFF
//#define MatchTemplateType	CV_TM_SQDIFF_NORMED
//#define MatchTemplateType	CV_TM_CCORR
//#define MatchTemplateType	CV_TM_CCORR_NORMED
//#define MatchTemplateType	CV_TM_CCOEFF
#define MatchTemplateType	CV_TM_CCOEFF_NORMED

//
#define maxPrevious			50

//
#define load_from_hard		1

//
#define myMax(a,b)    (((a) > (b)) ? (a) : (b))
#define myMin(a,b)    (((a) < (b)) ? (a) : (b))

//
#define Features_LPQ		30

//
#define ulbp_length			59

// "matchThresh2" is the minimum acceptable match score for a plate (product
// of the match score for different characters of the plate)
#define matchThresh2 0.10

// "Templates" contains the template images of the all available characters
#define max_temp	2 // Why 20 ???????????????????????????????????

// "numOfChars" is the number of available characters in the plates
#define numOfChars	28

// "numbers" is the number of characters in a true plate
#define numbers 8

// "maxPlates" is the maximum number of plates that can be exist in an image
#define maxPlates 20

//
extern bool queue;

// CarPlateRecognition is the main class
class CarPlateRecognition
{
public:
	CarPlateRecognition( void );
	~CarPlateRecognition(void);

	int release( void );

	float difTime[10];

	//
	char storePath[200];
	bool shouldStore;
	char queue_img_name[200];
	char* plateName;

	//
	void setStorePath(char* path0);

	// "name" contains the character name for the folders (for example, in folder 'w',
	// the desired character is "Sad"
	char* name[numOfChars];

	//
	wstring farsiName[numOfChars];
	
	// This function loads all the template images from the memory
	int initialize(void);

	// If in the desired application, size of the plates are similar, we can 
	// perform the search for the plates in one scale (instead of multiscale
	// that is more time consumer). In such applications, this function may
	// be used. Input of this function is an image that at least one plate
	// in the desired format is existed in it. The returned vale is a boolean
	// variable and determines that any acceptable plate is recognized in the
	// input image or not.
	// In this function some global variables are initialized that are used in
	// the other functions (such as "PlateDetection")
	bool setDefaultPlateSize(Mat img);

	// In the "resetDefaultPlateSize" function some global variables are initialized
	// that are used in the other functions (such as "PlateDetection")
	void resetDefaultPlateSize(void);

	//
	int setCameraNames( char* processCamIP, char* captureCamIP, bool useQueue, char* queueFolder ); // We should use default values for useQueue and queueFolder

	/* Other functions are emitted from this class */

	// This function detects the potentially locations for the existence of
	// the plates. Input is an image ("img"). Outputs are the location of the 
	// two opposite vertices for the detected plates ("rect") and the number
	// of the detected plates (returned value)
	//int PlateDetection(const IplImage *img, int rect[maxPlates][4]);
	
	// This function gives an image of a plate ("plate") as input and recognize
	// its number. The indexes of the recognized number is stored in "idx" and 
	// the returned value determines that the recognized plate number is valid
	// (1) or not (0) 
	//int PlateRecognition(IplImage *plate, int idx[numbers], CvPoint pnt[2], float* Match);

	// This function gives an image ("img") that may contain some digits and 
	// letters. It extract the strongest character in the "img". "sign" is a
	// two states parameter that express the character color against the plate
	// color (if the characters are darker the sign is positive and otherwise
	// it is negative). "type" specify that the desired character is a digit (0)
	// or a letter (1). The first output ("index") is the index of the recognized
	// character and the second output ("pnt") is the two vertices of the location
	// of the recognized character. Also, the returned value is the score of this
	// character (maximum of its absolute is 1)
	//float CharacterRecognition(IplImage *img, int sign, int type, int *index, CvPoint *pnt);
	//float CharacterRecognition(IplImage *img, int sign, int type, float* R, int *index, CvPoint *pnt);

	// This function is a threshold estimation for a gray level image (Otsu method).
	// "img" is the input image and "name" is the desired name for the window
	// that shows the histogram. The returned values is the desired threshold 
	//int HistogramThreshold(IplImage* img, char* name);

	// In this function a binary image ("img") is input and a horizontal
	// rectangle that covers all the 1 pixels is the output.
	//CvRect mrmBoundingRect(IplImage* img);
	
	// In this function an image of one character ("img") is input. This 
	// character will classified to one of the "group1" characters. The
	// returned value is the index of the classified character.
	//int ClassificationGroup1(IplImage* img, float* score);
	
	// In this function an image of one character ("img") is input. This 
	// character will classified to one of the "group1" characters. The
	// returned value is the index of the classified character.
	//int ClassificationGroup2(IplImage* img, float* score);

	// Default ??????????????????????????????????
	// This parameter controls that only one size should be searched (if true)
	// or multiscale is required (if false). 
	bool DefaultPlateSize;
	// "PlateSize" controls that the desired plate is predicted with what size.
	// "PlateSize" is less than or equal to 1 (smaller values express that the
	// desired plate is larger.
	float PlateSize; // 0.25

	//
	void drawRotatedRect(Mat img, RotatedRect rect, Scalar color, int thickness);

	//
	// "folder" contains the name of the folders that contain the templates
	char* folder[numOfChars];

	//
	wstring getPlateType(int type_num);
	wstring getPlateCity(int code);

	//
	int newRecord;

	//
	bool initialized;

	// Camera parameters. We require only one! but use 2 for another camera.
	CameraParams camParam1;
	CameraParams camParam2;

	//
	void runTransform( Mat &img, int cam = 1 );
	
	//	
	void setRotMat( int roll, int pitch, int yaw, int cam = 1 );
	//
	void transMatCompute( cv::Size S, int cam = 1 );

	//
	void runTransformSetting( Mat& img, int cam = 1 );

	//
	void setCameraParams( int mirror, bool subRect, int ROI, Rect ROI_rect, int cam = 1 );

	//
	int initializeCameraSetting( char* camIP );
	void transMatComputeSetting( int cam = 1 );
	Mat imgSetting;

	//
	void FormatPelakString2Display( char* plateStr );

	// This function is a combination of the above functions (PlateDetection & 
	// PlateRecognition). Its input is an image ("img") that can contain some
	// plates. Its outputs are the recognition indexes (recogNumbers) and the 
	// bounding rectangles (plates) for each plate. Also, the returned value is
	// the number of the recognized plates.
	//int PlateDetognition(IplImage *img, int recogNumbers[maxPlates][numbers], int plates[maxPlates][4], float Score[maxPlates]);
	vector<Plate> PlateDetognition(Mat img);
	vector<Plate> PlateDetognition2(Mat img);
	vector<Plate> PlateDetognitionStandard(Mat img, Mat imgCapture);

	int testClassifierGroup2( Mat img, float& score, int type );
	int testClassifierGroup1( Mat img, float& score, int type );
private:
	//
	Mat motionDetection(Mat img_old, Mat img);
	Mat lastImportantRegions(Mat img, vector<Plate> Plates);

	//
	bool notDifference(Mat img_old, Mat img);

	// For train the tow SVM for classification of group1 and group2 characters
	void TrainSVMs(void);


	//
	bool SVM_type; // 0 for LBP
	// 1 for Gray levels

	//
	bool save_group_images;
	int group1_idx;
	int group2_idx;
	int group3_idx;
	int group4_idx;

	//
	bool save_plate;
	int plate_idx[2];

	//
	int ulbp[ulbp_length-1];

	//
	float avgDetectionTime;
	float avgRecognition0Time;
	float avgRecognition1Time;

	int detectNum;
	int recog0Num;
	int recog1Num;

	// "matchThresh" is the minimum acceptable match score for a character
	float matchThresh;

	//
	int char_type;

	//
	int char_numbers;

	//
	Mat Templates[numOfChars][max_temp];

	// If "PlateSizeSetting" be setted, the search is performed in multiscale and
	// the scale of the best recognized plate is considered as the default scale
	// for the others.
	bool PlateSizeSetting;

	// "PlateScale" is the scaling coefficient in the multiscale search when the
	// "PlateSizeSetting" is setted. Its initial value is 0.5 and we reduce it for
	// more accurate searches.
	float PlateScale;

	// "MinPlateRatio" is the minimum acceptable scale for the plate. Its default
	// value is 0.1.
	float MinPlateRatio;

	// "DetectedPlatesSizes" is an array that contains the scale of the detected 
	// plates. We use scale of the best recognized plate as the using scale when 
	// "DefaultPlateSize" is setted.
	float DetectedPlatesSizes[maxPlates];

	// "PlateSettingMatch" is the match score of the best recognized plate. We use
	// this value to obtain the best scale when the "setDefaultPlateSize" function
	// is runned several times.
	float PlateSettingMatch;

	// "height_ratio" contains the ratio of the characters height to the plate height.
	// This parameter is different for different characters, but, for a character is
	// constant in the our plates
	float height_ratio[numOfChars];

	// "sign_chars" is a vector to specify the color of plate (dark on bright or vice versa)
	// for each of the characters. For example, "Alef" is only used for government vehicles
	// and its plates are bright on dark. In this vector, 1s are corresponded to dark on
	// bright, 2s are corresponded to bright on dark and 3 for both.
	int sign_chars[numOfChars];

	// "Scale" is a scale for the score of each character. This parameter can reduce 
	// the number of mistakes. For example, character "Gim" is complex, so, its scale
	// is large. On the other hand, character "Dal" is simple and its scale is small.
	// In other words, character "Dal" can be found in the some undesired regions (same
	// as a part of "Ya") but character "Gim" cannot.
	float scale[numOfChars];

	// "gorup1" & "gorup2" are two groups of the similar characters. "group1" is for "2",
	// "3" & "7" and "group2" is for "Sin" & "Sad". These similar characters require an
	// additional step for recognition (Using SVM classifier)
	int group1[5];
	int group2[5];
	int group3[5];
	int group4[5];

	float group1Score[5];
	float group2Score[5];
	float group3Score[5];
	float group4Score[5];

	Mat lbpHist(Mat img);
	int isPlate(Mat img);

	vector<Plate> PlateDetectionFast(Mat img_, Mat mask);
	int detectionTime1;
	int detectionTime2;

	vector<Plate> PlateDetection(Mat img, Mat mask);

	int ClassificationGroup1(Mat img, float& score);
	int ClassificationGroup2(Mat img, float& score);
	int ClassificationGroup_149(Mat img, float& score);
	int ClassificationGroup_58(Mat img, float& score);
	int ClassificationGroup_H_237(Mat img, float& score);
	float CharacterRecognition(Mat img, int sign, int type, float& R, int& index, Point *pnt);
	int PlateRecognition(Mat plate, int idx[numbers], Point pnt[2], float* Match, int charRows[numbers]);
	RotatedRect mergeRotatedRects(RotatedRect plate_rect1, RotatedRect plate_rect2);
	int PlateRecognitionStandard(Mat plate, int idx[numbers], float scores[numbers], Point pnt[2], float& Match, int& recNum);
	bool verifySizes(RotatedRect mr);
	Mat histeq(Mat in);
	vector<Plate> PlateDetection2(Mat img);
	void ulbp_codes(int ulbp[ulbp_length-1]);

	Rect mrmBoundingRect(Mat img, int maxVal = 255);

	RotatedRect mrmBoundingRotatedRect(Mat img);
	float CharacterRecognitionStandard(Mat img, int sign, int type, float& R, int& index, Point *pnt, Point pnt_last);

	//
	bool isCharacter3_method1( Mat img );

	//
	void savePCA( PCA pca, char* name );
	void loadPCA( PCA& pca, char* name );
	void savePCAs( void );
	void loadPCAs( void );

	//
	int PCAClassificationGroup1( Mat img, float &score );
	Mat featureExtractionGroup1( Mat img );

	//
	void lpq_params2( Feature& featureParam );

	//
	Mat lpq_feature_extraction2( Mat img, Feature featureParam );

	//
	PCA pca2;
	PCA pca3;
	PCA pca7;
	PCA pcaW;
	PCA pcaS;

	//
	Feature featureParam0;

	Mat imgOld;

	// For tracking
	vector<Plate> oldPlates;
	vector<Plate> newPlates;
	vector<Plate> trkPlates;
	void runTrackingStandard( void );

	//
	Plate mergePlates( Plate& plate1, Plate plate2 );
	//
	wstring GetFarsiHarf(int code);
	//System::String^ insert_in_DB(System::String^ plateNum );

	int PCAClassificationGroup2( Mat img, float &score );
	Mat featureExtractionGroup2( Mat img );

	int ClassificationGroup_sinsad( Mat img, float& score );

	//
	void setPlateString( Plate& plt );
	void saveMat( Mat myMat, char* fileName );
	void loadMat( Mat& myMat, char* fileName );

	//
	int CharacterClassificationFine( Mat img, Rect rect, int initGuess, float initMatch, float& score );

	void mrmMatchTemplate( Mat img, Mat templ, Mat& result, int method );
	void mrmCrossCorr( const Mat& img, const Mat& _templ, Mat& corr, Size corrsize, int ctype, Point anchor, double delta, int borderType );
	void FastMatchTemplate( Mat img, Mat templ, Mat& result, int method, int sign, int multiMatch );
	void fastMatchTemplate2(cv::Mat& srca, /* The reference image */ cv::Mat& srcb, /* The template image */ cv::Mat& dst, /* Template matching result */ int maxlevel) /* Number of levels */;
	
	void trainClassifierGroup1( void );

	Mat preprocessing( Mat img );
	void cropCharacter( Mat& img );
	void rotateCharacter( Mat& img );
	int ClassificationGroup1MRM( Mat img, float& score );
	int ClassificationGroup2MRM( Mat img, float& score );
	void smoothCurve(float* src, int srcN, float* dst);

	void localOptimums( float* src, int srcN, int* localMaxIdx, float* localMaxVal, int& numMax, int* localMinIdx, float* localMinVal, int& numMin, float threshMax = -1, float threshMin = -1 );
	
	//
	RotatedRect PlatePreprocessing( Mat& img );

	//
	void plot( float* x, int l, char* lab );

	//
	Mat cropRotatePlate( Mat img, RotatedRect rect );

	int isPlate2(Mat img);

	void removeShadow(Mat& inputMat);
	Mat cropRotatePlate2( Mat img, RotatedRect rect );

	void rotImg(Mat inputImg, Mat &outputImg, float theta, Size sz, Point2f center);
	void meshgrid(Mat xgv, Mat ygv, Mat &X, Mat &Y);
	void meshgridTest(Point2f xgv, Point2f ygv,float sampStepTemp, Mat &X, Mat &Y);
	void interp2(Mat pos2, Mat pos1, Mat img, Mat &warpImg);

	vector<Plate> PlateDetectionFast2(Mat img_, Mat mask);

	int checkOverlapRotatedRect(RotatedRect rect1, RotatedRect rect2);

	Rect mrmBoundingRectFast(Mat img);
	
	//
	Mat templatesMatch[numOfChars][max_temp][10];
	int PlateRecognitionStandardFast(Mat plate, int idx[numbers], float scores[numbers], Point pnt[2], float& Match, int& recNum);
	float LetterRecognitionStandardFast(Mat img, int sign, int type, float& R, int& index, Point *pnt, Point pnt_last);
	void ComputeTemplateMatchings( Mat img, int sign, float& R, int digitScale );
	float DigitRecognitionStandardFast( Mat img, int sign, int type, float& R, int& index, Point *pnt, Point pnt_last, int digitScale );

	void mrmConvertScaleAbs( Mat img1, Mat& img2 );
	//
	ofstream truePlates;
	ofstream falsePlates;

	//
	int subChars[numbers][numbers];
	int subPlates[maxPlates][maxPlates];

	int breaks[20];
};
#endif
