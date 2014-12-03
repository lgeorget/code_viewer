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
	explicit Node(Agnode_t *v, const Graph* graph, QGraphicsItem *parent = 0);
	~Node();
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	virtual QRectF boundingRect() const;

signals:

public slots:

private:
	const Graph* _graph;
	Agnode_t* _gv_node;
	QGraphicsEllipseItem* _inner;
	QGraphicsSimpleTextItem* _label;
};

#endif // NODE_H
