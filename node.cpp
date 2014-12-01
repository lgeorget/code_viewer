#include <QGraphicsItem>
#include <QRectF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include "node.h"

Node::Node(float x, float y, float width, float height, QString& label) :
	_x(x-width/2), _y(y-height/2), _width(width), _height(height), _label(label), QGraphicsItem()
{
}

QRectF Node::boundingRect() const
{
	return QRectF(_x, _y, _width, _height);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget* widget)
{
	painter->drawEllipse(_x, _y, _width, _height);
	painter->drawText(_x+20, _y+_height/2, _label);
}
