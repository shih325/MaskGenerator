//
// Created by tlss on 2019/11/26.
// Reconstruction by Anna on 2019/11/29
//
#include <Utils.h>

int thres_init_value = 130;
cv::Mat img;
cv::Mat gray;
cv::Mat threshold_img;
cv::Mat fill_mask_img;
cv::Mat mask_img;
cv::Mat img_orgi;

void onMouse(int event, int x, int y, int flags, void* param) {
    cv::Point seed = cv::Point(x, y);
    cv::Scalar fill_color = cv::Scalar(0, 0, 255);
    cv::Rect ccomp;
    flags = 4 | 0 | (255 << 8);
	switch (event) {
		case cv::EVENT_LBUTTONDOWN: floodFill(fill_mask_img, mask_img, seed, fill_color, &ccomp, cv::Scalar(20, 20, 5), cv::Scalar(20, 20, 5), flags);
		// case EVENT_RBUTTONDOWN: exit(0);
	}
	imshow("flood fill", fill_mask_img);
    imshow("mask", mask_img);
}

void onThreshold(int, void*) {
    cv::threshold(gray, threshold_img, thres_init_value, 255, cv::THRESH_TOZERO);
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(threshold_img, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
    img.copyTo(img_orgi);
    while (1) {
        drawContours(img_orgi, contours, -1, cv::Scalar(0, 255,0), 1);
        fill_mask_img = img_orgi;
        cv::imshow("flood fill", fill_mask_img);
        cv::namedWindow("flood fill");
        cv::setMouseCallback("flood fill", onMouse, 0);
        char key = (char)cv::waitKey(100);
        switch(key) {
            case 27: break;
            // press 'q' for quit
            case 113: cv::destroyAllWindows(); exit(0);
            default: break;
        }
//         (key == 27) {
//            cout << "key: " << key << endl;
//            break;
//        }
    }
}

void myDrawContours(cv::Mat src) {
    img = src;
    mask_img = cv::Mat::zeros(img.rows+2, img.cols+2, CV_8UC1);
    cv::cvtColor(img, gray, cv::COLOR_RGB2GRAY);
    cv::namedWindow("flood fill");
    cv::imshow("flood fill", img);
    cv::imshow("mask", mask_img);
//    namedWindow("mask");
    cv::createTrackbar("threshold", "flood fill", &thres_init_value, 255, onThreshold);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
