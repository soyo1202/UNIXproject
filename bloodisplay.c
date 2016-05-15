#include <gtk/gtk.h>


int number = 3;


gboolean expose_event_callback(GtkWidget *widget,GdkEventExpose *event,gpointer data) {
                               
    GdkGC *gc = widget->style->fg_gc[GTK_WIDGET_STATE(widget)];
    GdkDrawable *drawable = widget->window;
    GdkColor color;
	// 畫一張圖
	switch (number)
	{
		case 3:
			gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/aa.png", NULL), 0, 0, 120, 550, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
		case 2:
			gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/aa.png", NULL), 0, 0, 70, 550, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
		case 1:
			gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/aa.png", NULL), 0, 0, 20, 550, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);

		default:
			break;
	}
    return TRUE;
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *drawing_area;

    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkDrawingArea");

    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 800, 600);
                  
    g_signal_connect(GTK_OBJECT(drawing_area), "expose_event",  
                     G_CALLBACK(expose_event_callback), NULL);

    gtk_container_add(GTK_CONTAINER(window), drawing_area);

    g_signal_connect(GTK_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}


