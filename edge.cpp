#include <QPainter>
#include "edge.h"

class Node;

Edge::Edge(const Node *tail, const Node *head, QList<QPointF>& coords) :
	_tail(tail), _head(head)
{
	if (coords.length() == 2) {
		_path.moveTo(coords[0]);
		_path.lineTo(coords[1]);
	} else if (coords.length() == 3) {
		_path.moveTo(coords[0]);
		_path.quadTo(coords[1], coords[2]);
	} else {
		_path.moveTo(coords[0]);
		_path.cubicTo(coords[1], coords[2], coords[3]);

		if (coords.length() > 4) { // more than 2 control points, drawing an approximation...
			for (int i = 2 ; i < coords.length()-3 ; i++) {
				QPainterPath tempPath;
				tempPath.moveTo(coords[i]);
				tempPath.cubicTo(coords[i+1], coords[i+2], coords[i+3]);
				_path.addPath(tempPath);
			}
		}
	}
}


QRectF Edge::boundingRect() const
{
	return _path.boundingRect();
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget* widget)
{
	painter->drawPath(_path);
}
