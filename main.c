// test

//sdlMixer
#include "SDL/SDL_mixer.h"
#include "SDL/SDL.h"
//sdlMixer

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h> // for keyboard，GDK_Up is declared here 
#include <stdbool.h> // for bool
#include <glib.h> // for garray
#include <stdlib.h> // for malloc
#include <sys/time.h>   // clock_t, clock, CLOCKS_PER_SEC 
#include <stdlib.h> // for random
#include <stdio.h>//itoa

#include <time.h>
#include <string.h>
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
#define _start 0
#define _battle 1
#define _pause 2
#define _win 3
#define _lose 4
#define _story 6
#define _close 5
#define _back 7
#define _rm 0
#define _cron 2
#define _cp 3
#define _fork 1
// game state 0==start  1==battle  2==pause  3==end(win)  4==end(lose)
// music
Mix_Music *music = NULL;

int storyPic = 1;

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
const char *RLBoss[2] = {"image/android_l.png", "image/android_r.png"};
int turn = _R;
bool is_show_item = false;
int game_state = 0; // game state
int arrow_x = 195; // arrow's place in x
int arrow_y = 125; // arrow's place in y
//se
Mix_Chunk *tux = NULL;
Mix_Chunk *android = NULL;
Mix_Chunk *hitten = NULL;
Mix_Chunk *Tbullet = NULL;
Mix_Chunk *GetItem = NULL;
Mix_Chunk *protect = NULL;
void loadSE(){
	tux = Mix_LoadWAV( "music/tux.wav" );
	android = Mix_LoadWAV( "music/android.wav" );
	hitten = Mix_LoadWAV( "music/hitten.wav" );
	Tbullet = Mix_LoadWAV( "music/3bullet.wav" );
	GetItem = Mix_LoadWAV( "music/item.wav" );
	protect = Mix_LoadWAV( "music/protect.wav" );
if(tux == NULL || android == NULL || hitten == NULL || Tbullet == NULL || GetItem == NULL || protect ==NULL){
	printf("SE something goes wrong\n");
}
}
void BGMswitch(int state){
	switch(state){
	case _start:{
		Mix_HaltMusic();
		Mix_FreeMusic( music );
		music=Mix_LoadMUS("music/title.mp3");
		Mix_PlayMusic(music, -1);
		break;
		}
	case _story:{
		Mix_HaltMusic();
		Mix_FreeMusic( music );
		music=Mix_LoadMUS("music/story_normal.mp3");
		Mix_PlayMusic(music, -1);
		break;
		}
	case _battle:{
		Mix_HaltMusic();
		Mix_FreeMusic( music );
		music=Mix_LoadMUS("music/battle.mp3");
		Mix_PlayMusic(music, -1);
		break;
		}
	case _win:{
		Mix_HaltMusic();
		Mix_FreeMusic( music );
		music=Mix_LoadMUS("music/win.mp3");
		Mix_PlayMusic(music, -1);
		break;
		}
	case _lose:{
		Mix_HaltMusic();
		Mix_FreeMusic( music );
		music=Mix_LoadMUS("music/lose.mp3");
		Mix_PlayMusic(music, -1);
		break;
		}
	case _pause:{
		Mix_PauseMusic();
		break;
		}
	case _back:{
		Mix_ResumeMusic();
		break;
		}
	}
}
void draw_story(GdkGC *gc, GdkDrawable *drawable){
	char numbuf[5];	
	sprintf(numbuf,"%d",storyPic);
	
	char s[100] = "opening/";
	strcat(s, numbuf);
	strcat(s, ".png");
	printf("%s\n", s);
	if(storyPic < 18){
	gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file(s, NULL), 0, 0, 0, 0, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
	}
	else{
	storyPic = 1;
	game_state = _battle;
	BGMswitch(_battle);
	}	
}
bool checkCollision( int type );

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


