
void make_player_bullet()
{
	// 0 == up // 1 == left // 2 == down // 3 == right
    BULLET *tmp = malloc(sizeof(BULLET));
    tmp->dx = 0;
    tmp->dy = 0;
    
    switch( dir_shoot ){   // 键盘键值类型  
      /*  case 0: // up
        	tmp->x = player.x + (player.width-bullet_width)/2;
        	tmp->y = player.y;
        	tmp->dy = -10;
        	printf("W\n");
        	break;*/
        case 1: // left
        	tmp->x = player.x;
        	tmp->y = player.y + (player.height-bullet_height)/2;
        	tmp->dx = -10;
        	printf("A\n");
        	break;
     /*   case 2: // down
        	tmp->x = player.x + (player.width-bullet_width)/2;
        	tmp->y = player.y + player.height-bullet_height;	
        	tmp->dy = 10;
        	printf("S\n");
        	break;*/
        case 3: // right
        	tmp->x = player.x + player.width-bullet_width;
        	tmp->y = player.y + (player.height-bullet_height)/2;        	
        	tmp->dx = 10;
        	printf("D\n");
        	break;
        default:
        	break;
    }  
	
	if( dir_shoot==1 || dir_shoot==3 )
	{
		tmp->mode = player_bullet_mode;
		g_ptr_array_add( player_bullet, tmp );
		player_bullet_num++;
    }
    else
    	free(tmp);


}

void make_boss_3_bullet()
{
	// 0 == up // 1 == left // 2 == down // 3 == right
    BULLET *tmp = malloc(sizeof(BULLET));
    tmp->dx = 0;
    tmp->dy = 0;
    
    switch( dir_shoot ){   // 键盘键值类型  
    /*    case 0: // up
        	tmp->x = boss_3.x + (boss_3.width-bullet_width)/2;
        	tmp->y = boss_3.y;
        	tmp->dy = -10;
        	//printf("W\n");
        	break;*/
        case 1: // left
			tmp->x = boss_3.x + boss_3.width-bullet_width;
        	tmp->y = boss_3.y + (boss_3.height-bullet_height)/2;        	
        	tmp->dx = 10;
        	//printf("A\n");
        	break;
    /*    case 2: // down
        	tmp->x = boss_3.x + (boss_3.width-bullet_width)/2;
        	tmp->y = boss_3.y + boss_3.height-bullet_height;     
        	tmp->dy = 10;
        	//printf("S\n");
        	break;*/
        case 3: // right
        	tmp->x = boss_3.x;
        	tmp->y = boss_3.y + (boss_3.height-bullet_height)/2;
        	tmp->dx = -10;
        	//printf("D\n");
        	break;
        default:
        	break;
    }  
    
	if( dir_shoot==1 || dir_shoot==3 )
	{
		tmp->mode = player_bullet_mode;
		g_ptr_array_add( boss_bullet, tmp );
		boss_bullet_num++;
    }
    else
    	free(tmp);


}



bool checkCollision( int type )
{ // type == 1 check player and boss

	static gdouble sec = 0;
	static GTimer *timer;
	static first_call = true;
	
	if( first_call )
	{
		timer = g_timer_new();
	}
	if( sec == 0 )
		g_timer_start( timer );
	

	sec = g_timer_elapsed( timer, NULL );
	


	if( type == 1 )
	{
		if( player.x+player.width > boss_3.x && player.x < boss_3.x+boss_3.width
		&& player.y+player.height > boss_3.y && player.y < boss_3.y+boss_3.height &&( first_call || sec > 0.5 ))
		{
			first_call = false;
			g_timer_start( timer );
			printf("collision\n");
			player.life--;
		}
		
	}

}

void checkEatItem()
{
	if( item_num == 0 )
		return;
	int i;
	for( i = 0; i < item_num; i++ )
	{
		ITEM *tmp = g_ptr_array_index( item , i);
		if( player.x+player.width > tmp->x && player.x < tmp->x+_item_width
			&& player.y+player.height > tmp->y && player.y < tmp->y+_item_height )
		{
			
			switch(tmp->type)
			{
			case 0: printf("eat item 0\n"); break;
			case 1: printf("eat item 1\n"); break;
			case 2: printf("eat item 2\n"); break;
			case 3: printf("eat item 3\n"); break;
			
			
			}
			

			item_num--;
			tmp = g_ptr_array_remove_index(item,i);
			free(tmp);
			i--;
		
		}
	}


}

