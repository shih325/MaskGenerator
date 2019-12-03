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
#include <QGraphicsScene>
#include <QPixmap>
#include <QDateTime>
#include "Tools.h"
#include "Utils.h"
/*
 * 1. 直接打开的图片格式:png,jpg,jpeg,bmp,tif
 */

/*
 * 构造函数
 */
MaskGeneratorView::MaskGeneratorView(QWidget *parent) :QMainWindow(parent)
{
    //TODO:add custom inits.
	this->qimage_to_show = new QImage();
	this->history = new History();
    //TODO:add custom ui setup code.
    ui.setupUi(this);
    this->m_GraphicsView = new MyGraphicsView();
    ui.imgLayout->addWidget(m_GraphicsView);
    connect(m_GraphicsView, SIGNAL(mouseWheelZoom(int)), this, SLOT(onMouseWheelZoom(int)));

    this->m_HistoryLogWidget=new HistoryLogWidget(this);
    ui.historyLayout->addWidget(m_HistoryLogWidget);
    m_HistoryLogWidget->show();

    this->m_PosLabel =new QLabel("?");
    this->m_PosLabel->setMinimumSize(m_PosLabel->sizeHint());
    this->m_PosLabel->setAlignment(Qt::AlignHCenter);
    ui.statusbar->addWidget(this->m_PosLabel);
    ui.statusbar->setStyleSheet(QString("QStatusBar::item{border: 0px}"));
    ui.action_show_origin->setChecked(true);
    //接收拖放
    setAcceptDrops(true);
    MasterSwitch(false);
}
/*
 * Action:打开文件夹
 */
void MaskGeneratorView::onActionTriggered_OpenFolder() {
    /*
     * 打开文件夹,列出文件夹下的所有文件
     * 如果存在列表文件,则读取列表文件,否则创建列表文件
     * 然后显示尚未处理的第一张图片
     */
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
                JobStart();
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
                JobStart();
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
            JobStart();
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
//    QString savedMaskName = getCurrentImageName().split('.')[0]+"_mask.png";
//    QFileInfo prefix(srcpath);
//    std::string savedMaskPath = (prefix.absolutePath()+"/masks/"+savedMaskName).toStdString();
//    cv::Mat _mask=this->mask->clone();
//    auto _mask_orgi_size = _mask(cv::Rect(1,1, this->target->cols, this->target->rows));
////    if ((prefix.absolutePath()+"/masks/"+savedMaskName).toStdString().empty()) {
//    if (savedMaskPath.empty()) {
//        QMessageBox message(QMessageBox::NoIcon, "Error!", "Saved Failed: saved image path is empty!");
//        message.exec();
//        return ;
//    }
//    if (_mask_orgi_size.empty()) {
//        QMessageBox message(QMessageBox::NoIcon, "Error!", "Saved Failed: mask is empty!");
//        message.exec();
//        return ;
//    }
//    SaveCurrent(savedMaskName, ui.textEdit_label->toPlainText());
//    cv::imwrite(savedMaskPath, _mask_orgi_size);
    saveMaskImage();
    this->history->savedATop();
    m_HistoryLogWidget->clear();
    JobStart();
//    qDebug()<<getCurrentImageName();
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
	HistoryData * history_data=new HistoryData;
    if(this->history->undo(history_data)){
        this->threshold=history_data->threshold;
        ui.verticalSlider_threshold->setValue(this->threshold);

        *this->working_img=*history_data->workingImg;
        *this->mask=*history_data->maskImg;
        showMat(*this->working_img);
        this->m_HistoryLogWidget->undo();
    }
}
/*
 * Action:重做
 */
void MaskGeneratorView::onActionTriggered_ReDo() {
	HistoryData * history_data = new HistoryData;
	if (this->history->redo(history_data)) {
		this->threshold = history_data->threshold;
		ui.verticalSlider_threshold->setValue(this->threshold);

		*this->working_img = *history_data->workingImg;
		*this->mask = *history_data->maskImg;
		showMat(*this->working_img);
		this->m_HistoryLogWidget->redo();
	}
}
/*
 * Action:前一张
 */
