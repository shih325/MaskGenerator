//
// Created by Anna on 2019/11/26.
//

#ifndef MASKGENERATOR_MASKGENERATORVIEW_H
#define MASKGENERATOR_MASKGENERATORVIEW_H
#include <QtWidgets/QMainWindow>
#include "ui_MaskGenerator.h"
#include "opencv2/opencv.hpp"
#include "History.h"

class MaskGeneratorView :public QMainWindow{
    Q_OBJECT
public:
    MaskGeneratorView(QWidget *parent = Q_NULLPTR);
private:
    Ui::MaskGenerator ui;
    QString srcpath;//待标注文件所在的路径
    QString JsonFile;//列表文件所在的路径

    cv::Mat * target;      //当前正在标定的源图片(不更改)
    cv::Mat * working_img; //中间结果(更新,不保存到文件,需要历史记录)
    cv::Mat * mask;        //掩码(更新,保存到文件)
    History * history;     //历史记录


    void CreateJsonList(QStringList filelist);//初始化构造json文件
    void SaveCurrent(QString maskfilename,QString label); //向json中记录当前图片已经处理完
    QString getCurrentImageName();  //获取当前要处理的文件名
    bool CheckJson();
    void MasterSwitch(bool status); //程序功能总开关,用于控制界面控件的可访问性.
    bool loadimg(QString imgFileName);  //加载图片文件
public slots:
    void onActionTriggered_OpenFolder();
    void onActionTriggered_OpenFile();
    void onActionTriggered_Save();
    void onActionTriggered_Exit();
    void onActionTriggered_UnDo();
    void onActionTriggered_ReDo();
    void onActionTriggered_Prior();
    void onActionTriggered_Next();

    void onActionTriggered_ShowOrigin();
    void onActionTriggered_ShowMask();
    void onActionTriggered_ShowAlpha();

    void onActionTriggered_Settings();
    void onActionTriggered_Help();
    void onActionTriggered_About();

    void onActionTriggered_Test();

};


#endif //MASKGENERATOR_MASKGENERATORVIEW_H
