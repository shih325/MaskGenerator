//
// Created by Anna on 2019/12/1.
//

#ifndef MASKGENERATOR_HISTORYLOGWIDGET_H
#define MASKGENERATOR_HISTORYLOGWIDGET_H
#include <QTableWidget>
#include <QWidget>
#include <QString>
/*
 * 实现历史记录列表
 * 当前状态对应的记录前显示一个标志
 * 可以undo的记录显示为粗体
 * 可以redo的记录显示为斜体
 * 无论何时,调用add都会导致可以redo的记录失效
 */
class HistoryLogWidget :public QTableWidget{
    Q_OBJECT
public:
    HistoryLogWidget(QWidget*parent= nullptr);
public:
    void add(QString text);
    void redo();
    void undo();
    void clear();
    int getCurrentValue();
private:
    std::vector<QTableWidgetItem* > itemList;
    int current;
};


#endif //MASKGENERATOR_HISTORYLOGWIDGET_H
