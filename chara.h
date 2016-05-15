#ifndef CHARA_H
#define CHARA_H

typedef struct CHARA CHARA;

struct CHARA
{ 
	int x;
	int y;
	int width;
	int height;
	int speed;
	int type;	
	// 0 == player // 1 == boss
	int life;
};

void initCHARA( CHARA *chara, int x, int y, int width, int height, int speed, int type, int life)
{
	chara->x = x;
	chara->y = y;
	chara->width = width;
	chara->height = height;
	chara->speed = speed;
	chara->type = type;
	chara->life = life;
}
#endif

