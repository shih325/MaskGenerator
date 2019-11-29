//
// Created by Anna on 2019/11/29.
//

#include "History.h"
/*
 * 构造函数
 */
History::History() {
    this->stackA =new std::stack<cv::Mat>();
    this->stackB =new std::stack<cv::Mat>();
    /*
     * A栈顶端是当前显示在屏幕上的结果
     */
}
/*
 * 析构函数
 */
History::~History() {
    delete this->stackA;
    delete this->stackB;
}
/*
 * 添加一个新操作,压入A栈,如果B栈非空,则删空它
 */
void History::add(cv::Mat img) {
    this->stackA->push(img);
    if(!this->stackB->empty()){
        delete this->stackB;
        this->stackB = new std::stack<cv::Mat>();
    }
}
/*
 * 撤销
 * 将当前A栈顶端的操作保存在B栈中
 * A栈弹出顶端元素
 * 返回A栈当前顶端元素
 *
 */
bool History::undo(cv::Mat *img) {
    if(this->stackA->empty()){
        return false;
    }else{
        this->stackB->push(stackA->top());
        this->stackA->pop();
        *img = this->stackA->top();
        return true;
    }
}
/*
 * 重做
 * 返回B栈顶端元素,并压入A栈
 * B栈弹出顶端元素
 *
 */
bool History::redo(cv::Mat *img) {
    if(this->stackB->empty()){
        return false;
    }else{
        *img = this->stackB->top();
        this->stackA->push(*img);
        this->stackB->pop();
    }
    return true;
}
