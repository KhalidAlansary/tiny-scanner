#include <graphviz/gvc.h>
#include <QFileDialog>
#include <QMessageBox>

#include "./ui_mainwindow.h"
#include "mainwindow.hpp"
#include "parser.h"
#include "parserexception.hpp"
#include "util.h"

FILE* file;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::on_chooseFile_clicked() {
  QString filename = QFileDialog::getOpenFileName(
      this, "Open File", QDir::homePath(), "All files (*.*)");
  ui->label->setText(filename);

  QByteArray ba = filename.toLocal8Bit();
  const char* c_str = ba.data();
  file = fopen(c_str, "r");
}

void MainWindow::on_parse_clicked() {
  if (!file) {
    QMessageBox::warning(this, "Error", "Please select a file first.");
    return;
  }
  Node* root;
  try {
    root = program();
    if (!root) {
      qDebug() << "Error: Failed to parse program";
      return;
    }

    // Initialize Graphviz
    GVC_t* gvc = gvContext();
    if (!gvc) {
      qDebug() << "Error: Failed to create Graphviz context";
      return;
    }

    Agraph_t* g = agopen((char*)"g", Agdirected, 0);
    if (!g) {
      gvFreeContext(gvc);
      qDebug() << "Error: Failed to create graph";
      return;
    }

    // Convert parse tree to DOT format
    create_dot_graph(g, root);

    // Layout the graph
    if (gvLayout(gvc, g, "dot") != 0) {
      agclose(g);
      gvFreeContext(gvc);
      qDebug() << "Error: Failed to layout graph";
      return;
    }

    // Render to a temporary file
    const char* tempfile = "temp.png";
    if (gvRenderFilename(gvc, g, "png", tempfile) != 0) {
      gvFreeLayout(gvc, g);
      agclose(g);
      gvFreeContext(gvc);
      qDebug() << "Error: Failed to render graph";
      return;
    }

    // Load the image into QLabel
    QPixmap pixmap(tempfile);
    if (pixmap.isNull()) {
      qDebug() << "Error: Failed to load rendered image";
    } else {
      ui->graphLabel->setPixmap(pixmap);
      ui->graphLabel->setScaledContents(true);
    }

    // Cleanup
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
    free_tree(root);

  } catch (ParserException& e) {
    QMessageBox::warning(this, "Error", e.what());
  }
  fclose(file);
}

void MainWindow::create_dot_graph(Agraph_t* g, Node* node) {
  if (!node || !g)
    return;

  // Create current node
  char nodename[32];
  snprintf(nodename, sizeof(nodename), "node%p", (void*)node);
  Agnode_t* gnode = agnode(g, nodename, 1);
  if (!gnode)
    return;

  if (node->type < NODE_OP) {
    agsafeset(gnode, (char*)"shape", (char*)"box", (char*)"");
  }
  // Set node label
  char label[64];
  snprintf(label, sizeof(label), "%s", nodeTypeToString(node->type));
  if (node->value) {
    snprintf(label + strlen(label), sizeof(label) - strlen(label), "\n(%s)",
             node->value);
  }
  agsafeset(gnode, (char*)"label", label, (char*)"");

  // Process left child
  if (node->left) {
    char childname[32];
    snprintf(childname, sizeof(childname), "node%p", (void*)node->left);
    Agnode_t* child = agnode(g, childname, 1);
    if (child) {
      agedge(g, gnode, child, NULL, 1);
    }
    create_dot_graph(g, node->left);
  }

  // Process right child
  if (node->right) {
    char childname[32];
    snprintf(childname, sizeof(childname), "node%p", (void*)node->right);
    Agnode_t* child = agnode(g, childname, 1);
    if (child) {
      agedge(g, gnode, child, NULL, 1);
    }
    create_dot_graph(g, node->right);
  }

  // Process sibling
  if (node->next) {
    // Create subgraph for siblings
    char subgname[64];
    snprintf(subgname, sizeof(subgname), "subg%p", (void*)node);
    Agraph_t* subg = agsubg(g, subgname, 1);
    agsafeset(subg, (char*)"rank", (char*)"same", (char*)"");

    // Add current node to subgraph
    agsubnode(subg, gnode, 1);

    // Create and add sibling node
    char siblingname[32];
    snprintf(siblingname, sizeof(siblingname), "node%p", (void*)node->next);
    Agnode_t* sibling = agnode(g, siblingname, 1);
    if (sibling) {
      agsubnode(subg, sibling, 1);
      agedge(g, gnode, sibling, NULL, 1);
    }
    create_dot_graph(g, node->next);
  }
}
