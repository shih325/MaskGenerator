//
// Created by Anna on 2019/12/1.
//
#include "HistoryLogWidget.h"
#include <QTableWidgetItem>
HistoryLogWidget::HistoryLogWidget(QWidget *parent) : QTableWidget(parent) {
    current = 0;
    total = 0;
    this->setRowCount(2);
    this->setColumnCount(2);   //设置列数为2
    QStringList header;
    header<<"state"<<"message";
    this->setHorizontalHeaderLabels(header);
    this->resizeColumnsToContents();
    this->resizeRowsToContents();
}
/*
 * 添加新记录
 */
void HistoryLogWidget::add(QString text) {
    //如果current不是最后一个,把后面的都删除
    auto p =new QTableWidgetItem(QIcon("E:\\CLion\\MaskGenerator\\src\\resource\\green.ico"),"");
    auto tempitem = new QTableWidgetItem(text);
    itemList.push_back(tempitem);
    this->setItem(current,0,p);
    this->setItem(current,1,tempitem);
    current++;
}
/*
 * 撤销
 */
void HistoryLogWidget::undo() {

}
/*
 * 重做
 */
void HistoryLogWidget::redo() {

}