/*
 * input.cpp
 *
 *      Author: Justin Roark
 */

#include "input.h"
using std::cout;
using std::endl;

void input::nextPixel(){
	while(true){
		if (it == end){
			done = true;
		}
		else {
			sc_dt::sc_int<24> temp;
			temp.range(7,0) = (*it)[0]; //copy RGB values into 24 bit int
			temp.range(15,8) = (*it)[1];
			temp.range(23,16) = (*it)[2];
			pixel.write(temp);
			++it;
			count++;
			//cout << " " << count << " ";
			//cout <<"in: "<< temp.range(7,0) << ", " << temp.range(15,8) << ", " << temp.range(23,16) << endl;
		}
		wait();
	}
}

void input::setImage(Mat& imagein){
	I = imagein; //sets image
	it = I.begin<Vec3b>(); //sets Iterator for image
	end = I.end<Vec3b>();
}



