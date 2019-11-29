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
#include <QMessageBox>
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
    MasterSwitch(false);
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
    fileDialog->setWindowTitle(QStringLiteral("Select the folder where you want to mark pictures"));
    fileDialog->setFileMode(QFileDialog::Directory);
    if (fileDialog->exec() == QDialog::Accepted) {
        srcpath = fileDialog->selectedFiles()[0];
        //寻找json文件
        JsonFile = srcpath + "/list.json";
        QFileInfo fileInfo(JsonFile);

        if(fileInfo.isFile())//json存在
        {
            //对list进行校验,如果校验通过,则允许使用程序的相关功能
            if(CheckJson()){
                MasterSwitch(true);
            }else{
                QMessageBox message(QMessageBox::NoIcon, "error!", "Unknown JSON file!");
                message.exec();
            }
        }else{//json不存在
            //先校验当前文件夹
            QDir dir(srcpath);
            QStringList nameFilters;//设置文件过滤器
            nameFilters << "*.jpg"<<"*.png"<<"*.jpeg"<<"*.bmp"<<"*.tif";//将过滤后的文件名称存入到files列表中
            QStringList ImageList = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
            if(ImageList.size()<=0){
                QMessageBox message(QMessageBox::NoIcon, "error!", "Only \"jpg,png,jpeg,bmp,tif\" files are supported");
                message.exec();
            }else{
                CreateJsonList(ImageList);
                //允许程序的相关功能使用
                MasterSwitch(true);
            }
        }

    }
}
/*
 * Action:打开文件
 */
void MaskGeneratorView::onActionTriggered_OpenFile() {
    //打开文件
    QFileDialog * fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(QStringLiteral("Select the JSON file to store the file list"));
    fileDialog->setNameFilter(QStringLiteral("file list(*.json)"));
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    if (fileDialog->exec() == QDialog::Accepted) {
        JsonFile = fileDialog->selectedFiles()[0];
        QFileInfo fileInfo(JsonFile);
        srcpath=fileInfo.absolutePath();
        if(CheckJson()){
            MasterSwitch(true);
        }else{
            QMessageBox message(QMessageBox::NoIcon, "error!", "Unknown JSON file!");
            message.exec();
        }
    }
}
/*
 * Action:保存
 */
void MaskGeneratorView::onActionTriggered_Save() {
    SaveCurrent("maskfilename","this is a label");

    qDebug()<<getCurrentImageName();
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
 * 创建json文件
 */
void MaskGeneratorView::CreateJsonList(QStringList filelist) {
    QFile file(JsonFile);
    if(!file.open(QIODevice::ReadWrite)) {
        qDebug() << "error when open json: " << JsonFile;
        return;
    }

    file.resize(0); // 清空文件中的原有内容
    QJsonObject root;//根元素
    QJsonObject metaobj;//元数据
    metaobj.insert("num",filelist.size());
    metaobj.insert("current",0);
    root.insert("metadata",metaobj);

    QJsonArray imgArray;
    for(int i = 0; i < filelist.size(); i++) {
        QJsonObject imgObject;
        imgObject.insert("id", QString::number(i));
        imgObject.insert("name", filelist[i]);
        imgObject.insert("mask", "");
        imgObject.insert("label", "");
        imgArray.append(imgObject);
    }
    root.insert("imagelist",imgArray);

    QJsonDocument rootDoc;
    rootDoc.setObject(root);

    file.write(rootDoc.toJson());
    file.close();
}
/*
 * 向json中记录当前文件已经处理完
 */
void MaskGeneratorView::SaveCurrent(QString maskfilename, QString label) {
    QFile file(JsonFile);
    if(!file.open(QIODevice::ReadWrite)) {
        qDebug() << "error when open json: " << JsonFile;
        return;
    }
    QByteArray allData = file.readAll();

    QJsonDocument jdoc(QJsonDocument::fromJson(allData));
    QJsonObject RootObject = jdoc.object();

    QJsonValueRef ImageListJsonRef = RootObject.find("imagelist").value();
    QJsonArray ImageListJson =ImageListJsonRef.toArray();

    QJsonValueRef MetaDateRef = RootObject.find("metadata").value();
    QJsonObject MetaDataObj = MetaDateRef.toObject();
    int current = MetaDataObj["current"].toInt();

    //修改list中的下标为current的项
    QJsonArray::iterator ArrayIterator = ImageListJson.begin();
    QJsonValueRef targetValueRef = ArrayIterator[current];
    QJsonObject targetObject =targetValueRef.toObject();
    targetObject["mask"]= maskfilename;
    targetObject["label"] = label;
    targetValueRef=targetObject;
    ImageListJsonRef=ImageListJson;

    //修改current的值
    MetaDataObj["current"]=current+1;
    MetaDateRef=MetaDataObj;

    //写回到文件
    file.resize(0);
    file.write(QJsonDocument(RootObject).toJson());
    file.close();
}
/*
 * 获取当前要处理的图片文件名
 */
QString MaskGeneratorView::getCurrentImageName() {
    QFile file(JsonFile);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "error when open json: " << JsonFile;
        return QString::null;
    }
    QByteArray allData = file.readAll();
    file.close();
    QJsonDocument jdoc(QJsonDocument::fromJson(allData));
    QJsonObject RootObject = jdoc.object();

    QJsonValueRef ImageListJsonRef = RootObject.find("imagelist").value();
    QJsonArray ImageListJson =ImageListJsonRef.toArray();

    QJsonValueRef MetaDateRef = RootObject.find("metadata").value();
    QJsonObject MetaDataObj = MetaDateRef.toObject();
    int current = MetaDataObj["current"].toInt();

    QJsonArray::iterator ArrayIterator = ImageListJson.begin();
    QJsonValueRef targetValueRef = ArrayIterator[current];
    QJsonObject targetObject =targetValueRef.toObject();

    return targetObject["name"].toString();
}
/*
 * 总开关
 */
void MaskGeneratorView::MasterSwitch(bool status) {

}
/*
 * 校验Json文件
 */
bool MaskGeneratorView::CheckJson() {
    QFile file(JsonFile);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "error when open json: " << JsonFile;
        return false;
    }
    try {
        QByteArray allData = file.readAll();
        file.close();
        QJsonDocument jdoc(QJsonDocument::fromJson(allData));
        QJsonObject RootObject = jdoc.object();

        if(!RootObject.contains("imagelist")||!RootObject.contains("metadata")){
           return false;
        }
    }catch (...){
        return false;
    }

    return true;
}
