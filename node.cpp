#include "node.h"

Node::Node(const Agraph_t *graph, Agnode_t *v, qreal dpi, qreal dot_dpi, QGraphicsItem *parent) :
	QGraphicsObject(parent),
	_dpi(dpi),
	_scale(dpi/dot_dpi),
	_gv_node(v),
	_inner(new QGraphicsEllipseItem(ND_coord(v).x*_scale - ND_width(v)*dpi/2,
									(GD_bb(graph).UR.y - ND_coord(v).y)*_scale - ND_height(v)*dpi/2,
									ND_width(v)*dpi,
									ND_height(v)*dpi,
									this)),
	_label(new QGraphicsSimpleTextItem(this))
{
//	qDebug() << "Adding node " << v->name << " at coords " << node->x() << ", " << node->y() << ", " << node->boundingRect();
	_label->setText(v->name);
	_label->setFont(MONOSPACE_FONT);
	_label->setPos(ND_coord(v).x*_scale - ND_width(v)*dpi/2 + 20,
				  (GD_bb(graph).UR.y - ND_coord(v).y)*_scale - 5);
}

QRectF Node::boundingRect() const
{
	return _inner->boundingRect();
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	_inner->paint(painter, option, widget);
}

Node::~Node()
{
	delete _inner;
	delete _label;
}
