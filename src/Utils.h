//
// Created by tlss on 2019/11/26.
// Reconstruction by Anna on 2019/11/29
//

#ifndef MASKGENERATOR_UTILS_H
#define MASKGENERATOR_UTILS_H

#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>

extern int thres_init_value;
extern cv::Mat img;
extern cv::Mat gray;
extern cv::Mat threshold_img;
extern cv::Mat fill_mask_img;
extern cv::Mat mask_img;
extern cv::Mat img_orgi;

void onMouse(int event, int x, int y, int flags, void* param);

void onThreshold(int, void*);

void myDrawContours(cv::Mat src);

#endif //MASKGENERATOR_UTILS_H
