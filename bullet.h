#ifndef BULLET_H
#define BULLET_H


typedef struct BULLET BULLET;

struct BULLET
{ 
	int x;
	int y;
	int dx;
	int dy;
	int mode;
	bool exist;
};


#endif
