//
// Created by Anna on 2019/11/26.
//
#include "Tools.h"
#include <QPixmap>
#include <QFont>
/*
 * cv::Mat转换为QImage
 */
QImage Tools::cvMat2QImage(const cv::Mat &mat) {
    cv::Mat mat_8= mat;
    //switch (mat.channels())
    //{
    //    case 1:
    //        mat.convertTo(mat_8, CV_8UC1);
    //        break;
    //    case 2:
    //        mat.convertTo(mat_8, CV_8UC2);
    //        break;
    //    case 3:
    //        mat.convertTo(mat_8, CV_8UC3);
    //        break;
    //    case 4:
    //        mat.convertTo(mat_8, CV_8UC4);
    //        break;
    //}
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if (mat_8.type() == CV_8UC1)
    {
        QImage image(mat_8.cols, mat_8.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for (int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat_8.data;
        for (int row = 0; row < mat_8.rows; row++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat_8.cols);
            pSrc += mat_8.step;
        }
        return image;
    }
        // 8-bits unsigned, NO. OF CHANNELS = 3
    else if (mat_8.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat_8.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat_8.cols, mat_8.rows, mat_8.step, QImage::Format_RGB888);
        return image;//.rgbSwapped()
    }
    else if (mat_8.type() == CV_8UC4)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat_8.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat_8.cols, mat_8.rows, mat_8.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        return getDefaultImage();
    }
}
/*
 * QImage转换为cv::Mat
 */
cv::Mat Tools::QImage2cvMat(QImage image) {
    cv::Mat mat;
    //qDebug() << qimage_to_show.format();
    switch (image.format())
    {
        case QImage::Format_ARGB32:
        case QImage::Format_RGB32:
        case QImage::Format_ARGB32_Premultiplied:
            mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
            break;
        case QImage::Format_RGB888:
            mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
            cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
            break;
        case QImage::Format_Indexed8:
            mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
            break;
    }
    return mat;
}
/*
* 把img显示在view上
*/
void Tools::ShowImageOnUi(cv::Mat &img, QGraphicsScene *sense, QGraphicsView *view) {
    sense->clear();
    sense->addPixmap(QPixmap::fromImage(cvMat2QImage(img)));
    view->setScene(sense);
    view->fitInView(sense->itemsBoundingRect(), Qt::KeepAspectRatio);
    view->centerOn(0, 0);
    view->show();
    view->update();
}
/*
* 创建并返回idle qimage_to_show
*/
QImage Tools::getDefaultImage() {
    //图片上的字符串，例如 HELLO
    QString imageText = "JLU CGDM";//Computer graphics and digital media
    QFont font;

    font.setPixelSize(13);//设置显示字体的大小


    QFontMetrics fm(font);
    int charWidth = fm.width(imageText);
    charWidth = fm.boundingRect(imageText).width();

    QSize size(charWidth + 8, 15);//指定图片大小为字体的大小
    //QSize size(1920, 1080);//指定图片大小为字体的大小


    //以ARGB32格式构造一个QImage
    QImage image(size, QImage::Format_ARGB32);
    //填充图片背景,120/250为透明度
    image.fill(qRgba(255, 255, 255, 0));


    //为这个QImage构造一个QPainter
    QPainter painter(&image);
    //设置画刷的组合模式CompositionMode_SourceOut这个模式为目标图像在上。
    //改变组合模式和上面的填充方式可以画出透明的图片。
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);


    //改变画笔和字体
    QPen pen = painter.pen();
    pen.setColor(QColor(0, 141, 239));


    painter.setPen(pen);
    painter.setFont(font);


    //将Hello写在Image的中心
    painter.drawText(image.rect(), Qt::AlignCenter, imageText);
    return image;
}
