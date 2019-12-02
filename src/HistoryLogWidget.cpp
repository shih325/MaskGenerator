//
// Created by Anna on 2019/12/1.
//
#include "HistoryLogWidget.h"
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QAbstractItemView>
#include <iostream>

HistoryLogWidget::HistoryLogWidget(QWidget *parent) : QTableWidget(parent) {
    current = -1;//
    this->setColumnCount(2);   //设置列数为2
    this->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents); //与内容一致
    this->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch); //自适应列宽
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    QStringList header;
    header<<"*"<<"message";
    this->setHorizontalHeaderLabels(header);
    this->resizeColumnsToContents();
    this->resizeRowsToContents();
}
/*
 * 添加新记录
 */
void HistoryLogWidget::add(QString text) {
    //如果current不是最后一个,把后面的都删除
    if(current<itemList.size()-1){
        auto iter =itemList.begin()+current+1;
        int irow=current+1;
        while(iter!=itemList.end()){
            iter=itemList.erase(iter);
            this->removeRow(irow);
            irow++;
        }
    }
    //如果current不是第一个,修改图标
    if(current>=0){
        auto oldIcon=new QTableWidgetItem(QIcon("E:\\CLion\\MaskGenerator\\src\\resource\\gray.ico"),"");
        this->setItem(current,0,oldIcon);
    }

    //创建新的一行
    auto newIcon =new QTableWidgetItem(QIcon("E:\\CLion\\MaskGenerator\\src\\resource\\green.ico"),"");
    auto newMsg = new QTableWidgetItem(text);
    itemList.push_back(newMsg);

    //调整完毕,行数更新
    this->setRowCount(itemList.size());
    //显示新的行
    current++;
    this->setItem(current,0,newIcon);
    this->setItem(current,1,newMsg);
    this->clearSelection();
    this->selectRow(current);
}
/*
 * 撤销
 */
void HistoryLogWidget::undo() {
    //itemList长度大于1才可以撤销
    if(itemList.size()>1&&current>0){
        //current改成红色图标
        auto red =new QTableWidgetItem(QIcon("E:\\CLion\\MaskGenerator\\src\\resource\\red.ico"),"");
        this->setItem(current,0,red);
        current--;
        //current改成绿色图标
        auto green =new QTableWidgetItem(QIcon("E:\\CLion\\MaskGenerator\\src\\resource\\green.ico"),"");
        this->setItem(current,0,green);
        this->clearSelection();
        this->selectRow(current);
    }
}
/*
 * 重做
 */
void HistoryLogWidget::redo() {
    //current<item
    if(current<itemList.size()-1&&current>=0){
        //current改成灰色
        auto gray = new QTableWidgetItem(QIcon("E:\\CLion\\MaskGenerator\\src\\resource\\gray.ico"),"");
        this->setItem(current,0,gray);
        current++;
        //current改成绿色
        auto green =new QTableWidgetItem(QIcon("E:\\CLion\\MaskGenerator\\src\\resource\\green.ico"),"");
        this->setItem(current,0,green);
        this->clearSelection();
        this->selectRow(current);
    }
}

/*
 * clear the History Log Widget
 */
void HistoryLogWidget::clear() {
    auto iter = itemList.begin();
    int irow = itemList.size();
    while(iter!=itemList.end()){
        iter=itemList.erase(iter);
        this->removeRow(irow);
        // iter++;
        irow--;
        // std::cout << irow << std::endl;
    }
    current = -1;
}

/*
 * get current value
 */
int HistoryLogWidget::getCurrentValue() {
    return current;
}