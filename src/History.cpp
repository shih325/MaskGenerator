//
// Created by Anna on 2019/11/29.
//

#include "History.h"
/*
 * ==================
 * class HistoryData
 * ==================
 */

/*
 * 构造函数
 */
HistoryData::HistoryData() {
    this->threshold = 0;
    this->maskImg = nullptr;
    this->workingImg = nullptr;
}
HistoryData::HistoryData(int value,cv::Mat * working,cv::Mat * mask) {
    this->threshold = value;
    this->maskImg = new cv::Mat(mask->clone());
    this->workingImg = new cv::Mat(working->clone());
}
/*
 * 析构函数
 */
HistoryData::~HistoryData() {
    delete this->maskImg;
    delete this->workingImg;
}
/*
 * 拷贝构造函数
 */
HistoryData::HistoryData(const HistoryData &data) {
    this->threshold = data.threshold;
    this->maskImg = new cv::Mat(data.maskImg->clone());
    this->workingImg = new cv::Mat(data.workingImg->clone());
}
/*
 * 拷贝赋值运算符
 */
HistoryData &HistoryData::operator=(const HistoryData &data) {
    this->threshold = data.threshold;
    this->maskImg = new cv::Mat(data.maskImg->clone());
    this->workingImg = new cv::Mat(data.workingImg->clone());
    return *this;
}
/*
 * clone
 */
HistoryData *HistoryData::clone() {
    return new HistoryData(this->threshold,this->workingImg,this->maskImg);
}



/*
 * ================================
 * class History
 * ================================
 */

/*
 * 构造函数
 */
History::History() {
    this->stackA =new std::stack<HistoryData*>();
    this->stackB =new std::stack<HistoryData*>();
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
void History::add(HistoryData* data) {
    this->stackA->push(data);
    while (!this->stackB->empty()){
        delete this->stackB->top();
        this->stackB->pop();
    }
    delete this->stackB;
    this->stackB = new std::stack<HistoryData*>();
}
/*
 * 撤销
 * 将当前A栈顶端的操作保存在B栈中
 * A栈弹出顶端元素
 * 返回A栈当前顶端元素
 *
 */
bool History::undo(HistoryData* data) {
    if(this->stackA->size()<=1){//
        return false;
    }else{
        this->stackB->push(stackA->top()->clone());
        this->stackA->pop();
        data->maskImg = this->stackA->top()->maskImg;
		data->workingImg = this->stackA->top()->workingImg;
		data->threshold = this->stackA->top()->threshold;
        return true;
    }
}
/*
 * 重做
 * 返回B栈顶端元素,并压入A栈
 * B栈弹出顶端元素
 *
 */
bool History::redo(HistoryData* data) {
    if(this->stackB->empty()){
        return false;
    }else{
        //data = this->stackB->top();
		data->maskImg = this->stackB->top()->maskImg;
		data->workingImg = this->stackB->top()->workingImg;
		data->threshold = this->stackB->top()->threshold;
        this->stackA->push(data->clone());
        this->stackB->pop();
    }
    return true;
}



