#include <graphviz/gvc.h>
#include <gtk/gtk.h>

FILE* file;

static void on_file_dialog_response(GObject* source,
                                    GAsyncResult* result,
                                    gpointer _) {
  GtkFileDialog* dialog = GTK_FILE_DIALOG(source);
  GFile* gfile = gtk_file_dialog_open_finish(dialog, result, NULL);

  if (gfile) {
    char* filepath = g_file_get_path(gfile);
    file = fopen(filepath, "r");
    g_free(filepath);
    g_object_unref(gfile);
  }
}

static void on_file_choose_clicked(GtkWidget* _, gpointer user_data) {
  GtkWindow* window = GTK_WINDOW(user_data);
  GtkFileDialog* dialog = gtk_file_dialog_new();

  gtk_file_dialog_set_title(dialog, "Open File");
  gtk_file_dialog_open(dialog, window, NULL, on_file_dialog_response, NULL);
  g_object_unref(dialog);
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

  // print global variable file value
  if (file) {
    printf("file is not null\n");
    printf("file: %p\n", file);
  } else {
    printf("file is null\n");
  }

  return status;
}
