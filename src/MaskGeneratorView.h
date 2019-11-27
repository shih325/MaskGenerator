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
    QString list_path;//列表文件所在的路径
    QStringList ImageList;//图片文件名列表,不是绝对路径,只有文件名
    int current=0;
    int NumOfImage=0;
    void CreateXmlList();
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
