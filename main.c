#include "MLVengine/vector.h"
#include "MLVengine/scene.h"
#include "MLVengine/object.h"
#include "MLVengine/render.h"

#include <stdio.h>
#include <MLV/MLV_all.h>
#include <time.h>
#include <math.h>

#define SIZE_X 1280
#define SIZE_Y 720
#define MIN_X -650
#define MAX_X 650
#define MIN_Y -370
#define MAX_Y 370

#define FPS 60

#include "carac.c"
#include "other.c"
#include "create.c"
#include "ship_creator.c"

int main (int argc, char* argv[]) {
	
	Scene* s = newScene(SIZE_X, SIZE_Y);
	renderInit(s);
	Object* o;
	Object* fr=newObject();
	Object* fr2=newObject();
	setDrawType(fr, DRAW_TEXT);
	setDrawType(fr2, DRAW_TEXT);
	char strfps[10]={'0', 0};
	setDrawString(fr, "DATA/BebasNeue-Regular.ttf");
	setDrawString(fr2, "DATA/BebasNeue-Regular.ttf");
	setDrawText(fr, strfps);
	setDrawText(fr2, strfps);
	setDrawScale(fr, newVector(30, 30));
	setDrawScale(fr2, newVector(10, 10));
	addObject(s, fr, 3);
	addObject(s, fr2, 3);
	struct timespec last;
	struct timespec new;
	clock_gettime(CLOCK_REALTIME, &last);
	clock_gettime(CLOCK_REALTIME, &new);
	float accum;
	float smoothing=0.9;
	float fps=FPS;
	MLV_change_frame_rate(fps);
	
	int time=0;
	int totalFrame=0;
	
	readFile("DATA/ss_descriptor.data");
	
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
	
	o=createSpaceShipe(1);
	((Carac*)getCarac(o))->move=1;
	addObject(s, o, 1);
	addToSet(enemy, o);
	
    
	o=createSpaceShipe(1);
	((Carac*)getCarac(o))->move=2;
	addObject(s, o, 2);
	addToSet(enemy, o);
	
	Object* player = o=createSpaceShipe(0);
	((Carac*)getCarac(player))->move=3;
	vit=getVit(player);
	pos=getPos(player);
	addObject(s, player, 1);
	
	Object* cam=newObject();
	setCamera(s, cam);

	void vitEnemy(void* o) {
		moveObj((Object*)(o));
	}
	
	while (MLV_get_keyboard_state(MLV_KEYBOARD_q)!=MLV_PRESSED) {		

		if (MLV_get_keyboard_state(MLV_KEYBOARD_a)==MLV_PRESSED && time<=0) {
			addToSet(allyM, createMissile(s, getX(*pos)-5, getY(*pos), getX(*vit), getY(*vit)));
			addToSet(allyM, createMissile(s, getX(*pos)+5, getY(*pos), getX(*vit), getY(*vit)));
			time=FPS/6;
		}
		if (time>0) {
			time--;
		}
		void l1 (void** o) {moveStar((Object*)(*o));}
		void l2 (void** o) {vitEnemy((Object*)(*o));}
		eachSet(stars, l1);
		eachSet(enemy, l2);
		moveObj(player);
		iterate(s);
		
		void colE(void** m) {
			void col(void** e) {
				if (getObject(*e)==NULL || getObject(*m)==NULL) return;
				if (touch((*m), (*e))) {
                    setDrawString((Object*)(*e), "DATA/explosion");
					destroyObject((Object*)(*m), 1);
                    setKillTime((Object*)(*e), 80);
                    removeFromSet(enemy, (Object*)(*e));
                    
				}
			}
		
			eachSet(enemy, col);
		}
		eachSet(allyM, colE);
		
		renderScene(s, 1.0/(double)FPS);
		
		totalFrame++;
		
		last=new;
		clock_gettime(CLOCK_REALTIME, &new) ;
		accum=(new.tv_sec-last.tv_sec)+((new.tv_nsec-last.tv_nsec)/1000000000.0);
		fps=(1/accum)*(1-smoothing)+fps*smoothing;
		sprintf(strfps, "%.1f", fps);
		MLV_delay_according_to_frame_rate();
	}
	
	void destroyObj(void** o) {
		destroyObject(*o, 1);
	}
	renderEnd();
	eachSet(enemy, destroyObj);
	destroySet(enemy);
	destroySet(allyM);
	destroyObject(fr, 1);
	MLV_free_window();
	exit(0);
}
