/*
 * process.h
 *
 *  Created on: Oct 2, 2012
 *      Author: justin
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#include <systemc>
//#include "cv.h"
//#include "highgui.h"
//#include "opencv2/opencv.hpp"
//#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <string>

//using namespace cv;
using namespace sc_core;
using std::cout;
using std::endl;
using std::vector;
using std::string;

SC_MODULE(processimage){
	SC_HAS_PROCESS(processimage);

	sc_in<sc_dt::sc_int<24> > pixelin;
	sc_out<sc_dt::sc_int<24> > pixelout;
	sc_in<bool> donein;
	sc_out<bool> doneout;
	sc_in_clk clk;
	sc_fifo<sc_dt::sc_int<24> >* fifo1, *fifo2;
	sc_dt::sc_int<24> window[3][3];
	vector<vector<sc_dt::sc_int<24> >> img;

	int sobel_x[3][3] = {
		{-1, 0, 1},
		{-2, 0, 2},
		{-1, 0, 1}
	};

	int sobel_y[3][3] = {
		{-1, -2, -1},
		{ 0,  0,  0},
		{ 1,  2,  1}
	};

	int count;
	int filterCount;
	int cols;
	int rows;
	string select;

	void processing();
	sc_dt::sc_int<24> filter();

	processimage(sc_module_name nm, int col, int row, string filterSel) : sc_module(nm) {
		//fifo1 = new sc_fifo<sc_dt::sc_int<24> >(col);
		//fifo2 = new sc_fifo<sc_dt::sc_int<24> >(col);
		SC_CTHREAD(processing, clk.pos());
		doneout.initialize(false);
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++){
				window[i][j] = 0;
			}
		}
		for (int i = 0; i < col; i++){
			vector<sc_dt::sc_int<24> > tmpRow;
			for (int j = 0; j < row; j++){
				tmpRow.push_back(0);
			}
			img.push_back(tmpRow);
		}

		cols = col;
		rows = row;
		count = 0;
		filterCount = 0;
		select = filterSel;
	}
};

#endif /* PROCESS_H_ */
