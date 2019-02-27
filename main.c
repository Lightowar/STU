#include "MLVengine/vector.h"
#include "MLVengine/scene.h"
#include "MLVengine/object.h"
#include "MLVengine/render.h"
#include "carac.c"
#include <stdio.h>
#include <MLV/MLV_all.h>
#include <time.h>
#include <math.h>

#define MIN_X -300
#define MAX_X 300
#define MIN_Y -300
#define MAX_Y 300
#define SIZE_X 500
#define SIZE_Y 500



Object* createMissile (Scene* s, float x, float y, float vitX, float vitY) {
	Object* m=newObject();
	setX(getVit(m), vitX);
	setY(getVit(m), vitY-10);
	setX(getPos(m), x);
	setY(getPos(m), y-20);
	setKillTime(m, 100);
	setDrawType(m, DRAW_IMAGE);
	setDrawString(m, "DATA/missile.png");
	Vector* hit=getHitbox(m);
	setHitboxType(m, HITBOX_RECT);
	setX(hit, 10);
	setY(hit, 15);
	addObject(s, m, 1);
	return m;
}

void randomiseStar(Object* o) {
	Vector* scale=getDrawScale(o);
	Vector* pos=getPos(o);
	Vector* vit=getVit(o);
	int rand=MLV_get_random_integer(50, 100);
	if (rand<=1) {
		setY(pos, MLV_get_random_double(MIN_Y, MAX_Y));
		setX(pos, MIN_X);
		setX(vit, MLV_get_random_double(6, 20));
		setY(vit, MLV_get_random_double(0.2, 3));
		setX(scale, MLV_get_random_double(1, 3));
		setY(scale, MLV_get_random_double(1, 3));
		setDrawType(o, DRAW_IMAGE);
		setDrawString(o, "DATA/cat.png");
	} else {
		setX(pos, MLV_get_random_double(MIN_X, MAX_X));
		setY(pos, MIN_Y);
		setX(vit, 0);
		setY(vit, MLV_get_random_double(0.2, 3));
		setX(scale, MLV_get_random_double(1, 3));
		setY(scale, MLV_get_random_double(1, 3));
		setDrawType(o, DRAW_OVAL);
		setDrawString(o, "starColor");
		if (rand<=5) {
			setDrawType(o, DRAW_IMAGE);
			setDrawString(o, "DATA/star1.png");
		}
	}
}

void moveStar(Object* o) {
	Vector* v=getPos(o);
	float y=getY(v);
	float x=getX(v);
	if (y>MAX_Y || y<MIN_Y || x>MAX_X || x<MIN_X) {
		randomiseStar(o);
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
	Object* o;
	Object* fr=newObject();
	setDrawType(fr, DRAW_TEXT);
	char strfps[10]={'0', 0};
	setDrawString(fr, strfps);
	addObject(s, fr, 3);
	struct timespec last;
	struct timespec new;
	clock_gettime(CLOCK_REALTIME, &last);
	clock_gettime(CLOCK_REALTIME, &new);
	float accum;
	float smoothing=0.9;
	float fps=60;
	MLV_change_frame_rate(fps);
	
	int time=0;
	int totalFrame=0;
	
	Set* stars=newSet();
	int i;
	for(i=0; i<60; i++) {
		o = newObject();
		Vector* pos=getPos(o);
		randomiseStar(o);
		setY(pos, MLV_get_random_double(MIN_Y, MAX_Y));
		addObject(s, o, 0);
		addToSet(stars, o);
	}
	
	int cmpEqu(Object* o1, Object* o2) {
		return (getObject(o1)==getObject(o2));
	}
	Set* allyM;
	allyM=newSet();
	setEqual(allyM, (int (*)(void *, void *))cmpEqu);
	Set* enemy;
	enemy=newSet();
	setEqual(enemy, (int (*)(void *, void *))cmpEqu);
	
	Vector* vit;
	Vector* pos;
	Vector* hit;
	
	o = newObject();
	vit=getVit(o);
	pos=getPos(o);
	hit=getHitbox(o);
	setDrawType(o, DRAW_IMAGE);
	setDrawString(o, "DATA/spaceship.png");
	setHitboxType(o, HITBOX_RECT);
	setX(hit, 30);
	setY(hit, 40);
	addObject(s, o, 1);
	addToSet(enemy, o);
	
	o = newObject();
	vit=getVit(o);
	pos=getPos(o);
	hit=getHitbox(o);
	setDrawType(o, DRAW_IMAGE);
	setDrawString(o, "DATA/spaceship.png");
	setHitboxType(o, HITBOX_RECT);
	setX(pos, 100);
	setX(hit, 30);
	setY(hit, 40);
	addObject(s, o, 1);
	addToSet(enemy, o);
	
	float max=5;
	float acc=0.2;
	
	Object* player = newObject();
	vit=getVit(player);
	pos=getPos(player);
	hit=getHitbox(player);
	setDrawType(player, DRAW_IMAGE);
	setDrawString(player, "DATA/spaceship.png");
	setHitboxType(player, HITBOX_RECT);
	setX(hit, 30);
	setY(hit, 40);
	addObject(s, player, 1);
	
	Object* cam=newObject();
	setCamera(s, cam);

	void vitEnemy(void* o) {
		updateVit((Object*)(o), totalFrame);
	}
	
	while (MLV_get_keyboard_state(MLV_KEYBOARD_q)!=MLV_PRESSED) {
		
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
			addToSet(allyM, createMissile(s, getX(pos), getY(pos), getX(vit), getY(vit)));
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
		eachSet(stars, (void (*)(void *))moveStar);
		eachSet(enemy, (void (*)(void *))vitEnemy);
		
		void colE(void* m) {
			void col(void* e) {
				if (getObject(e)==NULL || getObject(m)==NULL) return;
				if (touch((m), (e))) {
					destroyObject((Object*)m, 1);
					destroyObject((Object*)e, 1);
				}
			}
			eachSet(enemy, col);
		}
		eachSet(allyM, colE);
		
		renderScene(s);
		
		totalFrame++;
		
		last=new;
		clock_gettime(CLOCK_REALTIME, &new) ;
		accum=(new.tv_sec-last.tv_sec)+((new.tv_nsec-last.tv_nsec)/1000000000.0);
		fps=(1/accum)*(1-smoothing)+fps*smoothing;
		sprintf(strfps, "%.1f", fps);
		MLV_delay_according_to_frame_rate();
	}
	destroySet(enemy);
	destroySet(allyM);
	exit(0);
}
