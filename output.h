/*
 * output.h
 *
 *      Author: Justin Roark
 */

#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <systemc>
//#include "cv.h"
//#include "highgui.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace sc_core;

SC_MODULE(output){
	Mat I;
	MatIterator_<Vec3b> it;
	MatIterator_<Vec3b> end;
	sc_in<sc_dt::sc_int<24> > pixel;
	sc_in_clk clk;
	sc_in<bool> done;
	int count;

	void buildImage(); //rebuilds pixels back into an image
	void setMat(int rows, int cols); //gets size of image from top level code
	Mat& getImage(); //returns image

	SC_CTOR(output){
		SC_CTHREAD(buildImage, clk.pos());
		Mat temp;
		I = temp; //initializes temporary image
		count = 0;
	}

};


#endif /* OUTPUT_H_ */
