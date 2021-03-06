
void checkPlayerMove( GdkEventKey *event, bool move)
{
	/*static gdouble ms = 0;
	static GTimer *timer;
	if( ms == 0 )
	{
		timer = g_timer_new();
		g_timer_start( timer );
	}
	ms = g_timer_elapsed( timer, NULL );*/
	
    switch(event->keyval){   // 键盘键值类型  
        case GDK_Up: 
		    dir_move[0] = move;
            break;  
            
        case GDK_Left:
		   	dir_move[1] = move;
		   	//if( ms > 0.1)
		   	//{
		   		turn = _L;
		   	//	g_timer_start( timer );
		   	//}
            break;  
            
        case GDK_Down:
        	dir_move[2] = move;
            break;  
            
        case GDK_Right:
		    dir_move[3] = move;
		    //if( ms > 0.1 )
		    //{
		    	turn = _R;
		    //	g_timer_start( timer );
            //}
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
	   // case 'a': case 'A':
	    	
	    //	break;
	  /*  case 's': case 'S':
			dir_shoot = shoot ? 2 : -1 ;
	    	break;*/
	    case 'd': case 'D':
	    	if( turn == _R )
				dir_shoot = shoot ? 3 : -1 ;
		if( turn == _L )
				dir_shoot = shoot ? 1 : -1 ;
		
		
	    	break;  
		default:
			return;
    }

}

void checkOther( GdkEventKey  *event )
{
	switch(event->keyval){   // 键盘键值类型  
	    case 'q': case 'Q':{
			defense = true;
			Mix_PlayChannel( -1, protect, 0 );
		}
		case 'p': case 'P':
			invin = !invin;		
		default:
			break;
    }

}
void checkScreen( GdkEventKey  *event )
{
	if(game_state == _story && event->keyval == GDK_space){
		
		printf("%s\n", "up_space");
		storyPic++;
		int wait = 0;
		while(wait > 1000000){
		wait++;
		}
	}
	switch(event->keyval){   // 键盘键值类型  
		case GDK_Up: 
		    if( game_state == _start )		
				arrow_y = 125; // change arrow y place
            break;  
            
        case GDK_Down:
        	if(game_state == _start)
				arrow_y = 210; // change arrow y place
            break;  
	
		case GDK_Escape: // pause in game
			if(game_state == _battle ){ // pause in game
				game_state = _pause;
				BGMswitch(_pause);
			}
			break;
			
	    case GDK_space: // continue game
	    	if( game_state == _start && arrow_y == 125 ){ // choose start
				game_state = _story;
				BGMswitch(_story);
			}
			else if( game_state == _start && arrow_y == 210 ) // choose exit
				gtk_main_quit();
			else if(game_state == _pause){
				game_state = _battle;
				BGMswitch(_back);
			}
			else if(game_state == _win || game_state == _lose ){ // return to start menu
				game_state = _start;
				BGMswitch(_start);
			}
			break;
		
	
	/*    case GDK_Return:
			if(game_state == _win ||game_state == _lose ){ // return to start menu
				game_state = _start;
				BGMswitch(_start);
			}
			break;*/
					
		default:
			break;
	
	
	
	}
}

gboolean deal_key_press(GtkWidget *widget, GdkEventKey  *event, gpointer data)
{  
	if( game_state == 1 )
	{
		checkPlayerMove( event, true );
		checkBulletShoot( event, true );
		checkOther( event );
    }
    checkScreen( event );

  	gtk_widget_queue_draw(widget);
    return TRUE;  
}  
  

gboolean deal_key_release(GtkWidget *widget, GdkEventKey  *event, gpointer data)  
{
	if( game_state == 1 )
	{
		checkPlayerMove( event, false );
		checkBulletShoot( event, false );
	}
	
	return TRUE; 
}


