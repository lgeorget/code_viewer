#ifndef NODE_H
#define NODE_H

#include <QGraphicsObject>
#include <QGraphicsEllipseItem>
#include <QGraphicsSimpleTextItem>
#include <QFont>
#include <gvc.h>
#include <types.h>

class Graph;

class Node : public QGraphicsObject
{
	Q_OBJECT
public:
	explicit Node(Agnode_t *v, Graph* graph, QGraphicsItem *parent = 0);
	~Node();
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	virtual QRectF boundingRect() const;
	void hide();

signals:

public slots:
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
	Graph* _graph;
	Agnode_t* _gv_node;
	QGraphicsEllipseItem* _inner;
	QGraphicsSimpleTextItem* _label;

friend class Graph;
};

#endif // NODE_H