void MaskGeneratorView::onActionTriggered_Prior() {
    // if the image has been masked
    if (m_HistoryLogWidget->getCurrentValue() != 0) {
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
            case QMessageBox::Save:
                saveMaskImage();
                break;
//            case QMessageBox::Discard:
//                break;
            case QMessageBox::Cancel:
                return ;
            default:
                break;
        }
        this->m_Direction = PREV_PREV;
        setCurrentValue();
        this->history->savedATop();
        m_HistoryLogWidget->clear();
        JobStart();
    }
    else {
        this->m_Direction = PREV;
        setCurrentValue();
        this->history->savedATop();
        m_HistoryLogWidget->clear();
        JobStart();
    }
}
/*
 * Action:下一张
 */
void MaskGeneratorView::onActionTriggered_Next() {
    // if the image has been masked
    if (m_HistoryLogWidget->getCurrentValue() != 0) {
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
            case QMessageBox::Save:
                saveMaskImage();
                break;
//            case QMessageBox::Discard:
//                break;
            case QMessageBox::Cancel:
                return ;
            default:
                break;
        }
        // this->m_Direction = NEXT;
        // setCurrentValue();
        this->history->savedATop();
        m_HistoryLogWidget->clear();
        JobStart();
    }
    else {
        this->m_Direction = NEXT;
        setCurrentValue();
        this->history->savedATop();
        m_HistoryLogWidget->clear();
        JobStart();
    }
}
/*
 * Action:显示原图
 */
void MaskGeneratorView::onActionTriggered_ShowOrigin() {
    ui.action_show_origin->setChecked(true);
    ui.action_show_mask->setChecked(false);
    this->m_DisplayMode=ORIGIN;
    updateUI();
}
/*
 * Action:显示掩码
 */
void MaskGeneratorView::onActionTriggered_ShowMask() {
    ui.action_show_origin->setChecked(false);
    ui.action_show_mask->setChecked(true);
    this->m_DisplayMode=MASK;
    updateUI();
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
    int total = MetaDataObj["num"].toInt();

    //修改list中的下标为current的项
    QJsonArray::iterator ArrayIterator = ImageListJson.begin();
    QJsonValueRef targetValueRef = ArrayIterator[current];
    QJsonObject targetObject =targetValueRef.toObject();
    targetObject["mask"]= maskfilename;
    targetObject["label"] = label;
    targetValueRef=targetObject;
    ImageListJsonRef=ImageListJson;

    //修改current的值
    if (current == total-1) MetaDataObj["current"] = current;
    else MetaDataObj["current"]=current+1;
    // MetaDataObj["current"]=current+1;
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
        return QString();
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
    std::cout << current << std::endl;

    QJsonArray::iterator ArrayIterator = ImageListJson.begin();
    QJsonValueRef targetValueRef = ArrayIterator[current];
    QJsonObject targetObject =targetValueRef.toObject();

    return targetObject["name"].toString();
}
/*
 * 总开关
 */
