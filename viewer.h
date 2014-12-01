#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>
#include <QListWidget>
#include "scene.h"

namespace Ui {
  class Viewer;
}

class Viewer : public QMainWindow
{
  Q_OBJECT

public:
  explicit Viewer(QWidget *parent = 0);
  ~Viewer();

public slots:
  void openGraph();

private:
  Ui::Viewer *ui;
  QList<Scene *> _scenes;
};

#endif // VIEWER_H
