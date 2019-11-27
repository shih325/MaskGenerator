//
// Created by tlss on 2019/11/26.
//
#include <Utils.h>

int thres_init_value = 130;
int canny_thres = 145;
Mat img;
Mat gray;
Mat threshold_img;
Mat canny_img;

void onCanny(int, void*) {
    Canny(threshold_img, canny_img, canny_thres, canny_thres * 3, 3);
    imshow("threshold", canny_img);
}

void onThreshold(int, void*) {
    threshold(gray, threshold_img, thres_init_value, 255, THRESH_TOZERO);
    vector<vector<Point> > contours;
    findContours(threshold_img, contours, RETR_LIST, CHAIN_APPROX_NONE);
    while (1) {
        Mat img_orgi;
        img.copyTo(img_orgi);
        drawContours(img_orgi, contours, -1, Scalar(0, 255,0), 1);
        imshow("res", img_orgi);
        imshow("img", img);
        cout << "onThreshold while loop is called." << endl;
        char key = waitKey(30) & 0xFF;
        if (key == 27)
            break;
    }
//    destroyWindow("res");
//    imshow("threshold", threshold_img);
}

void myDrawContours(Mat src) {
    img = src;
    cvtColor(img, gray, COLOR_RGB2GRAY);
    imshow("gray", gray);
    namedWindow("threshold");
    namedWindow("res");
//    resizeWindow("threshold", img.size());
    createTrackbar("threshold", "threshold", &thres_init_value, 255, onThreshold);
    // createTrackbar("canny", "threshold", &canny_thres, 255, onCanny);
    // threshold(gray, threshold_img, thres_init_value, 255, THRESH_BINARY);
    // imshow("threshold", threshold_img);

    // vector<vector<Point> > contours;
    // findContours(threshold_img, contours, RETR_LIST, CHAIN_APPROX_NONE);

    // drawContours(img, contours, -1, Scalar(0, 255,0), 1);
    // imshow("res", img);
    // Mat mask = Mat::zeros(img.size(), CV_8UC1);
    // drawContours(mask, contours, -1., Scalar(255), FILLED);

    // vector<vector<Point> >:: const_iterator it = contours.begin();
    // while (it != contours.end()) {
    //     if ((it->size() > 200)) {
    //         it = contours.erase(it);
    //     }
    //     //cout << it->size() << endl;
    //     else it++;
    // }

    // namedWindow("mask");
    // imshow("mask", mask);
    // // Mat crop(gray.size(), CV_8UC1);
    // // gray.copyTo(crop, mask);
    // // imshow("warpped img", crop);
    // Mat res = gray + mask;
    // imshow("res", res);
    waitKey(0);
    destroyAllWindows();
}
