
#include "func.c"

int bullet_x = 395,bullet_y = 36;
// 參考資料：http://gmd20.blog.163.com/blog/static/1684392320098183459926/
GtkWidget *wid;

void drawBoss(cairo_t *cr)
{
	cairo_surface_t *image = cairo_image_surface_create_from_png("img/aa.png");

	cairo_set_source_surface(cr, image, 382, 0);
	cairo_paint(cr); 
}


gboolean Shoot_Bullet(gpointer data) {
    static gint count = 0;
    if( bullet_y <= 600 ) {
       // g_signal_emit_by_name(button, "clicked");
       // g_print ("%s\n", (char *)data);
       // g_print ("timer\n");
      	bullet_y += 5;
      	if( defense && bullet_y+bullet_height > player.y && bullet_y < player.y+120 
      	&& bullet_x > player.x && bullet_x+bullet_width < player.x+120 )
      	{
      		defense = false;
      		shoot = false;
      		gtk_widget_queue_draw(wid);
      		return FALSE;
      	}
      	
      	
		gtk_widget_queue_draw(wid);
        return TRUE;
    }
    else {
    g_print ("timer expire\n");
        return FALSE;
    }
}

static gboolean on_expose_event
(GtkWidget * widget, GdkEventExpose * event, gpointer data)
{
        
	GdkWindow *gdk_window =widget->window;
	cr = gdk_cairo_create(gdk_window);


	drawBoss(cr);

	cairo_surface_t *image = cairo_image_surface_create_from_png("img/chara.png");
	cairo_set_source_surface(cr, image, x+24, y+24);
	cairo_paint(cr); 

	if( defense )
	{
		cairo_surface_t *image2 = cairo_image_surface_create_from_png("img/defense.png");
		cairo_set_source_surface(cr, image2, x, y);
		cairo_paint(cr); 
	}
	if( shoot )
	{
		cairo_surface_t *image2 = cairo_image_surface_create_from_png("img/bullet.png");
		cairo_set_source_surface(cr, image2, bullet_x, bullet_y);
		cairo_paint(cr); 
	}



    return FALSE;
}

int
main (int argc, char *argv[])
{
    GtkWidget *window;

    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    g_signal_connect (G_OBJECT (window), "expose-event",
                      G_CALLBACK (on_expose_event), NULL);
    g_signal_connect (G_OBJECT (window), "destroy",
                      G_CALLBACK (gtk_main_quit), NULL);
	g_signal_connect (window, "key-press-event",   
                      G_CALLBACK(deal_key_press), NULL);                
                      
    create_surfaces ();

    gtk_window_set_position (GTK_WINDOW (window),
                                 GTK_WIN_POS_CENTER);
    gtk_window_set_default_size (GTK_WINDOW (window), 800,600);
    gtk_window_set_title (GTK_WINDOW (window), "patterns");
		

    gtk_widget_set_app_paintable (window, TRUE);
    gtk_widget_show_all (window);



	cr2 = gdk_cairo_create(window->window); // position!!
 	wid = window;

	shoot = true;
	g_timeout_add(100, (GSourceFunc)Shoot_Bullet, cr2);


    gtk_main ();

    destroy_surfaces();

        return 0;
}
