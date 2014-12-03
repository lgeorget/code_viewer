#ifndef NODE_H
#define NODE_H

#include <QGraphicsObject>
#include <QGraphicsEllipseItem>
#include <QGraphicsSimpleTextItem>
#include <QPen>
#include <QFont>
#include <gvc.h>
#include <types.h>
#include "element.h"

class Graph;

class Node : public Element
{
	Q_OBJECT
public:
	explicit Node(Agnode_t *v, Graph* graph, QGraphicsItem *parent = 0);
	~Node();
	virtual void hide();

signals:

public slots:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
	Agnode_t* _gv_node;
	QGraphicsSimpleTextItem* _label;

friend class Graph;
};

#endif // NODE_H
