//
// Created by Anna on 2019/11/29.
//

#ifndef MASKGENERATOR_MYGRAPHICSVIEW_H
#define MASKGENERATOR_MYGRAPHICSVIEW_H
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
/*
 * ==========
 * view
 * ==========
 */
class MyGraphicsView :public QGraphicsView{
    Q_OBJECT
public:
    void wheelEvent(QWheelEvent * event) override;
	void mousePressEvent(QMouseEvent* event) override;
signals:
    void mouseWheelZoom(int delta);
};
/*
 * ==========
 * sense
 * ==========
 */
class MyQGraphicsScene : public QGraphicsScene
{
	Q_OBJECT
public:
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};
/*
 * ==========
 * item
 * ==========
 */
class MyQGraphicsPixmapItem : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
public:
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
signals:
	void mouseLeftDown(int x, int y);
};

#endif //MASKGENERATOR_MYGRAPHICSVIEW_H
