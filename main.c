#include "MLVengine/vector.h"
#include "MLVengine/scene.h"
#include "MLVengine/object.h"
#include "MLVengine/render.h"

#include <stdio.h>
#include <MLV/MLV_all.h>
#include <time.h>

Object* createMissile (Scene* s, int x, int y, Object* o) {
	Object* m=newObject();
	setY(getVit(m), -5);
	setX(getPos(m), x);
	setY(getPos(m), y);
	setKillTime(m, 100);
	addObject(s, m);
	return m;
}

int main (int argc, char* argv[]) {
	
	renderInit();
	
	Scene* s = newScene();
	Object* o = newObject();
	Vector* vit=getVit(o);
	Vector* pos=getPos(o);
	addObject(s, o);
	getX(pos);
	
	float max=3;
	float acc=0.1;
	
	struct timespec last;
	struct timespec new;
	float accum;
	
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
		if (MLV_get_keyboard_state(MLV_KEYBOARD_a)==MLV_PRESSED) {
			createMissile(s, getX(pos), getY(pos), o);
			
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
		renderScene(s);
		
		clock_gettime(CLOCK_REALTIME, &new) ;
		accum=(new.tv_sec-last.tv_sec)+((new.tv_nsec-last.tv_nsec)/1000000000);
		if(accum<(1.0/48.0)) {
			MLV_wait_milliseconds((int)(((1.0/48.0)-accum)*1000));
		}
	}
}
