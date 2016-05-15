
void checkPlayerMove( GdkEventKey *event, bool move)
{
    switch(event->keyval){   // 键盘键值类型  
        case GDK_Up: 
        	dir_move[0] = move;
        /*	if( player.y - player.speed >= 0 )
        		player.y -= player.speed; */
            // printf("Up\n");  
            break;  
        case GDK_Left:
       	 	dir_move[1] = move;
        /*	if( player.x - player.speed >= 0 )
				player.x -= player.speed;  */
            // printf("Left\n");  
            break;  
        case GDK_Down:
        	dir_move[2] = move;
        	/*	if( player.y + player.height <= window_height )
				player.y += player.speed; */ 
            // printf("Down\n");  
            break;  
        case GDK_Right:
        	dir_move[3] = move;
        
            /*	if( player.x + player.width <= window_width )
				player.x += player.speed;  */
            // printf("Right\n");  
            break;  
        default:
			break;
	}
}

void checkBulletShoot( GdkEventKey  *event, bool shoot )
{
	switch(event->keyval){   // 键盘键值类型  
	  /*  case 'w': case 'W':
			dir_shoot = shoot ? 0 : -1 ;
	    	printf("shoot = %d\n",shoot);
	    	break;*/
	    case 'a': case 'A':
			dir_shoot = shoot ? 1 : -1 ;
	    	// printf("A\n");
	    	break;
	  /*  case 's': case 'S':
			dir_shoot = shoot ? 2 : -1 ;
	    	// printf("S\n");
	    	break;*/
	    case 'd': case 'D':
			dir_shoot = shoot ? 3 : -1 ;
	   	 	// printf("D\n");
	    	break;
		default:
			break;
    }

}



gboolean deal_key_press(GtkWidget *widget, GdkEventKey  *event, gpointer data)
{  
    checkPlayerMove( event, true );
    checkBulletShoot( event, true );

  	gtk_widget_queue_draw(widget );
    return TRUE;  
}  
  

gboolean deal_key_release(GtkWidget *widget, GdkEventKey  *event, gpointer data)  
{
	checkPlayerMove( event, false );
	checkBulletShoot( event, false );
	
	return TRUE; 
}


