//
// Created by Anna on 2019/11/26.
//

#ifndef MASKGENERATOR_TOOLS_H
#define MASKGENERATOR_TOOLS_H


#include <QtGui/qimage.h>
#include <opencv2/opencv.hpp>
#include <QGraphicsScene>
#include <QGraphicsView>

class Tools {
    /*
	 * cv::Mat转换为QImage
	 */
    static QImage cvMat2QImage(const cv::Mat& mat);

    /*
     * QImage转换为cv::Mat
     */
    static cv::Mat QImage2cvMat(QImage image);

    /*
    * 把img显示在view上
    */
    static void ShowImageOnUi(cv::Mat& img, QGraphicsScene*sense, QGraphicsView* view);

    /*
    * 创建并返回idle image
    */
    static QImage getDefaultImage();
};


#endif //MASKGENERATOR_TOOLS_H
