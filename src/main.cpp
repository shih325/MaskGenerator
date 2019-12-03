#include <iostream>
#include <QtWidgets/QApplication>
// #include "MaskGeneratorView.h"

#include <Utils.h>
using namespace std;

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
    cv::Mat img = cv::imread("/home/tlss/repos/vrlab/MaskGenerator/Example/images/im0009.png");
    myDrawContours(img);
    return 0;
}
