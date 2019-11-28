//
// Created by Anna on 2019/11/26.
//

#ifndef MASKGENERATOR_MASKGENERATORVIEW_H
#define MASKGENERATOR_MASKGENERATORVIEW_H
#include <QtWidgets/QMainWindow>
#include "ui_MaskGenerator.h"

class MaskGeneratorView :public QMainWindow{
    Q_OBJECT
public:
    MaskGeneratorView(QWidget *parent = Q_NULLPTR);
private:
    Ui::MaskGenerator ui;
    QString srcpath;//待标注文件所在的路径
    QString JsonFile;//列表文件所在的路径

    void CreateJsonList(QStringList filelist);//初始化构造json文件
    void SaveCurrent(QString maskfilename,QString label); //向json中记录当前图片已经处理完
    QString getCurrentImageName();  //获取当前要处理的文件名
    bool CheckJson();
    void MasterSwitch(bool status); //程序功能总开关,用于控制界面控件的可访问性.
    bool loadimg(QString imgFileName);  //加载图片文件,准备mask,准备数据库
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

};


#endif //MASKGENERATOR_MASKGENERATORVIEW_H
