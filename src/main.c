#include <gtk/gtk.h>
#include <stdlib.h>
#include "sysinfo.h"
#include "gpuinfo.h"

/* Callback function: when the tray icon is activated (clicked),
 * create a new window displaying system and GPU information.
 */
static void on_status_icon_activate(GtkStatusIcon *status_icon, gpointer user_data) {
    /* Create a new top-level window */
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "System Information");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    /* Create a non-editable text view */
    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);

    /* Gather system info and GPU info */
    char *sysinfo = get_system_info();
    char *gpuinfo = get_gpu_info();
    char *combined = g_strdup_printf("%s\n\n%s", sysinfo, gpuinfo);

    /* Set the text in the text view */
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, combined, -1);

    /* Clean up allocated strings */
    free(sysinfo);
    free(gpuinfo);
    g_free(combined);

    /* Place the text view in a scrolled window */
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);
    gtk_container_add(GTK_CONTAINER(window), scrolled_window);

    /* Show all widgets */
    gtk_widget_show_all(window);

    /* When the window is destroyed, simply let it be freed */
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_widget_destroy), window);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    /* Create a status icon for the system tray.
     * (Here we use a themed icon name; you may also specify a file path.)
     */
    GtkStatusIcon *tray_icon = gtk_status_icon_new_from_icon_name("utilities-system-monitor");
    gtk_status_icon_set_tooltip_text(tray_icon, "System Info");
    g_signal_connect(G_OBJECT(tray_icon), "activate", G_CALLBACK(on_status_icon_activate), NULL);

    gtk_main();
    return 0;
}