void draw_player_life( GtkWidget *widget, GdkGC *gc, GdkDrawable *drawable )
{
	if( player.life == 0 )
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

gboolean boss3_skill(GtkWidget *widget)
{
	printf("enter boss3_skill timer\n");
	GdkGC *gc = widget->style->fg_gc[GTK_WIDGET_STATE(widget)];
    GdkDrawable *drawable = widget->window;
	static move_stage = 5;
	static change = -1; 
	// 5(boss position)->4->3->2->1->0(player position)->1->2->3->4->5
	calculate_boss3_pos();
	boss_3.x = player.x+(boss_3.x-player.x)/5*move_stage;
	boss_3.y = player.y+(boss_3.y-player.y)/5*move_stage;
	// printf("boss_3.x = %d\n",boss_3.x);
	/*gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/android_l.png", NULL), 0, 0, boss_3.x, boss_3.y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);*/
	move_stage += change;
	if( move_stage == 0 )
		change = 1;
	if( move_stage == 6 && change == 1 )
		return false;
		
	return true;
	
} 




void draw_boss3( GtkWidget *widget )
{
	// count time	
	GdkGC *gc = widget->style->fg_gc[GTK_WIDGET_STATE(widget)];
    GdkDrawable *drawable = widget->window;
	static gdouble ms = 0;
	static GTimer *timer;
	static use = false;
	static skill_complete = false;
	
	if( ms == 0 )
	{
		timer = g_timer_new();
		g_timer_start( timer );
	}
//	g_timer_stop( timer );
	ms = g_timer_elapsed( timer, NULL );
//	printf("time = %f\n",(float)ms);
	

	
	if( (float)(boss_3.life/fullblood) < (float)(1.0/3.0) && !skill_complete )
	{
		static move_stage = 5;
		static change = -1; 
		// 5(boss position)->...->0(player position)->...s->5
		if( ms > 0.04 )
		{
			calculate_boss3_pos();
			boss_3.x = player.x+(boss_3.x-player.x)/5*move_stage;
			boss_3.y = player.y+(boss_3.y-player.y)/5*move_stage;
			move_stage += change;
			if( move_stage == 0 )
				change = 1;
			if( move_stage == 6 && change == 1 )
			{
				// g_timer_destroy(timer);
				skill_complete = true;
			}
			g_timer_start( timer );
		}
		
	}
	else
	{
		calculate_boss3_pos();
	}
/*	printf("boss_3.x = %d\n",boss_3.x);
	printf("complete = %d\n",skill_complete);
	if( (float)(boss_3.life/fullblood) < (float)(1.0/3.0) )
	{
		if( !use )
		{
			printf("enter life if -in main.c 179 \n");
			use = true;
			skill_complete = g_timeout_add(1000, (GSourceFunc)boss3_skill, widget);
		}
	}
	else
	{
		printf("enter else\n");
		calculate_boss3_pos();
	}*/

	gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file(RLBoss[turn], NULL), 0, 0, boss_3.x, boss_3.y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
}





void cleanup()//reset all data
{
	initCHARA( &player, 0, 0, player_width, player_height, 7, 0, 3 );
	calculate_boss3_pos();
	initCHARA( &boss_3, boss_3.x, boss_3.y, boss_width, boss_height, 7, 1, 100 );
	g_ptr_array_free (player_bullet, TRUE);
	g_ptr_array_free (boss_bullet, TRUE);
	g_ptr_array_free (item, TRUE);
	
	player_bullet = g_ptr_array_new();
	boss_bullet = g_ptr_array_new();
	item = g_ptr_array_new();
	
	player_bullet_num = 0;
	boss_bullet_num = 0;
	item_num = 0;
	
	printf("enter cleaning\n");
	fullblood = 100;
	use_skill_boss3 = false;
	invin = false;
	storyPic = 1;
	dir_shoot = -1;
	
}
void drawfirstdisplay(GdkGC *gc, GdkDrawable *drawable) // start page
{
	gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/home.png", NULL), 0, 0, 0, 0, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);//load background
	gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/arrow1.png", NULL), 0, 0, arrow_x, arrow_y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);//load arrow
}






gboolean expose_event_callback(GtkWidget *widget, 
                               GdkEventExpose *event, 
                               gpointer data)
{
    GdkGC *gc = widget->style->fg_gc[GTK_WIDGET_STATE(widget)];
    GdkDrawable *drawable = widget->window;
//  GdkColor color;

//	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("chara.png", NULL);
//	gdk_draw_pixbuf(drawable, gc, pixbuf, 0, 0, x, y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);  

// game state 0==start  1==battle  2==pause  3==end(win)  4==end(lose) 
	if( game_state == _start ) // check game state
	{
		drawfirstdisplay(gc, drawable); // show the start page
		//playmusic
		Mix_PlayMusic(music, -1);
		cleanup(); // reset all data
	}
	else if(game_state == _story){
 		draw_story(gc, drawable);
		// cleanup();
	}
	else if(game_state == _pause ) // pause
	{
		gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/pause.png", NULL), 0, 0, 0, 0, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);//load pause background
	}
	else if(game_state == _win ) // enter win page
	{
		BGMswitch(_win);
		gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/win.jpg", NULL), 0, 0, 0, 0, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);//load background
	}
	else if(game_state == _lose ) // enter lose page
	{
		BGMswitch(_lose);
		gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/lose.jpg", NULL), 0, 0, 0, 0, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);//load background
	}
	else if(game_state == _close ) // close window 
	{
		// gtk_widget_destroy(GTK_WIDGET(widget));
		Mix_FreeChunk(tux);
		Mix_FreeChunk(android);
		Mix_FreeChunk(hitten);
		Mix_FreeChunk(Tbullet);
		Mix_FreeChunk(GetItem);
		Mix_FreeChunk(protect);
		Mix_FreeMusic( music );
		gtk_main_quit();
	}
	else if( game_state == _battle ) // battle
	{   
		// printf( "boss3 life = %d\n",boss_3.life );
		// drawbackground
		gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/battle.png", NULL), 0, 0, 0, 0, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
		if(boss_3.life <= 0) // player win
		{
			game_state = 3; // change state
			gtk_widget_queue_draw(widget);
			return;
		}
			

	
		gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file(RLchara[turn], NULL)
		, 0, 0, player.x, player.y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
	
		draw_player_life( widget, gc, drawable );
	
		if( stage == 3 )
			draw_boss3( widget );
	
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
			gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/boss_bullet.png", NULL)
				, 0, 0, t->x, t->y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
		}

		if( defense )
			make_defense( gc, drawable );
	
	
		for( i = 0; i < item_num; i++ ) // draw item
		{
			ITEM *tmp = g_ptr_array_index( item , i);
			switch(tmp->type){
			case _cron :
			gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/itemCRON.png", NULL)
				, 0, 0, tmp->x, tmp->y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
			break;
			case _rm :
			gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/itemRM.png", NULL)
				, 0, 0, tmp->x, tmp->y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
			break;
			case _cp :
			gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/itemCP.png", NULL)
				, 0, 0, tmp->x, tmp->y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
			break;
			case _fork :
			gdk_draw_pixbuf(drawable, gc, gdk_pixbuf_new_from_file("image/itemFORK.png", NULL)
				, 0, 0, tmp->x, tmp->y, -1, -1, GDK_RGB_DITHER_NORMAL, 0, 0);
			break;
			default:
			break;
			}
		}
	
	 	// blood line of boss	
		double boss_life = boss_3.life;
		double length = window_width * (double)(boss_3.life/fullblood);
		gdk_draw_rectangle( drawable, gc, 1, 0, 0, length, 20);
	}

		
	
 
    return TRUE;
}

int main(int argc, char *argv[]) {
    //init music
    if(Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1)
	printf("fail to init mixer.");
    Mix_Init(MIX_INIT_MP3);
    music=Mix_LoadMUS("music/title.mp3");
    SDL_Init(SDL_INIT_AUDIO);
    loadSE();
    //init music
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

