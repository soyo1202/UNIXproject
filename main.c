#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h> // for keyboard，GDK_Up is declared here 
#include <stdbool.h> // for bool
#include <glib.h> // for garray
#include <stdlib.h> // for malloc
#include <sys/time.h>   // clock_t, clock, CLOCKS_PER_SEC 

#include "chara.h"
#include "bullet.h"

#define player_width   72
#define player_height  72
#define boss_width    216
#define boss_height   216
#define window_width  800
#define window_height 600
#define _R 0
#define _L 1



CHARA player;
CHARA boss_3;
int player_bullet_num = 0;
int boss_bullet_num = 0;
int bullet_width = 10, bullet_height = 10;
int player_bullet_mode = 0;
GtkWidget *wid;
GPtrArray *player_bullet;
GPtrArray *boss_bullet;
bool dir_move[4] = {false, false, false, false}; // up left down right
int dir_shoot = -1;
int stage = 3; // 關卡
double fullblood = 100;
const char *RLchara[2] ={"image/chara.png", "image/chara_l.png"};
int turn = _R;


void calculate_boss3_pos()
{
	boss_3.y = player.y-(boss_height/2-player_height/2);
	if( boss_3.y < 0 )
		boss_3.y = 0;
	else if( boss_3.y+boss_height > window_height )
		boss_3.y = window_height-boss_height;	
	
	boss_3.x = window_width-(player.x+player_width/2)-boss_width/2;
	if( boss_3.x < 0 )
		boss_3.x = 0;
	else if( boss_3.x+boss_width > window_width )
		boss_3.x = window_width-boss_width;
}

void draw_boss3( GdkGC *gc, GdkDrawable *drawable )
{
	// count time	
	static gdouble ms = 0;
	static GTimer *timer;
	if( ms == 0 )
	{
		timer = g_timer_new();
		g_timer_start( timer );
	}
//	g_timer_stop( timer );
	ms = g_timer_elapsed( timer, NULL );
//	printf("time = %f\n",(float)ms);
	
	
	
	
	
	// check unique skill for boss_3
	if( (float)(boss_3.life/fullblood) < (float)(1.0/3.0) )
	{
	//	unique_skill_for_boss3( gc, drawable );
		printf("got");
	}
	else
		gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/BOSS.png", NULL), 0, 0, boss_3.x, boss_3.y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);

}

#include "timer.c"
#include "controlKeyboard.c"





gboolean expose_event_callback(GtkWidget *widget, 
                               GdkEventExpose *event, 
                               gpointer data) 
{
                               
    GdkGC *gc = widget->style->fg_gc[GTK_WIDGET_STATE(widget)];
    GdkDrawable *drawable = widget->window;
//    GdkColor color;

//	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("chara.png", NULL);
//	gdk_draw_pixbuf(drawable, gc, pixbuf, 0, 0, x, y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);  

	

	gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file(RLchara[turn], NULL)
	, 0, 0, player.x, player.y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
	
	
	int i;
	for( i = 0; i < player_bullet_num; i++ )
	{
		BULLET *tmp = g_ptr_array_index(player_bullet, i);	
		gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/bullet.png", NULL)
			, 0, 0, tmp->x, tmp->y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
	}
	for( i = 0; i < boss_bullet_num; i++ )
	{
		BULLET *t = g_ptr_array_index(boss_bullet, i);	
		gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/bullet.png", NULL)
			, 0, 0, t->x, t->y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
	}

 	// blood line of boss	
	double boss_life = boss_3.life;
	double length = window_width * (double)(boss_3.life/fullblood);
	gdk_draw_rectangle( drawable, gc, 1, 0, 0, length, 20);
	// check unique skill for boss 3
	if( stage == 3 )
		draw_boss3( gc, drawable );
	
 
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


	g_timeout_add( 50, (GSourceFunc)shoot_bullet, NULL);
	g_timeout_add( 50, (GSourceFunc)player_move, NULL);
	g_timeout_add(150, (GSourceFunc)deal_bullet_shoot, NULL);

	
	
	// var
	wid = window;
	player_bullet = g_ptr_array_new();
	boss_bullet = g_ptr_array_new();
	//  initCHARA( CHARA chara, int x, int y, int width, int height, int speed, int type, int life)
	initCHARA( &player, 0, 0, player_width, player_height, 7, 0, 100 );
	calculate_boss3_pos();
	initCHARA( &boss_3, boss_3.x, boss_3.y, boss_width, boss_height, 7, 1, 100 );
	
	
	
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

