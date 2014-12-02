#include <QString>
#include <QFileDialog>
#include <QGraphicsView>
#include "viewer.h"
#include "drawing.h"
#include "ui_viewer.h"

Viewer::Viewer(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::Viewer)
{
	ui->setupUi(this);
	connect(ui->actionQuitter, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui->actionOuvrir, SIGNAL(triggered()), this, SLOT(openGraph()));
}

void Viewer::openGraph()
{
	QString filename = QFileDialog::getOpenFileName(this);
	if (!filename.isEmpty()) {
		Drawing* d = new Drawing(filename);
		//_scenes.append(sc);
		//ui->docs->setActiveSubWindow(ui->docs->addSubWindow(sc->view()));
		setCentralWidget(d);

	}
}

Viewer::~Viewer()
{
	delete ui;
	delete _g;
}
