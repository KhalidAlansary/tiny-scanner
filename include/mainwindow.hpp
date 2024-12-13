#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <graphviz/gvc.h>

#include "parser.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private slots:
  void on_chooseFile_clicked();

  void on_parse_clicked();

 private:
  Ui::MainWindow* ui;
  void create_dot_graph(Agraph_t* g, Node* node);
};
#endif  // MAINWINDOW_HPP
