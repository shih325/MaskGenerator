//
// Created by Anna on 2019/11/26.
//

#include "MaskGeneratorView.h"
#include <QFileDialog>
#include <QDialog>
#include <QFileInfo>
#include <QDebug>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>

#include<QDateTime>
/*
 * 1. 直接打开的图片格式:png,jpg,jpeg,bmp,tif
 * 2. json对象设置一个自动保存线程,隔一会er自动保存一下
 */

/*
 * 构造函数
 */
MaskGeneratorView::MaskGeneratorView(QWidget *parent) :QMainWindow(parent)
{
    //TODO:add custom ui setup code.
    ui.setupUi(this);
    //接收拖放
    setAcceptDrops(true);

}
/*
 * Action:打开文件夹
 * 打开文件夹,列出文件夹下的所有文件
 * 如果存在列表文件,则读取列表文件,否则创建列表文件
 * 然后显示尚未处理的第一张图片
 */
void MaskGeneratorView::onActionTriggered_OpenFolder() {
    //打开文件夹
    QFileDialog * fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(QStringLiteral("Choose"));
    fileDialog->setFileMode(QFileDialog::Directory);
    if (fileDialog->exec() == QDialog::Accepted) {
        srcpath = fileDialog->selectedFiles()[0];
        //寻找xml文件
        list_path = srcpath+"/list.xml";
        QFileInfo fileInfo(list_path);
        if(fileInfo.isFile())//xml存在
        {
            //获得文件列表信息,获取当前待处理下标
        }else{//xml不存在
            //遍历文件,获得文件列表信息,初始化当前待处理下标,构造json
            QDir dir(srcpath);
            QStringList nameFilters;//设置文件过滤器
            nameFilters << "*.jpg"<<"*.png"<<"*.jpeg"<<"*.bmp"<<"*.tif";//将过滤后的文件名称存入到files列表中
            QStringList ImageList = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);


            qDebug() <<"num of files:"<<ImageList.size();
            qDebug() <<srcpath;
            for (int i = 0; i < ImageList.size(); ++i) {
                qDebug() << ImageList.at(i);
            }
        }

    }
}
/*
 * Action:打开文件
 */
void MaskGeneratorView::onActionTriggered_OpenFile() {

}
/*
 * Action:保存
 */
void MaskGeneratorView::onActionTriggered_Save() {
    CreateXmlList();
}
/*
 * Action:退出
 */
void MaskGeneratorView::onActionTriggered_Exit() {

}
/*
 * Action:撤销
 */
void MaskGeneratorView::onActionTriggered_UnDo() {

}
/*
 * Action:重做
 */
void MaskGeneratorView::onActionTriggered_ReDo() {

}
/*
 * Action:前一张
 */
void MaskGeneratorView::onActionTriggered_Prior() {

}
/*
 * Action:下一张
 */
void MaskGeneratorView::onActionTriggered_Next() {

}
/*
 * Action:显示原图
 */
void MaskGeneratorView::onActionTriggered_ShowOrigin() {

}
/*
 * Action:显示掩码
 */
void MaskGeneratorView::onActionTriggered_ShowMask() {

}
/*
 * Action:显示透视图
 */
void MaskGeneratorView::onActionTriggered_ShowAlpha() {

}
/*
 * Action:设置
 */
void MaskGeneratorView::onActionTriggered_Settings() {

}
/*
 * Action:帮助
 */
void MaskGeneratorView::onActionTriggered_Help() {

}
/*
 * Action:关于
 */
void MaskGeneratorView::onActionTriggered_About() {

}
/*
 * 创建xml文件
 */
void MaskGeneratorView::CreateXmlList() {
    QFile file("E:/CLion/MaskGenerator/src/1.json");
    if(!file.open(QIODevice::ReadWrite)) {
        qDebug() << "File open error";
    } else {
        qDebug() <<"File open!";
    }

    // 清空文件中的原有内容
    file.resize(0);


    QJsonObject root;//根元素

    QJsonObject metaobj;//元数据
    metaobj.insert("num",900);
    metaobj.insert("current",5);
    //metaobj["num"]="fuck";
    root.insert("metadata",metaobj);

    QJsonArray imgArray;
    for(int i = 0; i < 5; i++) {
        QJsonObject imgObject;
        imgObject.insert("id", QString::number(i+1));
        imgObject.insert("name", QString::number(i+1));
        imgObject.insert("mask", i+18);
        imgObject.insert("label", QDateTime::currentDateTime().toString());
        imgArray.append(imgObject);
    }
    root.insert("imagelist",imgArray);

    QJsonDocument rootDoc;
    rootDoc.setObject(root);

    file.write(rootDoc.toJson());
    file.close();


    qDebug() << "Write to file";

//    QFile loadFile("E:/CLion/MaskGenerator/src/1.json");
//    if(!loadFile.open(QIODevice::ReadOnly)) {
//        qDebug() << "File open error";
//    } else {
//        qDebug() <<"File open!";
//    }
//
//    QByteArray allData = loadFile.readAll();
//    loadFile.close();
//
//    QJsonParseError json_error;
//    QJsonDocument jsonDoc_read(QJsonDocument::fromJson(allData, &json_error));
//
//    if(json_error.error != QJsonParseError::NoError)
//    {
//        qDebug() << "json error!";
//        return;
//    }
//    QJsonObject rootObj = jsonDoc.object();


}
