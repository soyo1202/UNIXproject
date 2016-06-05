// test

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h> // for keyboard，GDK_Up is declared here 
#include <stdbool.h> // for bool
#include <glib.h> // for garray
#include <stdlib.h> // for malloc
#include <sys/time.h>   // clock_t, clock, CLOCKS_PER_SEC 
#include <stdlib.h> // for random
#include <time.h>

#include "chara.h"
#include "bullet.h"
#include "item.h"

#define player_width   72
#define player_height  72
#define boss_width     60
#define boss_height    60
#define window_width  800
#define window_height 600
#define _R 0
#define _L 1
#define _item_width	   40
#define _item_height   40


CHARA player;
CHARA boss_3;
int player_bullet_num = 0;
int boss_bullet_num = 0;
int item_num = 0;
int bullet_width = 20, bullet_height = 20;
int player_bullet_mode = 0;
GtkWidget *wid;
GPtrArray *player_bullet;
GPtrArray *boss_bullet;
GPtrArray *item;
bool dir_move[4] = {false, false, false, false}; // up left down right
int dir_shoot = -1;
int stage = 3; // 關卡
double fullblood = 100;
bool use_skill_boss3 = false; // 是否使用過絕招
bool defense = false;
bool invin = false; // 無敵
const char *RLchara[2] ={"image/chara_r.png", "image/chara_l.png"};
int turn = _R;
bool is_show_item = false;
int game_state = 0;//game state
int arrow_x = 195;//arrow's place in x
int arrow_y = 125;//arrow's place in y



bool checkCollision( int type );

void drawfirstdisplay(GdkGC *gc, GdkDrawable *drawable)//start page
{
	gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/home.png", NULL), 0, 0, 0, 0, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);//load background
	gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/arrow1.png", NULL), 0, 0, arrow_x, arrow_y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);//load arrow
}

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
	static use = false;

	if( ms == 0 )
	{
		timer = g_timer_new();
		g_timer_start( timer );
	}
//	g_timer_stop( timer );
	ms = g_timer_elapsed( timer, NULL );
//	printf("time = %f\n",(float)ms);
	

	// check unique skill for boss_3
	if( ((float)(boss_3.life/fullblood) < (float)(1.0/3.0) && !use ) || ( use && ms < 0.35 ))
	{
		boss_3.x = player.x-(boss_width-player_width)/2;
		boss_3.y = player.y-(boss_height-player_height)/2;
		gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/android_l.png", NULL), 0, 0, boss_3.x, boss_3.y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
		if( !use )
		{
			use = true;
			g_timer_start( timer );
		//	player.life--;
		}
	}
	else
	{
		calculate_boss3_pos();
		gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/android_l.png", NULL), 0, 0, boss_3.x, boss_3.y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
	}

}

void draw_player_life( GtkWidget *widget, GdkGC *gc, GdkDrawable *drawable )
{
	if(player.life == 0)
	{	
		game_state = 4; // player lose
		gtk_widget_queue_draw(widget);
	}
	switch (player.life)
	{
		case 5:
			gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/life.png", NULL), 0, 0, 220, 550, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
		case 4:
			gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/life.png", NULL), 0, 0, 170, 550, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
	
		case 3:
			gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/life.png", NULL), 0, 0, 120, 550, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
		case 2:
			gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/life.png", NULL), 0, 0, 70, 550, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
		case 1:
			gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/life.png", NULL), 0, 0, 20, 550, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
		default:
			break;
	}


}

void make_defense( GdkGC *gc, GdkDrawable *drawable )
{
	
	static gdouble sec = 0;
	static GTimer *timer;
	static first_call = true;
	
	if( first_call )
	{
		timer = g_timer_new();
		first_call = false;
	}
	if( sec == 0 )
		g_timer_start( timer );
	

	sec = g_timer_elapsed( timer, NULL );
	printf("sec = %f\n",sec);
	if( sec > 0.6 )
	{
		defense = false;
		g_timer_start(timer);
		sec = 0;
		//sec = g_timer_elapsed( timer, NULL );
		printf("iner sec = %f\n",sec);
	}
		
	
	gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/defense.png", NULL), 0, 0, player.x-24, player.y-24, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
	
	
	
	

}



#include "timer.c"
#include "controlKeyboard.c"

