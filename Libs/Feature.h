#ifndef CarPlateRecognition_h_M
#define CarPlateRecognition_h_M

#include "opencv2/core/core.hpp"

using namespace cv;

class Feature
{
public:
	Feature(void);

	int type;
	
	int cols;
	int rows;
	int block_length;
	int length;
	double M;
	double a;
	double rho;

	Mat VW;

	Mat V;
	Mat w0_r, w0_i;
	Mat w1_r, w1_i;
	Mat w2_r, w2_i;
	int freqestim;

	Mat region_factor;

	int width;
	int height;
};

#endif