gboolean show_item(gpointer data) // 30% Probability show item
{
	if( game_state != 1 )
		return true;
	
	printf("enter show_item\n");
	if( rand()%3 == 1 || rand()%3 == 2 )
		return true;
	if( item_num == 3 )
	{
		printf("item full\n");
		return true;
	}	
	
	ITEM *tmp = malloc(sizeof(ITEM));
	
	if( player.x >= window_width/2 ) // player in the right screen
		tmp->x = (rand()%window_width)/2+400; // 400-800
	else // player in the left screen
		tmp->x = (rand()%window_width)/2; // 0-400
		
	if( tmp->x+_item_width > window_width )
		tmp->x -= _item_width;
	
	tmp->y = (rand()%window_height);	
	if(tmp->y+_item_height > window_height)
		tmp->y -= _item_height;	
	
	tmp->type = rand()%4;  // 0-3
	g_ptr_array_add( item, tmp );
	item_num++;
	
	printf("put\n");
	return true;

}


gboolean deal_bullet_shoot(gpointer data) // press keyboard long time
{
//	printf("deal shoot = %d\n",dir_shoot);	
	if( game_state != 1 )
		return true;
		
	if( dir_shoot != -1 )
	{
    	make_player_bullet();
		if( stage == 3 )
			make_boss_3_bullet();
	}
	return true;

}

gboolean player_move(gpointer data)
{
	if( game_state != 1 )
		return true;
		
	if ( dir_move[0] )
       	if( player.y - player.speed >= 0 )
       		player.y -= player.speed;

    if( dir_move[1] )
		if( player.x - player.speed >= 0 )
			player.x -= player.speed;  
			
    if( dir_move[2] )
		if( player.y + player.height <= window_height )
			player.y += player.speed; 
			
    if( dir_move[3] )
		if( player.x + player.width <= window_width )
			player.x += player.speed; 
			
	
	if( (dir_move[0] || dir_move[1] || dir_move[2] || dir_move[3]) && stage == 3 )
		calculate_boss3_pos();
		
	checkCollision(1);
	checkEatItem();
	
	return true;
}



gboolean shoot_bullet(gpointer data) {
    // static gint count = 0;
	if( game_state != 1 )
		return true;
		
	int i;
	for( i = 0; i < player_bullet_num; i++ )
	{
		BULLET *tmp = g_ptr_array_index( player_bullet , i);
		// check bullet attack boss
		if((tmp->x >= boss_3.x && tmp->x <= boss_3.x + boss_width && tmp->y >= boss_3.y && tmp->y<= boss_3.y+boss_height))	{
			boss_3.life -= 20;
			player_bullet_num--;
			tmp = g_ptr_array_remove_index(player_bullet,i);
			free(tmp);
			i--;
		}
		// check bullet out of range
		else{
			tmp->x += tmp->dx;
			tmp->y += tmp->dy;
			if( tmp->x < 0 || tmp->x > window_width
			||  tmp->y < 0 || tmp->y > window_height )
			{
			
				player_bullet_num--;
				tmp = g_ptr_array_remove_index(player_bullet,i);
				free(tmp);
				i--;
			}
		}

	
	}
	
	for( i = 0; i < boss_bullet_num; i++ )
	{
		BULLET *tmp = g_ptr_array_index( boss_bullet , i);
		tmp->x += tmp->dx;
		tmp->y += tmp->dy;
		
		// check bullet attack player
	/*	if( tmp->y+bullet_height > player.y && tmp->y < player.y+120 
      	&& tmp->x > player.x && tmp->x+bullet_width < player.x+120 )*/
		if( tmp->y+bullet_height >= player.y && tmp->y <= player.y+player.height 
      	&& tmp->x >= player.x && tmp->x+bullet_width <= player.x+player.width )
      	{
      		if( defense )
      			defense = false;
      		else if( !invin ) // !defense
      			player.life--;
      		boss_bullet_num--;
			tmp = g_ptr_array_remove_index(boss_bullet,i);
			free(tmp);
			i--;
      	}
		// check boss bullet out of range
		else if( tmp->x < 0 || tmp->x > window_width
		||  tmp->y < 0 || tmp->y > window_height )
		{
			
			boss_bullet_num--;
			tmp = g_ptr_array_remove_index(boss_bullet,i);
			// printf("test free = %d\n", tmp->x);
			free(tmp);
			i--;
		}
	
	}
	
	
		
	gtk_widget_queue_draw(wid);
    return TRUE;

}