void MaskGeneratorView::MasterSwitch(bool status) {
    ui.verticalSlider_threshold->setEnabled(status);
    ui.action_show_mask->setEnabled(status);
    ui.action_show_origin->setEnabled(status);
    ui.action_UnDo->setEnabled(status);
    ui.action_Redo->setEnabled(status);
    ui.action_save->setEnabled(status);
    ui.action_prior->setEnabled(status);
    ui.action_next->setEnabled(status);
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
/*
 * 加载第一章图片,初始化相关的变量,开始工作
 */
bool MaskGeneratorView::JobStart() {
    //获取当前图片文件名
    QString imgfilename = getCurrentImageName();
    this->target = new cv::Mat(cv::imread((srcpath+"/"+imgfilename).toStdString()));
    if(this->target->empty()){
        return false;
    }
    //给working和mask赋值
    this->working_img=new cv::Mat(this->target->clone());
    this->mask = new cv::Mat(cv::Mat::zeros(this->target->rows+2, this->target->cols+2, CV_8UC1));
    //显示working
    updateUI();
    //历史记录初始化
    auto * initData = new HistoryData(this->threshold,this->working_img,this->mask);
    this->history->clear();
    this->history->add(initData);
    // m_HistoryLogWidget->clear();
    m_HistoryLogWidget->add("Open img");
    return true;
}
/*
 * Action: 测试1
 */
void MaskGeneratorView::onActionTriggered_Test_1() {
    /*cv::Mat src = cv::imread("E:\\CLion\\MaskGenerator\\Example\\images\\im0041.png");
    myDrawContours(src);*/
    this->m_HistoryLogWidget->add("hello world");
}
/*
 * Action: 测试2
 */
void MaskGeneratorView::onActionTriggered_Test_2() {
    this->m_HistoryLogWidget->undo();
}
/*
 * Action: 测试3
 */
void MaskGeneratorView::onActionTriggered_Test_3() {
    this->m_HistoryLogWidget->redo();
}
/*
 * 响应graphics区域的鼠标滚轮动作:缩放图片
 */
void MaskGeneratorView::onMouseWheelZoom(int delta) {
    //qDebug() << QString::fromStdString(std::to_string(delta));
    if (delta > 0 && scaleFactor >= 0)
    {
        scaleFactor += 5;
        QMatrix old_matrix;
        old_matrix = m_GraphicsView->matrix();
        m_GraphicsView->resetMatrix();
        m_GraphicsView->translate(old_matrix.dx(), old_matrix.dy());
        m_GraphicsView->scale(scaleFactor / 100.0, scaleFactor / 100.0);
    }
    else if (delta < 0 && scaleFactor >= 0)
    {
        scaleFactor -= 5;
        QMatrix old_matrix;
        old_matrix = m_GraphicsView->matrix();
        m_GraphicsView->resetMatrix();
        m_GraphicsView->translate(old_matrix.dx(), old_matrix.dy());
        m_GraphicsView->scale(scaleFactor / 100.0, scaleFactor / 100.0);
    }
    else if (scaleFactor < 0) {
        scaleFactor = 0.0;
    }
}
/*
 * 显示图片
 * 
 * 这里有内存泄漏!
 */
void MaskGeneratorView::showMat(cv::Mat img) {
    if(!img.empty()){
        *qimage_to_show = Tools::cvMat2QImage(img);
        m_GraphicsScene = new MyQGraphicsScene();
        m_GraphicsItem = new MyQGraphicsPixmapItem();
        connect(m_GraphicsItem, SIGNAL(mouseLeftDown(int, int)), this, SLOT(onMouseLeftDown(int, int)));
        connect(m_GraphicsItem, SIGNAL(mouseMoved(int, int)), this, SLOT(onMouseMoved(int, int)));
        m_GraphicsItem->setPixmap(QPixmap::fromImage(*qimage_to_show));
        //m_GraphicsScene->addPixmap(QPixmap::fromImage(*qimage_to_show));
        m_GraphicsScene->addItem(m_GraphicsItem);
        m_GraphicsView->setScene(m_GraphicsScene);
        m_GraphicsView->show();
    }
}
/*
 * 响应阈值滑动条的运动
 */
void MaskGeneratorView::onValueChanged_threshold(int value) {
    if(this->threshold!=value){
        this->threshold = value;
        workingImgRefresh();
    }
}
/*
 * 响应图片上的左键按下事件
 */
void MaskGeneratorView::onMouseLeftDown(int x, int y)
{
    if(this->m_DisplayMode==ORIGIN){
        cv::Point seed = cv::Point(x, y);
        cv::Scalar fill_color = cv::Scalar(255, 0, 0);
        cv::Rect ccomp;
        int flags = 4 | 0 | (255 << 8);  //四联通 | ??  | 填充颜色
        //working_img 和 mask都会被改
        floodFill(*this->working_img,*this->mask,
                  seed, fill_color, &ccomp, cv::Scalar(20, 20, 5), cv::Scalar(20, 20, 5), flags);
        showMat(*this->working_img);
        updateUI();
        auto history_data=new HistoryData(this->threshold,this->working_img,this->mask);
        this->history->add(history_data);
        QString msg;
        msg.sprintf("Mark at (%d,%d)", x,y);
        m_HistoryLogWidget->add(msg);
    }
}

/*
 * 刷新工作图片
 */
void MaskGeneratorView::workingImgRefresh() {
    //获取一张原始图像的灰度化版本
    cv::Mat gray,threshold_img;
    cv::cvtColor(*this->target, gray, cv::COLOR_RGB2GRAY);
    //使用当前的threshold进行处理


    cv::threshold(gray, threshold_img, this->threshold, 255, cv::THRESH_TOZERO);
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(threshold_img, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
    //workingimg赋值为原始图加上边缘
	*this->working_img = this->target->clone();//要深拷贝,防止元数据污染
	//把之前画好的mask叠加过来
	cv::Mat _mask=this->mask->clone();
    auto _mask_orgi_size = _mask(cv::Rect(1,1, this->target->cols, this->target->rows));
    this->working_img->setTo(cv::Scalar(255,0,0),_mask_orgi_size);
//    cv::Mat pure_blue = cv::Mat(this->target->rows, this->target->cols, CV_8UC3, cv::Scalar(255, 0, 0));
//    pure_blue.copyTo(*this->working_img,_mask_orgi_size);
    drawContours(*this->working_img, contours, -1, cv::Scalar(0, 255,0), 1);
    updateUI();
}
/*
 * 响应鼠标移动
 */
void MaskGeneratorView::onMouseMoved(int x, int y) {
    QString msg;
    msg.sprintf("(%d,%d)", x,y);
    this->m_PosLabel->setText(msg);
    this->m_PosLabel->show();
}
/*
 * 更新ui
 */
void MaskGeneratorView::updateUI() {
    switch(this->m_DisplayMode){
        case ORIGIN:
            showMat(*working_img);
            break;
        case MASK:
            showMat(*mask);
            break;
    }
}

void MaskGeneratorView::setCurrentValue() {
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
    int total = MetaDataObj["num"].toInt();

    //修改current的值
    switch(this->m_Direction) {
        case PREV_PREV:
            // check the low boundary
            if (current == 0) MetaDataObj["current"] = current;
            else MetaDataObj["current"]=current-2;
            break;
        case PREV:
            // check the low boundary
            if (current == 0) MetaDataObj["current"] = current;
            else MetaDataObj["current"]=current-1;
            break;
        case NEXT:
            // check the high boundary
            if (current == total-1) MetaDataObj["current"] = current;
            else MetaDataObj["current"]=current+1;
            break;
        default:
            break;
    }
    MetaDateRef=MetaDataObj;

    //写回到文件
    file.resize(0);
    file.write(QJsonDocument(RootObject).toJson());
    file.close();
}

void MaskGeneratorView::saveMaskImage() {
    QString savedMaskName = getCurrentImageName().split('.')[0]+"_mask.png";
    QDir dir(srcpath);
    std::string savedMaskPath;
    if (dir.cdUp()) {
        QString savedMaskPrefix = dir.absolutePath()+"/masks/";
        QDir dir(savedMaskPrefix);
        if (!dir.exists()) {
            dir.mkdir(savedMaskPrefix);
        } // if masks dir doesn't exist, then create the mask dir;
        savedMaskPath = (savedMaskPrefix+savedMaskName).toStdString();
        std::cout << savedMaskPath << std::endl;
        if (savedMaskPath.empty()) {
            QMessageBox message(QMessageBox::NoIcon, "Error!", "Saved Failed: saved image path is empty!");
            message.exec();
            return ;
        }
    } // parent dir exists;
    else { return ; } // parent dir not exists;
    cv::Mat _mask=this->mask->clone();
    auto _mask_orgi_size = _mask(cv::Rect(1,1, this->target->cols, this->target->rows));
    if (_mask_orgi_size.empty()) {
        QMessageBox message(QMessageBox::NoIcon, "Error!", "Saved Failed: mask is empty!");
        message.exec();
        return ;
    }
    SaveCurrent(savedMaskName, ui.textEdit_label->toPlainText());
    cv::imwrite(savedMaskPath, _mask_orgi_size);
    // this->history->clear();
}

