#ifndef SCENE_H
#define SCENE_H

#include <QWidget>
#include <QProcess>
#include <QGraphicsScene>
#include <QMap>
#include "node.h"

namespace Ui {
	class Scene;
}

class Scene : public QWidget
{
	Q_OBJECT

public:
	explicit Scene(QString filename, QWidget *parent = 0);
	~Scene();
	void parseGraph(QList<QByteArray> b);
	void parseNode(QList<QByteArray> b);
	void parseEdge(QList<QByteArray> b);
	QGraphicsView *view() const;

public slots:
	void drawSomething();
	void cleanGV(int status);
	void repaint();

private:
	Ui::Scene *ui;
	QProcess *_gv;
	QGraphicsScene *_drawing;
	QGraphicsView *_view;
	QMap<QString,Node*> _nodes;

	static const int RAW_BUFFER_SIZE = 4096;
	char _raw[RAW_BUFFER_SIZE];
	int _r;
};

#endif // SCENE_H
