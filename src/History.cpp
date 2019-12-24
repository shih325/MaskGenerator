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
    this->iptLasso = 0;
    this->inumLasso = 0;
    this->maskImg = nullptr;
    this->workingImg = nullptr;
}
HistoryData::HistoryData(int value, int ptlassoValue, int numLasso, cv::Mat * working,cv::Mat * mask) {
    this->threshold = value;
    this->iptLasso = ptlassoValue;
    this->inumLasso = numLasso;
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
    // std::cout << "copy construct operator" << std::endl;
    this->threshold = data.threshold;
    this->iptLasso = data.iptLasso;
    this->inumLasso = data.inumLasso;
    this->maskImg = new cv::Mat(data.maskImg->clone());
    this->workingImg = new cv::Mat(data.workingImg->clone());
}
/*
 * 拷贝赋值运算符
 */
HistoryData &HistoryData::operator=(const HistoryData &data) {
    // std::cout << "copy assign operator" << std::endl;
    this->threshold = data.threshold;
    this->iptLasso = data.iptLasso;
    this->inumLasso = data.inumLasso;
    this->maskImg = new cv::Mat(data.maskImg->clone());
    this->workingImg = new cv::Mat(data.workingImg->clone());
    return *this;
}
/*
 * clone
 */
HistoryData *HistoryData::clone() {
    return new HistoryData(this->threshold,this->iptLasso, this->inumLasso, this->workingImg,this->maskImg);
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
    this->dequeA =new std::deque<HistoryData*>();
    this->dequeB =new std::deque<HistoryData*>();
    /*
     * A栈顶端是当前显示在屏幕上的结果
     */
}
/*
 * 析构函数
 */
History::~History() {
    delete this->dequeA;
    delete this->dequeB;
}
/*
 * 添加一个新操作,压入A栈,如果B栈非空,则删空它
 */
void History::add(HistoryData* data) {
    // std::cout << "add func called" << std::endl;
    // HistoryData* temp = data->clone();
    if (this->dequeA->size() >= DEQUEA_MAX_SIZE) dequeA->pop_front();
    this->dequeA->push_back(data);
    // std::cout << "deque A top: " << this->dequeA->top()->workingImg << std::endl;
//    while (!this->dequeB->empty()){
//        this->dequeB->pop_back();
//        // this->dequeB->();
//    }
    dequeB->clear();
    delete this->dequeB;
    this->dequeB = new std::deque<HistoryData*>();
}
/*
 * 撤销
 * 将当前A栈顶端的操作保存在B栈中
 * A栈弹出顶端元素
 * 返回A栈当前顶端元素
 *
 */
bool History::undo(HistoryData* data) {
    // std::cout << "undo func called" << std::endl;
    if(this->dequeA->size()<=1){//
        return false;
    }else{
        this->dequeB->push_back(this->dequeA->back()->clone());
        this->dequeA->pop_back();
        data->maskImg = new cv::Mat(this->dequeA->back()->maskImg->clone());
		data->workingImg = new cv::Mat(this->dequeA->back()->workingImg->clone());
        // data->maskImg = this->dequeA->back()->maskImg;
        // data->workingImg = this->dequeA->back()->workingImg;
		data->threshold = this->dequeA->back()->threshold;
		data->iptLasso = this->dequeA->back()->iptLasso;
		data->inumLasso = this->dequeA->back()->inumLasso;
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
    // std::cout << "redo func called" << std::endl;
    if(this->dequeB->empty()){
        return false;
    }else{
        //data = this->dequeB->top();
		data->maskImg = this->dequeB->back()->maskImg;
		data->workingImg = this->dequeB->back()->workingImg;
		data->threshold = this->dequeB->back()->threshold;
		data->iptLasso = this->dequeB->back()->iptLasso;
		data->inumLasso = this->dequeB->back()->inumLasso;
        this->dequeA->push_back(data->clone());
        this->dequeB->pop_back();
    }
    return true;
}

/*
 * keep the top element of deque A, as saved mask image, then prepare for clear();
 */
void History::savedATop() {
    // std::cout << "saved A top func called" << std::endl;
    while (!this->dequeB->empty()){
        delete this->dequeB->back();
        this->dequeB->pop_back();
    }
//    delete this->dequeB;
//    this->dequeB = new std::deque<HistoryData*>();
    this->dequeB->push_back(dequeA->back()->clone());
    while (!this->dequeA->empty()){
        delete this->dequeA->back();
        this->dequeA->pop_back();
    }
//    delete this->dequeA;
//    this->dequeA = new std::deque<HistoryData*>();
    this->dequeA->push_back(dequeB->back()->clone());
    delete this->dequeB->back();
    this->dequeB->pop_back();
}

/*
 * clear the history
 */
void History::clear() {
    // std::cout << "clear" << std::endl;
    while (!this->dequeB->empty()){
        delete this->dequeB->back();
        this->dequeB->pop_back();
    }
    delete this->dequeB;
    this->dequeB = new std::deque<HistoryData*>();
//    this->dequeB->push(dequeA->top()->clone());
    while (!this->dequeA->empty()){
        delete this->dequeA->back();
        this->dequeA->pop_back();
    }
    delete this->dequeA;
    this->dequeA = new std::deque<HistoryData*>();
    // this->dequeA->push(dequeB->top()->clone());
    // delete this->stackB->top();
    // this->stackB->pop();
}
