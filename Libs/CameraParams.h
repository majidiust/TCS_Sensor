#ifndef CameraParams_h_M
#define CameraParams_h_M

#include "opencv2/core/core.hpp"

using namespace cv;

class CameraParams
{
public:
	CameraParams(void);

	int roll;
	int pitch;
	int yaw;

	Mat transMat;

	cv::Size newSize;

	bool mirror;

	bool subRect;
	int ROI;
	cv::Rect ROI_rect;

	Mat Rot;

	void save( char* name );
	void load( char* name );

	CameraParams clone(CameraParams camParam);
};

#endif
