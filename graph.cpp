#include <cstdio>
#include <cstring>
#include <QString>
#include <exception>
#include <QGraphicsPathItem>
#include <QPair>
#include <QQueue>
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

void Graph::pimpSubTree(Node *n, std::function<void (Element &)> f, std::function<bool (Element&)> test)
{
	Agnode_t* v = n->_gv_node;
	QQueue<Agnode_t*> waitingNodes;

	waitingNodes.enqueue(v);
	while (!waitingNodes.empty()) {
		Agnode_t* currentNode = waitingNodes.dequeue();
		f(*_nodes[currentNode->id]);
		for (Agedge_t* in = agfstin(_graph,currentNode) ; in ; in = agnxtin(_graph,in)) {
			f(*_edges[in->id]);
		}

		for (Agedge_t* e = agfstout(_graph,currentNode) ; e ; e = agnxtout(_graph,e)) {
			Agnode_t* nextNode = e->head;
			bool toProcess = true;
			for (Agedge_t* in = agfstin(_graph,nextNode) ; (in != nullptr) && toProcess ; in = agnxtin(_graph,in)) {
				if (test(*_nodes[in->tail->id])) {
					toProcess = false;
				}
			}
			f(*_edges[e->id]);
			if (toProcess) {
				waitingNodes.enqueue(nextNode);
			}
		}
	}
}

void Graph::pimpSubTree(Edge *e, std::function<void (Element &)> f, std::function<bool (Element&)> test)
{
	f(*e);
	Node *n = _nodes[e->_gv_edge->head->id];
	pimpSubTree(n,f,test);
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
	_nodes[v->id] = node;
}

void Graph::addEdge(Agedge_t* e)
{
	Edge* edge = new Edge(e,this);
	addItem(edge);
	_edges[e->id] = edge;
}
