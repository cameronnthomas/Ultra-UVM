/*
 * output.cpp
 *
 *      Author: Justin Roark
 */
#include "output.h"
using std::cout;
using std::endl;

void output::buildImage(){
	while(true){
		if (!done && count >= I.cols + 3){ //delays till fifo is full
			sc_dt::sc_int<24> tempsc;
			tempsc = pixel.read();
			(*it)[0] = tempsc.range(7,0); //puts RGB values into Mat variable
			(*it)[1] = tempsc.range(15,8);
			(*it)[2] = tempsc.range(23,16);
			//cout << "out: " << tempsc.range(7,0) << ", " << tempsc.range(15,8) << ", " << tempsc.range(23,16) << endl;
			++it;
		}
		count++;
		//cout << " " << count << " ";
		wait();
	}
}

void output::setMat(int rows, int cols){
	Mat temp(rows,cols, CV_8UC3, Scalar(0,0,0)); //sets up image with correct dimensions
	I = temp;
	it = I.begin<Vec3b>();
	end = I.end<Vec3b>();
}

Mat& output::getImage(){
	return I;
}



