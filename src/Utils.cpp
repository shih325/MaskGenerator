//
// Created by tlss on 2019/11/26.
//
#include <Utils.h>

int thres_init_value = 130;
Mat img;
Mat gray;
Mat threshold_img;
Mat fill_mask_img;
Mat mask_img;
Mat img_orgi;

void onMouse(int event, int x, int y, int flags, void* param) {
    Point seed = Point(x, y);
    Scalar fill_color = Scalar(0, 0, 255);
    Rect ccomp;
    flags = 4 | 0 | (255 << 8);
	switch (event) {
		case EVENT_LBUTTONDOWN: floodFill(fill_mask_img, mask_img, seed, fill_color, &ccomp, Scalar(20, 20, 5), Scalar(20, 20, 5), flags);
		// case EVENT_RBUTTONDOWN: exit(0);
	}
	imshow("flood fill", fill_mask_img);
    imshow("mask", mask_img);
}

void onThreshold(int, void*) {
    threshold(gray, threshold_img, thres_init_value, 255, THRESH_TOZERO);
    vector<vector<Point> > contours;
    findContours(threshold_img, contours, RETR_LIST, CHAIN_APPROX_NONE);
    img.copyTo(img_orgi);
    while (1) {
        drawContours(img_orgi, contours, -1, Scalar(0, 255,0), 1);
        fill_mask_img = img_orgi;
        imshow("flood fill", fill_mask_img);
        namedWindow("flood fill");
        setMouseCallback("flood fill", onMouse, 0);
        char key = (char)waitKey(100);
        switch(key) {
            case 27: break;
            // press 'q' for quit
            case 113: destroyAllWindows(); exit(0);
            default: break;
        }
//         (key == 27) {
//            cout << "key: " << key << endl;
//            break;
//        }
    }
}

void myDrawContours(Mat src) {
    img = src;
    mask_img = Mat::zeros(img.rows+2, img.cols+2, CV_8UC1);
    cvtColor(img, gray, COLOR_RGB2GRAY);
    namedWindow("flood fill");
    imshow("flood fill", img);
    imshow("mask", mask_img);
//    namedWindow("mask");
    createTrackbar("threshold", "flood fill", &thres_init_value, 255, onThreshold);
    waitKey(0);
    destroyAllWindows();
}
