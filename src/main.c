#include <graphviz/gvc.h>
#include <gtk/gtk.h>

#include "parser.h"
#include "util.h"

FILE* file;
Node* root;

static void on_file_dialog_response(GObject* source,
                                    GAsyncResult* result,
                                    gpointer user_data) {
  GtkFileDialog* dialog = GTK_FILE_DIALOG(source);
  GFile* gfile = gtk_file_dialog_open_finish(dialog, result, NULL);

  if (gfile) {
    char* filepath = g_file_get_path(gfile);
    file = fopen(filepath, "r");
    root = program();
    g_free(filepath);
    g_object_unref(gfile);
  }
}

static void on_file_choose_clicked(GtkWidget* w, gpointer user_data) {
  GtkWindow* window = GTK_WINDOW(user_data);
  GtkFileDialog* dialog = gtk_file_dialog_new();

  gtk_file_dialog_set_title(dialog, "Open File");
  gtk_file_dialog_open(dialog, window, NULL, on_file_dialog_response, NULL);
  g_object_unref(dialog);
}

static int node_count = 0;

static Agnode_t* add_subtree(Agraph_t* g, Node* node) {
  if (!node)
    return NULL;

  // Create unique node ID and label
  char node_id[32];
  char node_value[256];
  snprintf(node_id, sizeof(node_id), "node%d", node_count++);
  snprintf(node_value, sizeof(node_value), "%s\n%s",
           nodeTypeToString(node->type), node->value ? node->value : "");

  // Create the node
  Agnode_t* gv_node = agnode(g, node_id, 1);
  agsafeset(gv_node, "label", node_value, "");
  if (node->type >= NODE_OP) {
    agsafeset(gv_node, "shape", "oval", "");
  }

  // Process left child
  if (node->left) {
    Agnode_t* left_node = add_subtree(g, node->left);
    if (left_node) {
      Agedge_t* e = agedge(g, gv_node, left_node, NULL, 1);
      agsafeset(e, "constraint", "true", "");
    }
  }

  // Process right child
  if (node->right) {
    Agnode_t* right_node = add_subtree(g, node->right);
    if (right_node) {
      Agedge_t* e = agedge(g, gv_node, right_node, NULL, 1);
      agsafeset(e, "constraint", "true", "");
    }
  }

  // Process next sibling
  if (node->next) {
    Agnode_t* next_node = add_subtree(g, node->next);
    if (next_node) {
      Agedge_t* e = agedge(g, gv_node, next_node, NULL, 1);
      agsafeset(e, "constraint", "true", "");

      // Create invisible subgraph for same rank
      Agraph_t* subg = agsubg(g, "same_rank", 1);
      agsubnode(subg, gv_node, 1);
      agsubnode(subg, next_node, 1);
      agsafeset(subg, "rank", "same", "");
    }
  }

  return gv_node;
}

static void display_graph(GtkWindow* parent_window) {
  if (!root)
    return;

  // Initialize Graphviz
  GVC_t* gvc = gvContext();
  Agraph_t* g = agopen("g", Agdirected, NULL);

  // Set graph attributes
  agattr(g, AGRAPH, "splines", "ortho");
  agattr(g, AGRAPH, "nodesep", "0.5");  // Minimum space between nodes
  agattr(g, AGRAPH, "ranksep", "0.8");  // Minimum space between ranks
  agattr(g, AGRAPH, "size", "11,11");   // Graph size in inches

  // Set default node attributes
  agattr(g, AGNODE, "shape", "box");   // Rectangular nodes
  agattr(g, AGNODE, "height", "0.4");  // Node height
  agattr(g, AGNODE, "width", "0.8");   // Node width
  agattr(g, AGNODE, "margin", "0.2");  // Margin within nodes

  // Reset node counter
  node_count = 0;

  // Build the graph
  add_subtree(g, root);

  // Layout and render with higher DPI for better quality
  gvLayout(gvc, g, "dot");
  gvRenderFilename(gvc, g, "png:cairo:gd",
                   "parse_tree.png");  // Use cairo renderer

  // Create window to display the image
  GtkWidget* graph_window = gtk_window_new();
  gtk_window_set_title(GTK_WINDOW(graph_window), "Parse Tree");
  gtk_window_set_default_size(GTK_WINDOW(graph_window), 1300, 600);
  gtk_window_set_transient_for(GTK_WINDOW(graph_window), parent_window);

  // Create scrolled window
  GtkWidget* scrolled = gtk_scrolled_window_new();
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  // Use GtkPicture instead of GtkImage for better scaling
  GtkWidget* picture = gtk_picture_new_for_filename("parse_tree.png");
  gtk_picture_set_can_shrink(GTK_PICTURE(picture), TRUE);
  gtk_picture_set_content_fit(GTK_PICTURE(picture), GTK_CONTENT_FIT_FILL);

  // Set up widget hierarchy
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), picture);
  gtk_window_set_child(GTK_WINDOW(graph_window), scrolled);

  gtk_window_present(GTK_WINDOW(graph_window));

  // Cleanup
  gvFreeLayout(gvc, g);
  agclose(g);
  gvFreeContext(gvc);
}

static void activate(GtkApplication* app) {
  GtkWidget* window = gtk_application_window_new(app);
  GtkWidget* parse_button = gtk_button_new_with_label("Parse");
  GtkWidget* file_chooser = gtk_button_new_with_label("Choose File");
  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

  gtk_window_set_title(GTK_WINDOW(window), "Tiny Parser");
  gtk_window_set_default_size(GTK_WINDOW(window), 900, 800);

  g_signal_connect(file_chooser, "clicked", G_CALLBACK(on_file_choose_clicked),
                   window);
  g_signal_connect_swapped(parse_button, "clicked", G_CALLBACK(display_graph),
                           window);

  // Align the buttons
  gtk_widget_set_halign(parse_button, GTK_ALIGN_START);
  gtk_widget_set_valign(parse_button, GTK_ALIGN_START);
  gtk_widget_set_halign(file_chooser, GTK_ALIGN_START);
  gtk_widget_set_valign(file_chooser, GTK_ALIGN_START);

  gtk_window_set_child(GTK_WINDOW(window), box);

  gtk_box_append(GTK_BOX(box), parse_button);
  gtk_box_append(GTK_BOX(box), file_chooser);

  gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char** argv) {
  GtkApplication* app;
  int status;

  app = gtk_application_new("com.tiny.parser", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  free_tree(root);

  return status;
}
