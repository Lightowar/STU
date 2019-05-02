#include "MLVengine/vector.h"
#include "MLVengine/scene.h"
#include "MLVengine/object.h"
#include "MLVengine/render.h"

#include <stdio.h>
#include <MLV/MLV_all.h>
#include <time.h>
#include <math.h>

#define UI_SIZE 300
#define SIZE_X 1280
#define SIZE_Y 720
#define MAX_X ((SIZE_X-UI_SIZE)/2+100)
#define MIN_X -MAX_X
#define MAX_Y (SIZE_Y/2+100)
#define MIN_Y -MAX_Y

#define FPS 60

#define TEXT_SIZE 65536

Set* enemy;
Set* enemyM;
Set* allyM;
Object* player;
Scene* s;

Object* createMissile (Object* o, Vector vit, Vector dec);
#include "carac.c"
#include "ship_creator.c"	
#include "create.c"
#include "level.c"
#include "other.c"

int main (int argc, char* argv[]) {
	
	s = newScene(SIZE_X, SIZE_Y);
	renderInit(s);
	Object* o;
	Object* fr=newObject();
	setDrawType(fr, DRAW_TEXT);
	char strfps[10]={'0', 0};
	setDrawString(fr, "");
	ObjColor red = {255, 0, 0, 255};
	setColor(fr, red);
	setDrawText(fr, strfps);
	setDrawScale(fr, newVector(30, 30));
	addObject(s, fr, 3);
	struct timespec last;
	struct timespec new;
	clock_gettime(CLOCK_REALTIME, &last);
	clock_gettime(CLOCK_REALTIME, &new);
	float accum;
	float smoothing=0.9;
	float fps=FPS;
	MLV_change_frame_rate(fps);
	
	int totalFrame=0;
	
	readFile("DATA/ss_descriptor.data");
	openLevel("DATA/level.data");
	
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
	enemyM=newSet();
	setEqual(enemyM, (int (*)(void *, void *))cmpEqu);
	allyM=newSet();
	setEqual(allyM, (int (*)(void *, void *))cmpEqu);
	enemy=newSet();
	setEqual(enemy, (int (*)(void *, void *))cmpEqu);
	
	Vector* vit;
	Vector* pos;
	
	player = createSpaceShipe(0);
	vit=getVit(player);
	pos=getPos(player);
	setY(pos, 300);
	*vit=(Vector){0, 0};
	addObject(s, player, 2);
	((Carac*)getCarac(player))->fireTime=0;
	
	Object* cam=newObject();
	setPos(cam, newVector(UI_SIZE/2, 0));
	setCamera(s, cam);
	
	Object* UI=newObject();
	addObject(s, UI, 3);
	setDrawType(UI, DRAW_RECT);
	setDrawScale(UI, newVector(UI_SIZE, SIZE_Y));
	setPos(UI, newVector(SIZE_X-UI_SIZE, 0));
	setColor(UI, (ObjColor){50, 50, 50, 255});
	
	Object* bar1=newObject();
	setDrawType(bar1, DRAW_RECT);	
	setDrawScale(bar1, newVector(30*8, 30));
	setPos(bar1, newVector(30+(SIZE_X-UI_SIZE), 30));
	addObject(s, bar1, 3);
	setColor(bar1, (ObjColor){127, 0, 0, 127});
	
	Object* bar2=newObject();
	setDrawType(bar2, DRAW_RECT);
	setDrawScale(bar2, newVector(30*8-8, 30-8));
	setPos(bar2, newVector(30+4+(SIZE_X-UI_SIZE), 30+4));
	addObject(s, bar2, 3);
	setColor(bar2, (ObjColor){0, 191, 0, 192});
	
	int sizeText=0;
	char text[TEXT_SIZE] = "";
	char partialText[TEXT_SIZE] = "";
	int textBoxAdvance=0;
	int enterPressed=0;

	Object* textBox=newObject();
	setDrawType(textBox, DRAW_TEXT);
	setDrawString(textBox, "DATA/BebasNeue-Regular.ttf");
	setDrawScale(textBox, newVector(20, 20));
	setPos(textBox, newVector(SIZE_X-UI_SIZE, SIZE_Y/2));
	setTextBox(textBox, newVector(UI_SIZE ,SIZE_Y));
	addObject(s, textBox, 3);
	setDrawText(textBox, partialText);
	setColor(textBox, (ObjColor){255, 0, 0, 255});
	
	Object* face=newObject();
	setDrawType(face, DRAW_IMAGE);
	char faceString[255]="";
	setDrawString(face, faceString);
	setPos(face, newVector(SIZE_X-(UI_SIZE/2), SIZE_Y/2-110));
	addObject(s, face, 3);
	
	
	void vitEnemy(void* o) {
		moveObj((Object*)(o));
	}
	
	void majBar() {
		float percent;
		if (player==NULL) {
			percent=0;
		} else {
			Carac* c = (Carac*)getCarac(player);
			percent = (float)c->lifePoint/(float)c->maxLifePoint;
		}
		setDrawScale(bar2, newVector((30.0*8.0-8.0)*percent, 30-8));
	}
	
	int type;
	void* object;
	int wait=0;
	int prec;
	Object* objToWait=NULL;
	int textToWait=0;
	
	while (MLV_get_keyboard_state(MLV_KEYBOARD_q)!=MLV_PRESSED) {
		while (wait==0 && textToWait==0 && getObject(objToWait)==NULL) {
			if (readLine(&type, &object, &wait, &prec)<=0) break;
			Object* o;
			switch (type) {
				case 1:
					o = (Object*)object;
					addObject(s, o, 1);
					addToSet(enemy, o);
					((Carac*)getCarac(o))->init=*getPos(o);
					if (prec==1) objToWait=o;
					else objToWait=NULL;
					break;
				case 2:
					strcpy(text, object);
					textBoxAdvance=0;
					sizeText=strlen(text);
					if (prec==1) textToWait=1;
					else textToWait=0;
					break;
				case 3:
					strcpy(faceString, object);
					setDrawString(face, faceString);
					break;
				default:
					break;
			}
		}
		if (wait > 0) wait--;
		
		if (textBoxAdvance<sizeText) {
			partialText[textBoxAdvance]=text[textBoxAdvance];
			partialText[textBoxAdvance+1]='\0';
		}
		
		if (textBoxAdvance<(sizeText*2)) {
			textBoxAdvance++;
		} else {
			partialText[0]='\0';
			textBoxAdvance=0;
			sizeText=0;
			textToWait=0;
		}
		
		if (MLV_get_keyboard_state(MLV_KEYBOARD_RETURN)==MLV_RELEASED) {
			enterPressed=0;
		} else {		
			if (MLV_get_keyboard_state(MLV_KEYBOARD_RETURN)==MLV_PRESSED && textBoxAdvance<(sizeText*2) && !enterPressed) {
				enterPressed=1;
				if (textBoxAdvance<sizeText) {
					textBoxAdvance=sizeText;
					strcpy(partialText, text);
				} else {
					textBoxAdvance=2*sizeText;
				}
			}
		}
		
		void l1 (void** o) {moveStar((Object*)(*o));}
		void l2 (void** o) {vitEnemy((Object*)(*o));}
		void l3 (void** o) {fireObj((Object*)(*o));}
		eachSet(stars, l1);
		eachSet(enemy, l2);
		eachSet(enemy, l3);
		if (getObject(player)!=NULL) {
			moveObj(player);
			fireObj(player);
		}
		
		void boundSet(Set* s) {
			void bound(void** obj) {
				if (getObject(*obj)==NULL) return;
				Object* o = (Object*)(*obj);
				Vector pos = *getPos(o);
				if (getXInt(pos)>MAX_X || getXInt(pos)<MIN_X || getYInt(pos)>MAX_Y || getYInt(pos)<MIN_Y) {
					destroyObject(o, 1);
					almostRemoveFromSet(s, o);
				}
			}
			eachSet(s, bound);
		}
		
		boundSet(enemy);
		boundSet(enemyM);
		boundSet(allyM);
		
		void colE(void** m) {
			void col(void** e) {
				if (getObject(*e)==NULL || getObject(*m)==NULL) return;
				if (touch((*m), (*e))) {
					if (dammage(*m, *e)) {
						destroySpaceShip((Object*)(*e));
						almostRemoveFromSet(enemy, (Object*)(*e));
					}
				}
			}
			eachSet(enemy, col);
		}
		eachSet(allyM, colE);
		
		void colEM(void** m) {
			if (getObject(player) == NULL || getObject(*m)==NULL) return;
			if (touch((*m), player)) {
				if (dammage(*m, player)) {
					player=NULL;
					destroySpaceShip(player);
				}
				majBar();
			}
		}
		eachSet(enemyM, colEM);
		
		void colP(void** e) {
			if (collision((Object*)(*e), player)) {
				int d = dammage((Object*)(*e), player);
				if (dammage(player, (Object*)(*e))) {
					destroySpaceShip((Object*)(*e));
					almostRemoveFromSet(enemy, (Object*)(*e));
				}
				if (d) {
					destroySpaceShip(player);
					player=NULL;
				}
			}
			majBar();
		}
		eachSet(enemy, colP);
		
		iterate(s);
		renderScene(s, 1.0/(double)FPS);
		
		totalFrame++;
		
		last=new;
		clock_gettime(CLOCK_REALTIME, &new) ;
		accum=(new.tv_sec-last.tv_sec)+((new.tv_nsec-last.tv_nsec)/1000000000.0);
		fps=(1/accum)*(1-smoothing)+fps*smoothing;
		if (totalFrame%10==0) sprintf(strfps, "%.1f", fps);
		MLV_delay_according_to_frame_rate();
	}
	
	void destroyObj(void** o) {
		destroyObject(*o, 1);
	}
	closeLevel();
	renderEnd();
	eachSet(enemy, destroyObj);
	destroySet(enemy);
	destroySet(allyM);
	destroyObject(fr, 1);
	MLV_free_window();
	exit(0);
}
