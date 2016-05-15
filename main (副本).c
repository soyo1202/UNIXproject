#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>   //键盘头文件，GDK_Up在这声明  
#include <stdbool.h> // for bool
#include <glib.h> // for garray

#include <stdlib.h> // for malloc


#include "chara.h"
#include "bullet.h"

CHARA player;
int bullet_num = 0;
int window_width = 800, window_height = 600;
int bullet_width = 10, bullet_height = 10;
int bullet_mode = 0;
GtkWidget *wid;
GPtrArray *bullet;

gboolean Shoot_Bullet(gpointer data) {
    static gint count = 0;
    //if( bullet_y <= 600 ) {
       // g_signal_emit_by_name(button, "clicked");
       // g_print ("%s\n", (char *)data);
       // g_print ("timer\n");
		int i;
		for( i = 0; i < bullet_num; i++ )
		{
			BULLET *tmp = g_ptr_array_index( bullet , i);
			tmp->x += tmp->dx;
			tmp->y += tmp->dy;
			if( tmp->x < 0 || tmp->x > window_width
			||  tmp->y < 0 || tmp->y > window_height )
			{
				
				bullet_num--;
				tmp = g_ptr_array_remove_index(bullet,i);
				printf("test free = %d\n", tmp->x);
				free(tmp);
				i--;
			}
		
		}
		
		gtk_widget_queue_draw(wid);
        return TRUE;
 //  }
 //   else {
 //   	g_print ("timer expire\n");
 //       return FALSE;
 //   }
}


void controlPlayerMove( GdkEventKey *event )
{
    switch(event->keyval){   // 键盘键值类型  
        case GDK_Up: 
        	if( player.y - player.speed >= 0 )
        		player.y -= player.speed; 
            // printf("Up\n");  
            break;  
        case GDK_Left:
        	if( player.x - player.speed >= 0 )
				player.x -= player.speed;  
            // printf("Left\n");  
            break;  
        case GDK_Right:
        	if( player.x + player.width <= window_width )
				player.x += player.speed;  
            // printf("Right\n");  
            break;  
        case GDK_Down:
        	if( player.y + player.height <= window_height )
				player.y += player.speed;  
            // printf("Down\n");  
            break;  
	}
}
gboolean deal_key_release(GtkWidget *widget, GdkEventKey  *event, gpointer data)  
{
}
gboolean deal_key_press(GtkWidget *widget, GdkEventKey  *event, gpointer data)  
{  
    controlPlayerMove( event );
    int dir = -1; // 0 == up // 1 == left // 2 == down // 3 == right
    BULLET *tmp = malloc(sizeof(BULLET));
    tmp->dx = 0;
    tmp->dy = 0;
    
    switch(event->keyval){   // 键盘键值类型  
        case 'w': case 'W':
        	dir = 0;
        	tmp->x = player.x + (player.width-bullet_width)/2;
        	tmp->y = player.y;
        	tmp->dy = -10;
        	printf("W\n");
        	break;
        case 'a': case 'A':
        	dir = 1;
        	tmp->x = player.x;
        	tmp->y = player.y + (player.height-bullet_height)/2;
        	tmp->dx = -10;
        	printf("A\n");
        	break;
        case 's': case 'S':
        	dir = 2;
        	tmp->x = player.x + (player.width-bullet_width)/2;
        	tmp->y = player.y + player.height-bullet_height;      	
        	tmp->dy = 10;
        	printf("S\n");
        	break;
        case 'd': case 'D':
        	dir = 3;
        	tmp->x = player.x + player.width-bullet_width;
        	tmp->y = player.y + (player.height-bullet_height)/2;        	
        	tmp->dx = 10;
        	printf("D\n");
        	break;
    }  
    if( dir != -1 )
    {
    	tmp->mode = bullet_mode;
    	g_ptr_array_add( bullet, tmp );
    	bullet_num++;
    }
    
    
    
  	gtk_widget_queue_draw(widget );
    return TRUE;  
}  
  

gboolean expose_event_callback(GtkWidget *widget, 
                               GdkEventExpose *event, 
                               gpointer data) {
                               
    GdkGC *gc = widget->style->fg_gc[GTK_WIDGET_STATE(widget)];
    GdkDrawable *drawable = widget->window;
    GdkColor color;

//	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("chara.png", NULL);
//	gdk_draw_pixbuf(drawable, gc, pixbuf, 0, 0, x, y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);  

	gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/chara.png", NULL)
	, 0, 0, player.x, player.y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
	
	int i;
	

/*	BULLET *tmp = malloc(sizeof(BULLET));
	tmp->x = 20;
	tmp->y = 30;
	g_ptr_array_add( bullet, tmp );
	printf( "1 = %d,%d\n", tmp->x, tmp->y );
	
	g_ptr_array_remove_index( bullet, 0 );
	printf( "1 = %d,%d\n", tmp->x, tmp->y );
	free(tmp);*/

// OK	
	for( i = 0; i < bullet_num; i++ )
	{
		BULLET *tmp = g_ptr_array_index(bullet, i);	
		gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/bullet.png", NULL)
			, 0, 0, tmp->x, tmp->y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
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
    gtk_widget_set_size_request(drawing_area, window_width, window_height);
                  
    g_signal_connect(GTK_OBJECT(drawing_area), "expose_event",  
                     G_CALLBACK(expose_event_callback), NULL);

    gtk_container_add(GTK_CONTAINER(window), drawing_area);

    g_signal_connect(GTK_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);
                     
	g_signal_connect(window, "key-press-event",   
                    G_CALLBACK(deal_key_press), NULL);  
	g_signal_connect(window, "key-release-event",   
                    G_CALLBACK(deal_key_release), NULL);  


	g_timeout_add(100, (GSourceFunc)Shoot_Bullet, NULL);

	
	
	// var
	wid = window;
	bullet = g_ptr_array_new();
	//  initCHARA( CHARA chara, int x, int y, int width, int height, int speed, int type, int life)
	initCHARA( &player, 0, 0, 72, 72, 10, 0, 100 );
	
	
	
	
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

