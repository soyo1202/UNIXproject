
void checkPlayerMove( GdkEventKey *event, bool move)
{
    switch(event->keyval){   // 键盘键值类型  
        case GDK_Up: 
        	dir_move[0] = move;
            break;  
        case GDK_Left:
       	 	dir_move[1] = move;
       	 	turn = _L;
            break;  
        case GDK_Down:
        	dir_move[2] = move;
            break;  
        case GDK_Right:
        	dir_move[3] = move;
        	turn = _R;
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
		default:
			return;
    }

	static gdouble sec = 0;
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
	if( !shoot )
		first_call = true;

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


