#include <QString>
#include <cstdio>
#include <exception>
#include <QGraphicsPathItem>
#include <QPair>
#include <QDebug>
#include <types.h>
#include "graph.h"
#include "edge.h"
#include "node.h"

const qreal Graph::DOT_DEFAULT_DPI = 72.0;
const QFont Graph::MONOSPACE_FONT = QFont("Monospace", 15, QFont::Normal);

Graph::Graph(const QString& filename) : QGraphicsScene(), _gv_con(gvContext()), _graph()
{
	std::FILE* fp = fopen(qPrintable(filename), "r");
	if (fp == nullptr)
		throw std::runtime_error("Couldn't open input graph");

	_graph = agread(fp);
	if (!_graph)
		throw std::runtime_error("Couldn't parse graph from input file");

	_dpi = QString(agget(_graph, const_cast<char *>("dpi"))).toDouble();
	if (_dpi == 0)
		_dpi = 96.0;

	setAttrs();
	doLayout();

	for (Agnode_t* v = agfstnode(_graph) ; v ; v = agnxtnode(_graph,v)) {
		addNode(v);
		for (Agedge_t* e = agfstout(_graph,v) ; e ; e = agnxtout(_graph,e))
			addEdge(e);
	}
}

Graph::~Graph()
{
	gvFreeLayout(_gv_con, _graph);
	gvFreeContext(_gv_con);
	qDeleteAll(_nodes);
	qDeleteAll(_edges);
}

const Agraph_t *Graph::getAgraph() const
{
	return _graph;
}

qreal Graph::getDpi() const
{
	return _dpi;
}

void Graph::setAttrs()
{
	agnodeattr(_graph, "fontname", const_cast<char*>(qPrintable(MONOSPACE_FONT.family())));
	agnodeattr(_graph, "fontsize", const_cast<char*>(qPrintable(QString("%1").arg(MONOSPACE_FONT.pointSizeF()*DOT_DEFAULT_DPI/_dpi))));
}

void Graph::doLayout()
{
	gvFreeLayout(_gv_con, _graph);
	gvLayout(_gv_con, _graph, "dot");

	setSceneRect(GD_bb(_graph).LL.x*(_dpi/DOT_DEFAULT_DPI), GD_bb(_graph).LL.y*(_dpi/DOT_DEFAULT_DPI),
				  GD_bb(_graph).UR.x*(_dpi/DOT_DEFAULT_DPI), GD_bb(_graph).UR.y*(_dpi/DOT_DEFAULT_DPI));
}

void Graph::addNode(Agnode_t* v)
{
	Node* node = new Node(v,this);
	addItem(node);
	_nodes[v->name] = node;
}

void Graph::addEdge(Agedge_t* e)
{
	Edge* edge = new Edge(e,this);
	addItem(edge);
	_edges[QPair<QString,QString>(e->head->name,e->tail->name)] = edge;
}
