/*
 * toplevel.cpp
 *
 *      Author: Justin Roark
 */


#include <systemc>
#include <opencv2/opencv.hpp>

#include "input.h"
#include "process.h"
#include "output.h"
#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::endl;

using namespace cv;
using namespace std;

enum {
        CV_IMWRITE_JPEG_QUALITY=1
};


int sc_main(int argc, char** argv){
	sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated", sc_core::SC_DO_NOTHING );
	Mat imagein, imageout;
	imagein = imread( argv[1], 1 ); //bring in image from file
	string filterSel = "";
	if (argc == 3) {
		filterSel = argv[2];
	}

	input in("input"); //input module
	output out("output"); //output module
	processimage proc("process", imagein.cols, imagein.rows, filterSel); // Filter

	sc_signal<sc_dt::sc_int<24> > pixel1, pixel2; //24 bit signal to move pixels between modules
	sc_signal<bool> done1, done2; //signals when each module is finished

	sc_clock clk("clk",10,SC_NS,0.5);

	in.pixel(pixel1);
	in.done(done1);
	in.clk(clk);
	out.pixel(pixel2);
	out.done(done2);
	out.clk(clk);
	proc.pixelin(pixel1);
	proc.donein(done1);
	proc.pixelout(pixel2);
	proc.doneout(done2);
	proc.clk(clk);

	in.setImage(imagein); //provides image to input module
	out.setMat(imagein.rows, imagein.cols); //provides dimensions to output module

	cout << "Starting" << endl;
	while(!done2){ //run simulation till it is finished
		sc_start(100, SC_NS);
	}

	imageout = out.getImage(); //retrieve image
	vector<int> params;
	params.push_back(CV_IMWRITE_JPEG_QUALITY);
	params.push_back(95);

	imwrite("processed.jpg", imageout, params); //write image to file

	cout << "Finished" << endl;

	return 0;
}
