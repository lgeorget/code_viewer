#include <QProcess>
#include <QDebug>
#include <QtAlgorithms>
#include <exception>
#include "scene.h"
#include "ui_scene.h"
#include "node.h"
#include "edge.h"

#define DPI 96

Scene::Scene(QString inputFileName, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Scene),
	_drawing(new QGraphicsScene(this)),
	_view(new QGraphicsView(_drawing,this))
{
	ui->setupUi(this);

	_gv = new QProcess(this);
	connect(_gv,SIGNAL(readyReadStandardOutput()), this, SLOT(drawSomething()));
	connect(_gv, SIGNAL(finished(int)), this, SLOT(cleanGV(int)));

	QString prog = "dot";
	QStringList args;
	args << "-T" << "plain"
		 << inputFileName;
	_gv->start(prog,args,QIODevice::ReadOnly);
}

void Scene::drawSomething()
{
	if (!_gv || !_gv->isOpen())
		return;

	while ((_r = _gv->readLine(_raw + _r, RAW_BUFFER_SIZE-1)) > 0)
	{
		QByteArray ba(_raw);
		if (ba.endsWith('\n')) {
			ba.chop(1);
			_r = 0;
		} else {
			return;
		}

		if (ba.startsWith("graph")) {
			ba.remove(0, 6);
			parseGraph(ba.split(' '));
		} else if (ba.startsWith("node")) {
			ba.remove(0, 5);
			parseNode(ba.split(' '));
		} else if (ba.startsWith("edge")) {
			ba.remove(0, 5);
			parseEdge(ba.split(' '));
		} else if (ba.startsWith("stop")) {
			qDebug() << "Graph parsed";
			return;
		} else {
			qDebug() << "b : " << ba;
			throw std::invalid_argument("Unexpected input from dot");
		}
	}
}

void Scene::cleanGV(int status)
{
	if (status != 0)
		throw std::runtime_error("GraphViz process terminated with errors");

	_gv->close();
	this->repaint();
}

void Scene::repaint()
{
	_view->repaint();
}

QGraphicsView *Scene::view() const
{
	return _view;
}

void Scene::parseGraph(QList<QByteArray> b)
{
	bool ok = true;
	bool res;
//	float scale = b[0].toFloat(&res);
//	ok = ok && res;
	float width = b[1].toFloat(&res) * DPI;
	qDebug() << "b[1] : " << b[1] << " " << width << " " << res;
	ok = ok && res;
	float height = b[2].toFloat(&res) * DPI;
	qDebug() << "b[2] : " << b[2] << " " << width << " " << res;
	ok = ok && res;

	if (!ok)
		throw std::invalid_argument("Malformed 'graph' line");

	_drawing->setSceneRect(0,0,width,height);
}

void Scene::parseNode(QList<QByteArray> b)
{
	bool ok = true;
	bool res;

	QString name(b[0]);
	float x = b[1].toFloat(&res) * DPI;
	qDebug() << "b[1] : " << b[1] << " " << x << " " << res;
	ok = ok && res;
	float y = b[2].toFloat(&res) * DPI;
	qDebug() << "b[2] : " << b[2] << " " << y << " " << res;
	ok = ok && res;
	float width = b[3].toFloat(&res) * DPI;
	qDebug() << "b[3] : " << b[3] << " " << width << " " << res;
	ok = ok && res;
	float height = b[4].toFloat(&res) * DPI;
	qDebug() << "b[4] : " << b[4] << " " << height << " " << res;
	ok = ok && res;
	QString label(b[5]);

	if (!ok)
		throw std::invalid_argument("Malformed 'node' line");

	Node* n = new Node(x,y,width,height,label);
	_nodes[label] = n;
	_drawing->addItem(n);
}

void Scene::parseEdge(QList<QByteArray> b)
{
	bool ok = true;
	bool res;

	QString tail(b[0]);
	QString head(b[1]);
	long n = b[2].toLong(&res);
	qDebug() << "b[2] : " << b[2] << " " << n << res;
	ok = ok && res;
	if (!ok)
		throw std::invalid_argument("Malformed 'edge' line");

	QList<QPointF> coords;
	for (int i=0 ; i<n ; i++) {
		float x = b[3+2*i].toFloat(&res) * DPI;
		qDebug() << "b[" << 3+2*i << "] : " << b[3+2*i] << " " << x << res;
		ok = ok && res;
		float y = b[3+2*i+1].toFloat(&res) * DPI;
		qDebug() << "b[" << 3+2*i+1 << "] : " << b[3+2*i+1] << " " << y << res;
		ok = ok && res;
		if (!ok)
			throw std::invalid_argument("Malformed 'edge' line");
		coords.append(QPointF(x, y));
	}

	_drawing->addItem(new Edge(_nodes[tail], _nodes[head], coords));
}

Scene::~Scene()
{
	delete ui;
	qDeleteAll(_nodes);
}
