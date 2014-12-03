#include <QFont>
#include <QDebug>
#include <cstring>
#include "node.h"
#include "graph.h"

Node::Node(Agnode_t *v, Graph *graph, QGraphicsItem *parent) :
	QGraphicsObject(parent),
	_graph(graph),
	_gv_node(v),
	_inner(new QGraphicsEllipseItem(this)),
	_label(new QGraphicsSimpleTextItem(this))
{
	qreal dpi = _graph->getDpi();
	qreal scale = dpi/_graph->DOT_DEFAULT_DPI;
//	qDebug() << "dpi : " << dpi << " " << scale;
	const Agraph_t* g = _graph->getAgraph();
	_inner->setRect(ND_coord(v).x*scale - ND_width(v)*dpi/2,
					(GD_bb(g).UR.y - ND_coord(v).y)*scale - ND_height(v)*dpi/2,
					ND_width(v)*dpi,
					ND_height(v)*dpi);


//	qDebug() << "Adding node " << v->name << " at coords " << node->x() << ", " << node->y() << ", " << node->boundingRect();
	_label->setText(v->name);
	_label->setFont(_graph->MONOSPACE_FONT);
	_label->setPos(ND_coord(v).x*scale - ND_width(v)*dpi/2 + 20,
				  (GD_bb(g).UR.y - ND_coord(v).y)*scale - 5);
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

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	_graph->hideSubTree(this);
}

void Node::hide()
{
	agsafeset(_gv_node,"style","invisible","invisible");
	QGraphicsItem::hide();

}