void cleanup()//reset all data
{
	initCHARA( &player, 0, 0, player_width, player_height, 7, 0, 3 );
	calculate_boss3_pos();
	initCHARA( &boss_3, boss_3.x, boss_3.y, boss_width, boss_height, 7, 1, 100 );

}



gboolean expose_event_callback(GtkWidget *widget, 
                               GdkEventExpose *event, 
                               gpointer data)
{
    GdkGC *gc = widget->style->fg_gc[GTK_WIDGET_STATE(widget)];
    GdkDrawable *drawable = widget->window;
//    GdkColor color;

//	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("chara.png", NULL);
//	gdk_draw_pixbuf(drawable, gc, pixbuf, 0, 0, x, y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);  
	
	if(game_state == 0)//check game state
	{
		drawfirstdisplay(gc, drawable);//show the start page
		cleanup();//reset all data
	}
	else if(game_state == 1){//enter game

	
		gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/battle.png", NULL), 0, 0, 0, 0, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);//load background
		gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file(RLchara[turn], NULL)
		, 0, 0, player.x, player.y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
	
		draw_player_life( widget, gc, drawable );
	
			if( stage == 3 )
			draw_boss3( gc, drawable );
		int i;	
		if(player_bullet_mode == 0)//check bullet mode
		{
	
		for( i = 0; i < player_bullet_num; i++ )//push one shoot one
		{
			BULLET *tmp = g_ptr_array_index(player_bullet, i);	
			gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/bullet.png", NULL)
				, 0, 0, tmp->x, tmp->y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
		}
	}
	else if(player_bullet_mode == 1)//check bullet mode
	{
			
		for( i = 0; i < player_bullet_num; i++ )//push one shoot three
		{
			
			BULLET *tmp = g_ptr_array_index(player_bullet, i);
			gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/bullet.png", NULL)
				, 0, 0, tmp->x, tmp->y-25, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);	
			gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/bullet.png", NULL)
				, 0, 0, tmp->x, tmp->y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
			gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/bullet.png", NULL)
				, 0, 0, tmp->x, tmp->y+25, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);	
		
		}
	}
	for( i = 0; i < boss_bullet_num; i++ )
	{
		BULLET *t = g_ptr_array_index(boss_bullet, i);	
		gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/bullet.png", NULL)
			, 0, 0, t->x, t->y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
	}
	
	if( defense )
		make_defense( gc, drawable );
	
	
	for( i = 0; i < item_num; i++ ) // draw item
	{
		ITEM *tmp = g_ptr_array_index( item , i);
		gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/item.png", NULL)
			, 0, 0, tmp->x, tmp->y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
	}
	
 	// blood line of boss	
	double boss_life = boss_3.life;
	double length = window_width * (double)(boss_3.life/fullblood);
	gdk_draw_rectangle( drawable, gc, 1, 0, 0, length, 20);
	if(boss_3.life == 0)//player win
		game_state = 3;//change state
	}
 	else if(game_state == 2)//pause
	{
	
	gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/pause.png", NULL), 0, 0, 0, 0, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);//load pause background
		
	}
	else if(game_state == 3)//enter win page
	{
		gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/win.jpg", NULL), 0, 0, 0, 0, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);//load background
	}
	 else if(game_state == 4)//enter lose page
	{
	gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/lose.jpg", NULL), 0, 0, 0, 0, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);//load background
	}
	else if(game_state == 5)//close window !!!!!!!!!!cannot close the window!!!!!!!!!!!!!
	{
		gtk_widget_destroy(GTK_WIDGET(widget));
		
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


	g_timeout_add(  50, (GSourceFunc)shoot_bullet, NULL);
	g_timeout_add(  50, (GSourceFunc)player_move, NULL);
	g_timeout_add( 150, (GSourceFunc)deal_bullet_shoot, NULL);
	g_timeout_add(2000, (GSourceFunc)show_item, NULL);
	
	
	// var
	wid = window;
	player_bullet = g_ptr_array_new();
	boss_bullet = g_ptr_array_new();
	item = g_ptr_array_new();
	//  initCHARA( CHARA chara, int x, int y, int width, int height, int speed, int type, int life)
	initCHARA( &player, 0, 0, player_width, player_height, 7, 0, 3 );
	calculate_boss3_pos();
	initCHARA( &boss_3, boss_3.x, boss_3.y, boss_width, boss_height, 7, 1, 100 );
	srand(time(NULL));
	
	
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

