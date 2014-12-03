#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsObject>
#include <QGraphicsPathItem>
#include <gvc.h>
#include <types.h>

class Graph;

class Edge : public QGraphicsObject
{
	Q_OBJECT
public:
	explicit Edge(Agedge_t *v, Graph* graph, QGraphicsItem *parent = 0);
	~Edge();
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	virtual QRectF boundingRect() const;
	void hide();

signals:

public slots:
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
	Graph* _graph;
	Agedge_t* _gv_edge;
	QGraphicsPathItem* _inner;

friend class Graph;
};

#endif // EDGE_H