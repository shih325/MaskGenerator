//
// Created by Anna on 2019/11/29.
// 实现撤销和重做的功能
//

#ifndef MASKGENERATOR_HISTORY_H
#define MASKGENERATOR_HISTORY_H
#include <iostream>
#include <stack>
#include "opencv2/opencv.hpp"

class History {
public:
    History();
    ~History();
private:
    std::stack<cv::Mat> * stackA;
    std::stack<cv::Mat> * stackB;

public:
    void add(cv::Mat img);
    bool undo(cv::Mat * img);
    bool redo(cv::Mat * img);
};


#endif //MASKGENERATOR_HISTORY_H
