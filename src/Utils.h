//
// Created by tlss on 2019/11/26.
//

#ifndef MASKGENERATOR_UTILS_H
#define MASKGENERATOR_UTILS_H

#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

extern int thres_init_value;
extern Mat img;
extern Mat gray;
extern Mat threshold_img;
extern Mat fill_mask_img;
extern Mat mask_img;
extern Mat img_orgi;

void onMouse(int event, int x, int y, int flags, void* param);

void onThreshold(int, void*);

void myDrawContours(Mat src);

#endif //MASKGENERATOR_UTILS_H
