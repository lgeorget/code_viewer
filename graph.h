#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>
#include <QGraphicsScene>
#include <gvc.h>
#include <QGraphicsScene>
#include <QMap>
#include <QPair>
#include <QFont>

class Graph : public QGraphicsScene
{
	Q_OBJECT

public:
	Graph(const QString& filename);
	~Graph();
	static const qreal DOT_DEFAULT_DPI;
	static const QFont MONOSPACE_FONT;
	const Agraph_t* getAgraph() const;
	qreal getDpi() const;

public slots:
	void doLayout();

private:
	void addNode(Agnode_t* v);
	void addEdge(Agedge_t* v);
	void setAttrs();

	GVC_t* _gv_con;
	Agraph_t* _graph;
	qreal _dpi;
	QMap<QString,QGraphicsItem*> _nodes;
	QMap<QPair<QString,QString>,QGraphicsItem*> _edges;
};

#endif // GRAPH_H
