#include <functional>
#include <QFont>
#include <QDebug>
#include <cstring>
#include "node.h"
#include "graph.h"
#include "element.h"

Node::Node(Agnode_t *v, Graph *graph, QGraphicsItem *parent) :
	Element(graph, parent),
	_gv_node(v),
	_label(new QGraphicsSimpleTextItem(this))
{
	qreal dpi = _graph->getDpi();
	qreal scale = dpi/_graph->DOT_DEFAULT_DPI;
//	qDebug() << "dpi : " << dpi << " " << scale;
	const Agraph_t* g = _graph->getAgraph();
	_inner = new QGraphicsEllipseItem(ND_coord(v).x*scale - ND_width(v)*dpi/2,
									  (GD_bb(g).UR.y - ND_coord(v).y)*scale - ND_height(v)*dpi/2,
									  ND_width(v)*dpi,
									  ND_height(v)*dpi,
									  this);
	_inner->setPen(DEFAULT_PEN);

//	qDebug() << "Adding node " << v->name << " at coords " << node->x() << ", " << node->y() << ", " << node->boundingRect();
	_label->setText(v->name);
	_label->setFont(_graph->MONOSPACE_FONT);
	_label->setPos(ND_coord(v).x*scale - ND_width(v)*dpi/2 + 20,
				  (GD_bb(g).UR.y - ND_coord(v).y)*scale - 5);

	setZValue(100);

	setFlags(QGraphicsItem::ItemIsSelectable);
	setAcceptHoverEvents(true);
}

Node::~Node()
{
	delete _inner;
	delete _label;
}

void Node::hide()
{
	agsafeset(_gv_node,"style","invisible","invisible");
	QGraphicsItem::hide();
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	std::function<void(Element&)> callback = &Element::hide;
	_graph->pimpSubTree(this,callback,&Node::isVisible,true);
}

void Node::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	_graph->pimpSubTree(this,&Element::highlight);
}

void Node::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	if (!isSelected()) //FIXME: vérifier qu'un ancêtre n'est pas sélectionné
		_graph->pimpSubTree(this, &Element::unhighlight);
}
