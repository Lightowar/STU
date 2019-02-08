#include "MLVengine/vector.h"
#include "MLVengine/scene.h"
#include "MLVengine/object.h"
#include "MLVengine/render.h"
#include <stdio.h>
#include <MLV/MLV_all.h>
#include <time.h>

#define MIN_X -300
#define MAX_X 300
#define MIN_Y -300
#define MAX_Y 300
#define SIZE_X 500
#define SIZE_Y 500

Object* createMissile (Scene* s, float x, float y, float vitX, float vitY, Object* o) {
	Object* m=newObject();
	setX(getVit(m), vitX);
	setY(getVit(m), vitY-10);
	setX(getPos(m), x);
	setY(getPos(m), y-20);
	setKillTime(m, 100);
	setDrawType(m, DRAW_IMAGE);
	setDrawString(m, "DATA/missile.png");
	addObject(s, m, 1);
	return m;
}

void randomiseStar(Object* o) {
	Vector* scale=getDrawScale(o);
	Vector* pos=getPos(o);
	Vector* vit=getVit(o);
	int rand=MLV_get_random_integer(1, 100);
	if (rand<=4) {
		setY(pos, MLV_get_random_double(MIN_Y, MAX_Y));
		setX(pos, MIN_X);
		setX(vit, MLV_get_random_double(6, 20));
		setY(vit, MLV_get_random_double(0.2, 3));
		setX(scale, MLV_get_random_double(1, 3));
		setY(scale, MLV_get_random_double(1, 3));
		setDrawType(o, DRAW_IMAGE);
		setDrawString(o, "DATA/cat.png");
	} else {;
		setX(pos, MLV_get_random_double(MIN_X, MAX_X));
		setY(pos, MIN_Y);
		setX(vit, 0);
		setY(vit, MLV_get_random_double(0.2, 3));
		setX(scale, MLV_get_random_double(1, 3));
		setY(scale, MLV_get_random_double(1, 3));
		setDrawType(o, DRAW_OVAL);
		setDrawString(o, "starColor");
		if (rand<=8) {
			setDrawType(o, DRAW_IMAGE);
			setDrawString(o, "DATA/star1.png");
		}
	}
}
void moveStar(Object** o) {
	Vector* v=getPos(*o);
	float y=getY(v);
	float x=getX(v);
	if (y>MAX_Y || y<MIN_Y || x>MAX_X || x<MIN_X) {
		randomiseStar(*o);
	}
}
float min(float a, float b) {
	if (a<b) return a;
	return b;
}
float max(float a, float b) {
	if (a>b) return a;
	return b;
}
float between(float a, float min, float max) {
	if (a<min) return min;
	if (a>max) return max;
	return a;
}

int main (int argc, char* argv[]) {
	Scene* s = newScene(SIZE_X, SIZE_Y);
	renderInit(s);
	
	Set* stars=newSet();
	
	Object* fr=newObject();
	setDrawType(fr, DRAW_TEXT);
	char strfps[10]={'0', 0};
	setDrawString(fr, strfps);
	addObject(s, fr, 3);
	
	Object* o;
	
	int i;
	for(i=0; i<60; i++) {
		o = newObject();
		Vector* pos=getPos(o);
		randomiseStar(o);
		setY(pos, MLV_get_random_double(MIN_Y, MAX_Y));
		addObject(s, o, 0);
		addToSet(stars, o);
	}
	
	o = newObject();
	Vector* vit=getVit(o);
	Vector* pos=getPos(o);
	Vector* hit=getHitbox(o);
	setDrawType(o, DRAW_IMAGE);
	setDrawString(o, "DATA/spaceship.png");
	setHitboxType(o, HITBOX_RECT);
	setX(hit, 30);
	setY(hit, 40);
	addObject(s, o, 1);
	
	float max=5;
	float acc=0.2;
	
	struct timespec last;
	struct timespec new;
	float accum;
	
	float smoothing=0.7;
	float fps=1;
	
	int time=0;
	
	Object* cam=newObject();
	setCamera(s, cam);
	
	while (1) {
	
		clock_gettime(CLOCK_REALTIME, &last);	
		
		float x=getX(vit);
		float y=getY(vit);
		float targetX=0;
		float targetY=0;
		if (MLV_get_keyboard_state(MLV_KEYBOARD_LEFT)==MLV_PRESSED) targetX-=max;
		if (MLV_get_keyboard_state(MLV_KEYBOARD_DOWN)==MLV_PRESSED) targetY+=max;
		if (MLV_get_keyboard_state(MLV_KEYBOARD_UP)==MLV_PRESSED) targetY-=max;
		if (MLV_get_keyboard_state(MLV_KEYBOARD_RIGHT)==MLV_PRESSED) targetX+=max;
		if (targetX!=0 && targetY!=0) {
			targetX*=0.707107;
			targetY*=0.707107;
		}
		if (MLV_get_keyboard_state(MLV_KEYBOARD_a)==MLV_PRESSED && time<=0) {
			createMissile(s, getX(pos), getY(pos), getX(vit), getY(vit), o);
			time=10;
		}
		if (time>0) {
			time--;
		}
		float accX=targetX-x;
		float accY=targetY-y;
		if (accX>acc)accX=acc;
		if (accX<-acc)accX=-acc;
		if (accY>acc)accY=acc;
		if (accY<-acc)accY=-acc;
		x+=accX;
		y+=accY;
		setX(vit, x);
		setY(vit, y);
		iterate(s);
		eachSet(stars, (void (*)(void **))moveStar);
		
		renderScene(s);
		
		clock_gettime(CLOCK_REALTIME, &new) ;
		accum=(new.tv_sec-last.tv_sec)+((new.tv_nsec-last.tv_nsec)/1000000000.0);
		if(accum<(1.0/58.5)) {
			MLV_wait_milliseconds((int)(((1.0/58.5)-accum)*1000));
		}
		clock_gettime(CLOCK_REALTIME, &new) ;
		accum=(new.tv_sec-last.tv_sec)+((new.tv_nsec-last.tv_nsec)/1000000000.0);
		fps=(1/accum)*(1-smoothing)+fps*smoothing;
		sprintf(strfps, "%.1f", fps);
	}
}
