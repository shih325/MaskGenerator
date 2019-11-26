//
// Created by tlss on 2019/11/26.
//
#include <Utils.h>

int thres_init_value = 100;
int canny_thres = 30;
Mat gray;
Mat threshold_img;
Mat canny_img;

void onCanny(int, void*) {
    Canny(threshold_img, canny_img, canny_thres, canny_thres * 3, 3);
    imshow("threshold", canny_img);
}

void onThreshold(int, void*) {
    threshold(gray, threshold_img, thres_init_value, 255, THRESH_TOZERO);
    imshow("threshold", threshold_img);
}

void myDrawContours(Mat src) {
    Mat img = src;
    cvtColor(img, gray, COLOR_RGB2GRAY);
    imshow("gray", gray);
    namedWindow("threshold");
    createTrackbar("threshold", "threshold", &thres_init_value, 255, onThreshold);
    createTrackbar("canny", "threshold", &canny_thres, 255, onCanny);

    waitKey(0);
    destroyAllWindows();
}
