#include <QString>
#include <cstdio>
#include <exception>
#include <QGraphicsPathItem>
#include <QPair>
#include <QDebug>
#include <types.h>
#include "graph.h"
#include "node.h"

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

void Graph::setAttrs()
{
	agnodeattr(_graph, "fontname", const_cast<char*>(qPrintable(MONOSPACE_FONT.family())));
	agnodeattr(_graph, "fontsize", const_cast<char*>(qPrintable(QString("%1").arg(MONOSPACE_FONT.pointSizeF()*_dpi/DOT_DEFAULT_DPI))));
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
	Node* node = new Node(_graph,v,_dpi,DOT_DEFAULT_DPI);
	addItem(node);
	_nodes[v->name] = node;
}

void Graph::addEdge(Agedge_t* e)
{
	QPainterPath path;

	//Calculate the path from the spline (only one spline, as the graph is strict. If it
	//wasn't, we would have to iterate over the first list too)
	//Calculate the path from the spline (only one as the graph is strict)
	if ((ED_spl(e)->list!=0) && (ED_spl(e)->list->size%3 == 1))
	{
		//If there is a starting point, draw a line from it to the first curve point
		if(ED_spl(e)->list->sflag)
		{
			path.moveTo(ED_spl(e)->list->sp.x*(_dpi/DOT_DEFAULT_DPI),
						 (GD_bb(_graph).UR.y - ED_spl(e)->list->sp.y)*(_dpi/DOT_DEFAULT_DPI));
			path.lineTo(ED_spl(e)->list->list[0].x*(_dpi/DOT_DEFAULT_DPI),
					(GD_bb(_graph).UR.y - ED_spl(e)->list->list[0].y)*(_dpi/DOT_DEFAULT_DPI));
		}
		else
			path.moveTo(ED_spl(e)->list->list[0].x*(_dpi/DOT_DEFAULT_DPI),
					(GD_bb(_graph).UR.y - ED_spl(e)->list->list[0].y)*(_dpi/DOT_DEFAULT_DPI));

		//Loop over the curve points
		for(int i=1; i<ED_spl(e)->list->size; i+=3)
			path.cubicTo(ED_spl(e)->list->list[i].x*(_dpi/DOT_DEFAULT_DPI),
				  (GD_bb(_graph).UR.y - ED_spl(e)->list->list[i].y)*(_dpi/DOT_DEFAULT_DPI),
				  ED_spl(e)->list->list[i+1].x*(_dpi/DOT_DEFAULT_DPI),
				  (GD_bb(_graph).UR.y - ED_spl(e)->list->list[i+1].y)*(_dpi/DOT_DEFAULT_DPI),
				  ED_spl(e)->list->list[i+2].x*(_dpi/DOT_DEFAULT_DPI),
				  (GD_bb(_graph).UR.y - ED_spl(e)->list->list[i+2].y)*(_dpi/DOT_DEFAULT_DPI));

		//If there is an ending point, draw a line to it
		if(ED_spl(e)->list->eflag)
			path.lineTo(ED_spl(e)->list->ep.x*(_dpi/DOT_DEFAULT_DPI),
						 (GD_bb(_graph).UR.y - ED_spl(e)->list->ep.y)*(_dpi/DOT_DEFAULT_DPI));
	}

	QGraphicsPathItem* edge = new QGraphicsPathItem(path);

//	qDebug() << "Adding edge between " << e->head->name << " and " << e->head->name;
	addItem(edge);
	_edges[QPair<QString,QString>(e->head->name,e->tail->name)] = edge;
}
