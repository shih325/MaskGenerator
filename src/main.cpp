#include <iostream>
#include <QtWidgets/QApplication>
#include "MaskGeneratorView.h"

#include <Utils.h>
using namespace std;
using namespace cv;

/*
 * magic
 */
static int _=[]()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    return 0;
}();

int main(int argc, char *argv[]) {
    //ShowWindow(GetConsoleWindow(), SW_HIDE);
//    QApplication a(argc, argv);
//    MaskGeneratorView w;
//    w.show();
//    return a.exec();
    Mat src = imread("/home/tlss/repos/vrlab/MaskGenerator/pics/im0087.png");
    myDrawContours(src);
    return 0;
}