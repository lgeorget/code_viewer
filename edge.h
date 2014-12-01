#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>
#include <QPainterPath>
#include "node.h"

class Edge : public QGraphicsItem
{
public:
	explicit Edge(const Node *tail, const Node *head, QList<QPointF>& coords);
	virtual QRectF boundingRect() const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
	const Node *_tail;
	const Node *_head;
	QPainterPath _path;
};

#endif // EDGE_H
