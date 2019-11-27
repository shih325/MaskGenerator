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
Mat fill_mask_img;

Mat img_orgi;
void onMouse(int event, int x, int y, int flags, void* param) {
    Point seed = Point(x, y);
    Scalar fill_color = Scalar(0, 0, 255);
    Rect ccomp;
	switch (event) {
		case EVENT_LBUTTONDOWN: floodFill(fill_mask_img, seed, fill_color, &ccomp, Scalar(10,10,10), Scalar(10,10,10));
	}
	imshow("flood fill", fill_mask_img);
}

void onCanny(int, void*) {
    Canny(threshold_img, canny_img, canny_thres, canny_thres * 3, 3);
    imshow("threshold", canny_img);
}

void onThreshold(int, void*) {
    threshold(gray, threshold_img, thres_init_value, 255, THRESH_TOZERO);
    vector<vector<Point> > contours;
    findContours(threshold_img, contours, RETR_LIST, CHAIN_APPROX_NONE);
    int cnt = 0;
    while (1) {
        img.copyTo(img_orgi);
        drawContours(img_orgi, contours, -1, Scalar(0, 255,0), 1);
        imshow("res", img_orgi);
        cout << "onThreshold while loop is called." << cnt << endl;
        fill_mask_img = img_orgi;
        setMouseCallback("flood fill", onMouse, 0);
        char key = (char)waitKey(10);
        if (key == 27)
            break;
        cnt++;
    }
//    destroyWindow("res");
//    imshow("threshold", threshold_img);
}

void myDrawContours(Mat src) {
    img = src;
    cvtColor(img, gray, COLOR_RGB2GRAY);
    imshow("gray", gray);
    namedWindow("threshold");
    namedWindow("flood fill");
    // namedWindow("res");
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
