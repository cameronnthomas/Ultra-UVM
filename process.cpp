/* #define int64 sc_int64
#define uint64 sc_uint64
#include <systemc>
#undef int64
#undef uint64

#define int64 cv_int64
#define uint64 cv_uint64
#include <opencv2/opencv.hpp>
#undef int64
#undef uint64 */

#include "process.h"
#include <iostream>
#include <algorithm>
#include <math.h>
#include <math.h>

/*========FILTER========*/

void processimage::processing() {
    if (select.compare("median") == 0|| select.compare("Median") == 0|| select.compare("") == 0){
        while (true) {
        
        if (donein.read()) { // If input module is done
            doneout.write(true); // Set output module to done
            //break; // End simulation
        }
        //cout << " " << count << " ";
        if (count < cols+1) {
            img[count%cols][count/cols] = pixelin.read();
            //cout << " " << count%cols << " ";
            
        } else if (count < cols*rows){
            if (!donein.read()) img[count%cols][count/cols] = pixelin.read();
            //cout << " " << count << " ";
            // Check if current pixel is on the edge
            bool is_edge = (filterCount % cols == 0 || filterCount % cols == cols-1 || filterCount / cols == 0 || filterCount / cols == cols-1);
            if (is_edge) {
                //cout << " edge ";
            } else {
                // Copy values from image into window
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        int x = filterCount%cols + j-1;
                        int y = filterCount/cols + i-1;
                        if (x >= 0 && x < cols && y >= 0 && y < rows) { 
                            window[i][j] = img[x][y];
                        } else {
                            // Handle edge case (e.g. set to 0 or a constant value)
                            window[i][j] = 0;
                        }
                    }
                }
                // Compute filtered pixel and write to output
                pixelout.write(filter());
            }
            filterCount++;
        }
        count++;
        wait();
        }
    }
    else if (select.compare("sobel") == 0 || select.compare("Sobel") == 0) {
        while (true) {
            if (donein.read()) { // If input module is done
                doneout.write(true); // Set output module to done
                break; // End simulation
            }
            if (count < cols+1) {
                img[count%cols][count/cols] = pixelin.read();
            } else if (count < cols*rows){
                if (!donein.read()) img[count%cols][count/cols] = pixelin.read();
                bool is_edge = (filterCount % cols == 0 || filterCount % cols == cols-1 || filterCount / cols == 0 || filterCount / cols == cols-1);
                if (is_edge) {
                    // Handle edge case (set to 0)
                    pixelout.write(0);
                } else {
                    for (int i = 0; i < 3; i++) {
                        for (int j = 0; j < 3; j++) {
                            int x = filterCount%cols + j-1;
                            int y = filterCount/cols + i-1;
                            if (x >= 0 && x < cols && y >= 0 && y < rows) { 
                                window[i][j] = img[x][y];
                            } else {
                                // Handle edge case (set to 0)
                                window[i][j] = 0;
                            }
                        }
                    }
                    sc_dt::sc_int<24> mag = filter();
                    sc_dt::sc_int<24> magAvg = (mag.range(7,0) + mag.range(15,8) + mag.range(23,16)) / 3;
                    if (magAvg > 40) {
                        sc_dt::sc_int<24> white;
                        white.range(7,0) = 255; //copy RGB values into 24 bit int
                        white.range(15,8) = 255;
                        white.range(23,16) = 255;
                        pixelout.write(white);
                    } else {
                        pixelout.write(0);
                    }
                }
                filterCount++;
            }
            count++;
            wait();
        }
    }
}

sc_dt::sc_int<24> processimage::filter() {
    if (select.compare("median") == 0 || select.compare("Median") == 0|| select.compare("") == 0){
        sc_dt::sc_int<24> vals[9];
    int idx = 0;
    // Copy values from window to array
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            vals[idx++] = window[i][j];
        }
    }
    // Sort array
    for (int i = 0; i < 9; i++) {
        for (int j = i+1; j < 9; j++) {
            if (vals[j] < vals[i]) {
                sc_dt::sc_int<24> temp = vals[i];
                vals[i] = vals[j];
                vals[j] = temp;
            }
        }
    }
    // Return median value
    return vals[4];
    } 
    else if (select.compare("sobel") == 0 || select.compare("Sobel") == 0) { 
        int Gx_r = 0, Gy_r = 0, Gx_g = 0, Gy_g = 0, Gx_b = 0, Gy_b = 0;
        // Apply Sobel operator to window for each color channel
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                Gx_r += window[i][j].range(7,0) * sobel_x[i][j];
                Gy_r += window[i][j].range(7,0) * sobel_y[i][j];
                Gx_g += window[i][j].range(15,8) * sobel_x[i][j];
                Gy_g += window[i][j].range(15,8) * sobel_y[i][j];
                Gx_b += window[i][j].range(23,16) * sobel_x[i][j];
                Gy_b += window[i][j].range(23,16) * sobel_y[i][j];
            }
        }

        int G_r = sqrt((Gx_r * Gx_r) + (Gy_r * Gy_r));
        int G_g = sqrt((Gx_g * Gx_g) + (Gy_g * Gy_g));
        int G_b = sqrt((Gx_b * Gx_b) + (Gy_b * Gy_b));
        // Combine channels and return result
        sc_dt::sc_int<24> G = (G_r, G_g, G_b);
        return G;
    }
}
