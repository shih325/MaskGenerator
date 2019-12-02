//
// Created by Anna on 2019/11/29.
// 实现撤销和重做的功能
//

#ifndef MASKGENERATOR_HISTORY_H
#define MASKGENERATOR_HISTORY_H

#include <iostream>
#include <stack>
#include "opencv2/opencv.hpp"

class HistoryData{
public:
    HistoryData();
    HistoryData(int value,cv::Mat * working,cv::Mat * mask);
    ~HistoryData();
    HistoryData(const HistoryData& data);
    HistoryData * clone ();
    HistoryData &operator=(const HistoryData& data);

    cv::Mat * workingImg;
    cv::Mat * maskImg;
    int threshold;
};

class History {
public:
    History();
    ~History();
//private:
    std::stack<HistoryData*> * stackA;
    std::stack<HistoryData*> * stackB;

public:
    void add(HistoryData* data);
    bool undo(HistoryData* data);
    bool redo(HistoryData* data);
    void savedATop();               // saved the top element of stack A, prepare for clear();
    void clear();
};


#endif //MASKGENERATOR_HISTORY_H
