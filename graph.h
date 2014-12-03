#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>
#include <QGraphicsScene>
#include <gvc.h>
#include <QGraphicsScene>
#include <QMap>
#include <QPair>
#include <QFont>

class Node;
class Edge;

class Graph : public QGraphicsScene
{
	Q_OBJECT

public:
	Graph(const QString& filename);
	~Graph();
	void hideSubTree(Node *n);
	void hideSubTree(Edge *e);

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
	QMap<int,Node*> _nodes;
	QMap<int,Edge*> _edges;
};

#endif // GRAPH_H
