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
	QGraphicsView::mousePressEvent(event);
}
/*
 * 鼠标按下事件:sense
 */
void MyQGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsScene::mousePressEvent(event);
}
/*
 *鼠标按下事件:item
 */
void MyQGraphicsPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		emit mouseLeftDown(event->scenePos().x(), event->scenePos().y());
	}
}
/*
 * 鼠标移动事件:view
 */
void MyGraphicsView::mouseMoveEvent(QMouseEvent *event) {
    //QGraphicsView::setMouseTracking(true);
    QGraphicsView::mouseMoveEvent(event);
}
/*
 * 鼠标移动事件:sense
 */
void MyQGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mouseMoveEvent(event);
}
/*
 * 鼠标移动事件:item
 */
void MyQGraphicsPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit mouseLeftMoved(event->scenePos().x(), event->scenePos().y());
    }
}
/*
 * 鼠标双击事件:view
 */
void MyGraphicsView::mouseDoubleClickEvent(QMouseEvent *event) {
    QGraphicsView::mouseDoubleClickEvent(event);
}

/*
 * 鼠标双击事件:sense
 */
void MyQGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mouseDoubleClickEvent(event);
}

/*
 * 鼠标双击事件:item
 */
void MyQGraphicsPixmapItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
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
    qDebug() << "button release view";
    QGraphicsView::mouseReleaseEvent(event);
}
/*
 * 鼠标释放事件:Sense
 */
void MyQGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << "button release sense";
    QGraphicsScene::mouseReleaseEvent(event);
}
/*
 * 鼠标释放事件:Item
 */
void MyQGraphicsPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsPixmapItem::mouseReleaseEvent(event);
    qDebug() << "button release item";
    if (event->button() == Qt::LeftButton) 
    {
        emit mouseLeftRelease(event->scenePos().x(), event->scenePos().y());
        qDebug() << "left button release";
    }
}

