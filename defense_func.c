#ifndef _FUNC_C_
#define _FUNC_C_

#include <stdbool.h> // for bool
#include <math.h>
#include <cairo.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>   //键盘头文件，GDK_Up在这声明  

cairo_surface_t *surface1;
cairo_pattern_t *pattern1;
int x = 340,y = 400;
cairo_t *cr;
cairo_t *cr2;
bool defense = false, shoot = false;

static void create_surfaces ()
{
    surface1 = cairo_image_surface_create_from_png ("chara.png");
	pattern1 = cairo_pattern_create_for_surface (surface1);
}

static void destroy_surfaces ()
{
    g_print ("destroying surfaces\n");
    cairo_surface_destroy (surface1);
	cairo_destroy (cr);

}

gboolean deal_key_press
(GtkWidget *widget, GdkEventKey  *event, gpointer data)  
{  
      
    switch(event->keyval){   // 键盘键值类型  
        case GDK_Up:  
            //printf("Up\n");  
			y -= 10;
            break;  
        case GDK_Left:
			x -= 10;  
            //printf("Left\n");  
            break;  
        case GDK_Right:
			x += 10;  
            //printf("Right\n");  
            break;  
        case GDK_Down:
			y += 10;  
            //printf("Down\n");  
            break;  
		case GDK_KEY_space:
			//printf("Space\n");
			defense = true; 
			break;
    }  
  	gtk_widget_queue_draw(widget);
    return TRUE;  
}  

#endif
