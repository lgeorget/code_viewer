#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>

class Node : public QGraphicsItem
{
public:
	Node(float x, float y, float width, float height, QString &label);
	virtual QRectF boundingRect() const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
	float _x;
	float _y;
	float _width;
	float _height;
	QString _label;
};

#endif // NODE_H
