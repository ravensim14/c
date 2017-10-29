#include <gtk/gtk.h>

static struct {
  GtkWidget *main;
  GtkWidget *test;
} window;

static struct {
  GtkWidget *main;
  GtkWidget *top;
} grid;

static struct {
  GtkWidget *quit;
  GtkWidget *test;
  GtkWidget *example;
} button;

static struct {
  GtkWidget *top;
} separator;

static gboolean keypress(GtkWidget *widget, GdkEventKey *event) {

  if (event->keyval == 65307) {
    gtk_main_quit();
  }
}

int main(int argc, char *argv[]) {

  gtk_init (&argc, &argv);

  window.main = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(window.main), 500, 500);
  g_signal_connect(window.main, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(window.main, "key_press_event", G_CALLBACK(keypress), NULL);

  window.test = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(window.test), 400, 400);
  g_signal_connect(window.test, "delete-event", G_CALLBACK(gtk_widget_hide), NULL);

  grid.main = gtk_grid_new();
  gtk_grid_set_row_homogeneous(GTK_GRID(grid.main), true);
  gtk_grid_set_column_homogeneous(GTK_GRID(grid.main), true);

  grid.top = gtk_grid_new();
  gtk_grid_attach(GTK_GRID(grid.main), grid.top, 0, 0, 1, 1);

  button.test = gtk_button_new_with_label("TEST");
  gtk_grid_attach(GTK_GRID(grid.top), button.test, 0, 0, 1, 1);
  g_signal_connect_swapped(button.test, "clicked", G_CALLBACK(gtk_widget_show_all), window.test);

  separator.top = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_grid_attach(GTK_GRID(grid.top), separator.top, 1, 0, 1, 1);

  button.example = gtk_button_new_with_label("EXAMPLE");
  gtk_grid_attach(GTK_GRID(grid.top), button.example, 2, 0, 1, 1);

  button.quit = gtk_button_new_with_label("QUIT");
  gtk_grid_attach(GTK_GRID(grid.main), button.quit, 0, 1, 1, 1);
  g_signal_connect_swapped(button.quit, "clicked", G_CALLBACK(gtk_widget_destroy), window.main);

  gtk_container_add(GTK_CONTAINER(window.main), grid.main);

  gtk_widget_show_all(window.main);

  gtk_main();

  return 0;
}
