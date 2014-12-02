#ifndef NODE_H
#define NODE_H

#include <QGraphicsObject>
#include <QGraphicsEllipseItem>
#include <QGraphicsSimpleTextItem>
#include <QFont>
#include <gvc.h>
#include <types.h>


class Node : public QGraphicsObject
{
	Q_OBJECT
public:
	explicit Node(const Agraph_t* graph, Agnode_t *v, qreal dpi, qreal dot_dpi, QGraphicsItem *parent = 0);
	~Node();
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	virtual QRectF boundingRect() const;

signals:

public slots:

private:
	qreal _dpi;
	qreal _scale;
	Agnode_t* _gv_node;
	QGraphicsEllipseItem* _inner;
	QGraphicsSimpleTextItem* _label;
	const QFont MONOSPACE_FONT = QFont("Monospace", 10, QFont::Normal);
};

#endif // NODE_H
