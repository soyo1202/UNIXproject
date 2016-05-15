void make_player_bullet()
{
	// 0 == up // 1 == left // 2 == down // 3 == right
    BULLET *tmp = malloc(sizeof(BULLET));
    tmp->dx = 0;
    tmp->dy = 0;
    
    switch( dir_shoot ){   // 键盘键值类型  
        case 0: // up
        	tmp->x = player.x + (player.width-bullet_width)/2;
        	tmp->y = player.y;
        	tmp->dy = -10;
        	printf("W\n");
        	break;
        case 1: // left
        	tmp->x = player.x;
        	tmp->y = player.y + (player.height-bullet_height)/2;
        	tmp->dx = -10;
        	printf("A\n");
        	break;
        case 2: // down
        	tmp->x = player.x + (player.width-bullet_width)/2;
        	tmp->y = player.y + player.height-bullet_height;      	
        	tmp->dy = 10;
        	printf("S\n");
        	break;
        case 3: // right
        	tmp->x = player.x + player.width-bullet_width;
        	tmp->y = player.y + (player.height-bullet_height)/2;        	
        	tmp->dx = 10;
        	printf("D\n");
        	break;
        default:
        	break;
    }  

    tmp->mode = player_bullet_mode;
    g_ptr_array_add( player_bullet, tmp );
    player_bullet_num++;


}

void make_boss_3_bullet()
{
	// 0 == up // 1 == left // 2 == down // 3 == right
    BULLET *tmp = malloc(sizeof(BULLET));
    tmp->dx = 0;
    tmp->dy = 0;
    
    switch( dir_shoot ){   // 键盘键值类型  
        case 0: // up
        	tmp->x = boss_3.x + (boss_3.width-bullet_width)/2;
        	tmp->y = boss_3.y;
        	tmp->dy = -10;
        	//printf("W\n");
        	break;
        case 1: // left
        	tmp->x = boss_3.x;
        	tmp->y = boss_3.y + (boss_3.height-bullet_height)/2;
        	tmp->dx = 10;
        	//printf("A\n");
        	break;
        case 2: // down
        	tmp->x = boss_3.x + (boss_3.width-bullet_width)/2;
        	tmp->y = boss_3.y + boss_3.height-bullet_height;      	
        	tmp->dy = 10;
        	//printf("S\n");
        	break;
        case 3: // right
        	tmp->x = boss_3.x + boss_3.width-bullet_width;
        	tmp->y = boss_3.y + (boss_3.height-bullet_height)/2;        	
        	tmp->dx = -10;
        	//printf("D\n");
        	break;
        default:
        	break;
    }  

    tmp->mode = player_bullet_mode;
    g_ptr_array_add( boss_bullet, tmp );
    boss_bullet_num++;


}



gboolean deal_bullet_shoot(gpointer data) // press keyboard long time
{
//	printf("deal shoot = %d\n",dir_shoot);	
	if( dir_shoot != -1 )
	{
    	make_player_bullet();
	//	if( stage == 3 )
	//		make_boss_3_bullet();
	}
	return true;
}


gboolean player_move(gpointer data)
{

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
		
	return true;
}



gboolean shoot_bullet(gpointer data) {
    // static gint count = 0;

	

	int i;
	bool hit = false;
	for( i = 0; i < player_bullet_num; i++ )
	{
		BULLET *tmp = g_ptr_array_index( player_bullet , i);
		if((tmp->x >= boss_3.x && tmp->x <= boss_3.x + boss_width && tmp->y >= boss_3.y && tmp->y<= boss_3.y+boss_height))	{
			boss_3.life -= 20;
			player_bullet_num--;
			tmp = g_ptr_array_remove_index(player_bullet,i);
			free(tmp);
			i--;
			hit = true;
			//printf("gotcha1\n");
		}
		//printf("gotcha2\n");
		if(!hit){
			tmp->x += tmp->dx;
			tmp->y += tmp->dy;
			if( tmp->x < 0 || tmp->x > window_width
			||  tmp->y < 0 || tmp->y > window_height )
			{
			
				player_bullet_num--;
				tmp = g_ptr_array_remove_index(player_bullet,i);
				// printf("test free = %d\n", tmp->x);
				free(tmp);
				i--;
			}
		}
		hit = false;
	
	}
		
	gtk_widget_queue_draw(wid);
	
    return TRUE;


 //   else {
 //   	g_print ("timer expire\n");
 //     return FALSE;
 //   }
}


