//
// Created by Anna on 2019/11/29.
//

#include "MyGraphicsView.h"
#include <QWheelEvent>
#include <QMouseEvent>
#include <qDebug>
#include <QGraphicsSceneMouseEvent>
/*
 * 滚轮滑动事件:view
 */
void MyGraphicsView::wheelEvent(QWheelEvent *event) {
    QGraphicsView::wheelEvent(event);
    emit mouseWheelZoom( event->delta());
}
/*
 * 鼠标按下事件:view
 */
void MyGraphicsView::mousePressEvent(QMouseEvent * event)
{
    //qDebug() << "view:鼠标按键";
	QGraphicsView::mousePressEvent(event);
}
/*
 * 鼠标按下事件:sense
 */
void MyQGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    //qDebug() << "sense:鼠标按键";
	QGraphicsScene::mousePressEvent(event);
}
/*
 *鼠标按下事件:item
 */
void MyQGraphicsPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    //qDebug() << "item:鼠标按键";
	if (event->button() == Qt::LeftButton)
	{
        //qDebug() << "item:鼠标左键按下";
		emit mouseLeftDown(event->scenePos().x(), event->scenePos().y());
	}
}
/*
 * 鼠标移动事件:view
 */
void MyGraphicsView::mouseMoveEvent(QMouseEvent *event) {
    //qDebug() << "view:鼠标移动";
    //QGraphicsView::setMouseTracking(true);
    QGraphicsView::mouseMoveEvent(event);
}
/*
 * 鼠标移动事件:sense
 */
void MyQGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    //qDebug() << "sense:鼠标移动";
    QGraphicsScene::mouseMoveEvent(event);
}
/*
 * 鼠标移动事件:item
 */
void MyQGraphicsPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    /*qDebug() << "item:鼠标移动";
    qDebug() << "item:鼠标移动========================";*/
    emit mouseLeftMoved(event->scenePos().x(), event->scenePos().y());
}
/*
 * 鼠标双击事件:view
 */
void MyGraphicsView::mouseDoubleClickEvent(QMouseEvent *event) {
    //qDebug() << "view:鼠标双击";
    QGraphicsView::mouseDoubleClickEvent(event);
}

/*
 * 鼠标双击事件:sense
 */
void MyQGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    //qDebug() << "sense:鼠标双击";
    QGraphicsScene::mouseDoubleClickEvent(event);
}

/*
 * 鼠标双击事件:item
 */
void MyQGraphicsPixmapItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    //qDebug() << "item:鼠标双击";
    if (event->button() == Qt::LeftButton)
    {
        emit mouseDoubleClicked(event->scenePos().x(), event->scenePos().y());
    }
}
/*
 * 鼠标释放事件:View
 */
void MyGraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    //qDebug() << "button release view";
    QGraphicsView::mouseReleaseEvent(event);
}
/*
 * 鼠标释放事件:Sense
 */
void MyQGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    //qDebug() << "button release sense";
    QGraphicsScene::mouseReleaseEvent(event);
}
/*
 * 鼠标释放事件:Item
 */
void MyQGraphicsPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    //qDebug() << "button release item";
    if (event->button() == Qt::LeftButton) 
    {
        emit mouseLeftRelease(event->scenePos().x(), event->scenePos().y());
        //qDebug() << "left button release";
    }
}

