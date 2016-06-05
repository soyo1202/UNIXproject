
void checkPlayerMove( GdkEventKey *event, bool move)
{
    switch(event->keyval){   // 键盘键值类型  
        case GDK_Up: 
        	dir_move[0] = move;
		if(game_state == 0)		
			arrow_y = 125;//change arrow y place
            break;  
        case GDK_Left:
       	 	dir_move[1] = move;
       	 	turn = _L;
            break;  
        case GDK_Down:
        	dir_move[2] = move;
		if(game_state == 0)
			arrow_y = 210;//change arrow y place
            break;  
        case GDK_Right:
        	dir_move[3] = move;
        	turn = _R;
            break;  
	case GDK_space://make choice in start page 
		if(game_state == 0 && arrow_y ==125)//choose start
		{	game_state = 1;
			break;
		
		}
		else if(game_state == 0 && arrow_y == 210)//choose exit
		{	game_state = 5;	
			break;
		}
			
	    break;        
	default:
			return;
	}
}

void checkBulletShoot( GdkEventKey  *event, bool shoot )
{
	switch(event->keyval){   // 键盘键值类型  
	  /*  case 'w': case 'W':
			dir_shoot = shoot ? 0 : -1 ;
	    	break;*/
	    case 'a': case 'A':
			dir_shoot = shoot ? 1 : -1 ;
	    	break;
	  /*  case 's': case 'S':
			dir_shoot = shoot ? 2 : -1 ;
	    	break;*/
	    case 'd': case 'D':
			dir_shoot = shoot ? 3 : -1 ;
	    	break;
	    case GDK_Escape://pause in game
		if(game_state == 1 )//pause in game
			game_state = 2;
		break;
	    case GDK_space://continue game
		if(game_state == 2)
			game_state = 1;
		break;
	    case GDK_Return:
		if(game_state == 3 ||game_state == 4 )//return to start menu
			game_state = 0;
		break;
	    case 'w':case 'W'://change bullet mode to push one shoot three
		player_bullet_mode = 1;
		break;
	    case 'e':case 'E'://change bullet mode back to push one shoot one
		player_bullet_mode = 0;
		break;	
		default:
			return;
    }
/*	static gdouble sec = 0;
	static GTimer *timer;
	static first_call = true;
	if( sec == 0 )
	{
		timer = g_timer_new();
		g_timer_start( timer );
	}

	sec = g_timer_elapsed( timer, NULL );
	
	if( dir_shoot != -1 && ( first_call || sec > 0.3 ) )
	{
		first_call = false;
		g_timer_start( timer );
    	make_player_bullet();
		if( stage == 3 )
			make_boss_3_bullet();
	}
	if( !dir_shoot == -1 )
		first_call = true;
*/


}

void checkOther( GdkEventKey  *event )
{
	switch(event->keyval){   // 键盘键值类型  
	    case 'q': case 'Q':
			defense = true;
	    case 'p': case 'P':
			invin = !invin;
		default:
			break;
    }

}


gboolean deal_key_press(GtkWidget *widget, GdkEventKey  *event, gpointer data)
{  
    checkPlayerMove( event, true );
    checkBulletShoot( event, true );
    checkOther( event );

  	gtk_widget_queue_draw(widget );
    return TRUE;  
}  
  

gboolean deal_key_release(GtkWidget *widget, GdkEventKey  *event, gpointer data)  
{
	checkPlayerMove( event, false );
	checkBulletShoot( event, false );
	
	return TRUE; 
}


