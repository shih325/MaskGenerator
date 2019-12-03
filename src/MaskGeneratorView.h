//
// Created by Anna on 2019/11/26.
//

#ifndef MASKGENERATOR_MASKGENERATORVIEW_H
#define MASKGENERATOR_MASKGENERATORVIEW_H
#include <QtWidgets/QMainWindow>
#include "ui_MaskGenerator.h"
#include "opencv2/opencv.hpp"
#include "History.h"
#include <QImage>
#include "MyGraphicsView.h"
#include "HistoryLogWidget.h"
#include <QLabel>

class MaskGeneratorView :public QMainWindow{
    Q_OBJECT
public:
    MaskGeneratorView(QWidget *parent = Q_NULLPTR);
private:
    Ui::MaskGenerator ui;
    QString srcpath;                                        //待标注文件所在的路径
    QString JsonFile;                                       //列表文件所在的路径

    cv::Mat * target;                                       //当前正在标定的源图片(不更改)
    cv::Mat * working_img;                                  //中间结果(更新,不保存到文件,需要历史记录)
    cv::Mat * mask;                                         //掩码(更新,保存到文件,需要历史记录)
    int threshold = 0;                                      //边缘检测阈值(需要历史记录)

    History * history;                                      //用于记录历史以及支持撤销重做功能

    QImage * qimage_to_show;
    MyGraphicsView * m_GraphicsView = nullptr;
    MyQGraphicsScene * m_GraphicsScene = nullptr;
	MyQGraphicsPixmapItem * m_GraphicsItem = nullptr;
    float scaleFactor = 100;
    HistoryLogWidget * m_HistoryLogWidget = nullptr;
    QLabel * m_PosLabel = nullptr;
    enum DISPLAY_MODE {ORIGIN,MASK};
    DISPLAY_MODE m_DisplayMode = ORIGIN;
    enum DIRECTION {PREV_PREV, PREV, NEXT};                              // json 'current' value update direction;
    DIRECTION m_Direction = NEXT;

    void CreateJsonList(QStringList filelist);              //初始化构造json文件
    void SaveCurrent(QString maskfilename,QString label);   //向json中记录当前图片已经处理完
    QString getCurrentImageName();                          //获取当前要处理的文件名
    void setCurrentValue();                                 // set 'current' value in json
    bool CheckJson();                                       //校验json文件
    void MasterSwitch(bool status);                         //程序功能总开关,用于控制界面控件的可访问性.
    bool JobStart();                                        //加载第一章图片,初始化相关的变量,开始工作
    void workingImgRefresh();                               //刷新工作图片
    void showMat(cv::Mat img);                              //把Mat显示到UI上
    void updateUI();                                        //更新ui上图像显示区域
    void saveMaskImage();
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

    void onActionTriggered_Settings();
    void onActionTriggered_Help();
    void onActionTriggered_About();

    void onActionTriggered_Test_1();
    void onActionTriggered_Test_2();
    void onActionTriggered_Test_3();
    void onMouseWheelZoom(int delta);
	
    void onValueChanged_threshold(int value);
	void onMouseLeftDown(int x, int y);
	void onMouseMoved(int x,int y);
};


#endif //MASKGENERATOR_MASKGENERATORVIEW_H
