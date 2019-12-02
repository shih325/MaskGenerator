//
// Created by Anna on 2019/11/29.
//

#include "MyGraphicsView.h"
#include <QWheelEvent>
#include <QMouseEvent>
#include <qDebug>
#include <QGraphicsSceneMouseEvent>
/*
 * 滚轮滑动事件
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
