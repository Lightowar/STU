#include "MLVengine/vector.h"
#include "MLVengine/scene.h"
#include "MLVengine/object.h"
#include "MLVengine/render.h"
#include "MLVengine/input.h"
#include "MLVengine/other.h"

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#define UI_SIZE 300
#define SIZE_X 1280
#define SIZE_Y 720
#define MAX_X ((SIZE_X-UI_SIZE)/2+150)
#define MIN_X -MAX_X
#define MAX_Y (SIZE_Y/2+150)
#define MIN_Y -MAX_Y

#define FPS 60

#define TEXT_SIZE 65536

#define SIZE_MENU_1 2

Set* enemy;
Set* enemyM;
Set* allyM;
Object* player;
Scene* s;

int mouse=0;

Object* createMissile (Object* o, Vector vit, Vector dec, int id, int global);
#include "carac.c"
#include "ship_creator.c"	
#include "create.c"
#include "level.c"
#include "other.c"

int main (int argc, char* argv[]) {
	
	char *dirsep = strrchr( argv[0], '/' );
	if( dirsep != NULL ) dirsep = ".";
	
	if (chdir(dirsep)==-1) {
		fprintf(stdout, "error\n");
	}
	
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
	changeFrameRate(fps);
	
	int totalFrame=0;
	
	readFile("DATA/ss_descriptor.data");
	openLevel("DATA/level.data");
	
	Set* stars=newSet();
	int i;
	for(i=0; i<60; i++) {
		o = newObject();
		Vector* pos=getPos(o);
		randomiseStar(o);
		setY(pos, randomDouble(MIN_Y, MAX_Y));
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
	
	Object* cam=newObject();
	setPos(cam, newVector(UI_SIZE/2, 0));
	setCamera(s, cam);
	
	Object* UI1=newObject();
	addObject(s, UI1, 3);
	setDrawType(UI1, DRAW_RECT);
	setDrawScale(UI1, newVector(UI_SIZE, SIZE_Y));
	setPos(UI1, newVector(SIZE_X-UI_SIZE, 0));
	setColor(UI1, (ObjColor){80, 86, 99, 255});
	
	Object* UI2=newObject();
	addObject(s, UI2, 3);
	setDrawType(UI2, DRAW_RECT);
	setDrawScale(UI2, newVector(UI_SIZE-10, SIZE_Y-10));
	setPos(UI2, newVector(SIZE_X-UI_SIZE+5, 0+5));
	setColor(UI2, (ObjColor){53, 58, 68, 255});
	
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
	setDrawScale(textBox, newVector(0, 30));
	setPos(textBox, newVector(SIZE_X-UI_SIZE+10, SIZE_Y/2));
	setTextBox(textBox, newVector(UI_SIZE-20 ,SIZE_Y));
	addObject(s, textBox, 3);
	setDrawText(textBox, partialText);
	setColor(textBox, (ObjColor){152, 242, 215, 255});
	
	Object* face=newObject();
	setDrawType(face, DRAW_IMAGE);
	char faceString[255]="";
	setDrawString(face, faceString);
	setPos(face, newVector(SIZE_X-(UI_SIZE/2), SIZE_Y/2-110));
	addObject(s, face, 3);
	
	Object* menu1[SIZE_MENU_1*3];
	
	Object* restartBox1=newObject();
	setDrawType(restartBox1, DRAW_RECT);
	setDrawScale(restartBox1, newVector(310, 70));
	setPos(restartBox1, newVector((SIZE_X-UI_SIZE-300)/2-5, (SIZE_Y-260)/2-5));
	addObject(s, restartBox1, 3);
	setColor(restartBox1, (ObjColor){80, 86, 99, 255});
	menu1[0]=restartBox1;
	
	Object* restartBox2=newObject();
	setDrawType(restartBox2, DRAW_RECT);
	setDrawScale(restartBox2, newVector(300, 60));
	setPos(restartBox2, newVector((SIZE_X-UI_SIZE-300)/2, (SIZE_Y-260)/2));
	addObject(s, restartBox2, 3);
	setColor(restartBox2, (ObjColor){53, 58, 68, 255});
	menu1[1]=restartBox2;
	
	Object* restart=newObject();
	setDrawType(restart, DRAW_TEXT);
	setDrawString(restart, "DATA/conthrax-sb.ttf");
	setDrawScale(restart, newVector(0, 50));
	setPos(restart, newVector((SIZE_X-UI_SIZE-300)/2+10, (SIZE_Y-260)/2));
	addObject(s, restart, 3);
	setDrawText(restart, "restart");
	setColor(restart, (ObjColor){152, 242, 215, 255});
	menu1[2]=restart;
	
	Object* quitBox1=newObject();
	setDrawType(quitBox1, DRAW_RECT);
	setDrawScale(quitBox1, newVector(310, 70));
	setPos(quitBox1, newVector((SIZE_X-UI_SIZE-300)/2-5, (SIZE_Y-60)/2-5));
	addObject(s, quitBox1, 3);
	setColor(quitBox1, (ObjColor){80, 86, 99, 255});
	menu1[3]=quitBox1;
	
	Object* quitBox2=newObject();
	setDrawType(quitBox2, DRAW_RECT);
	setDrawScale(quitBox2, newVector(300, 60));
	setPos(quitBox2, newVector((SIZE_X-UI_SIZE-300)/2, (SIZE_Y-60)/2));
	addObject(s, quitBox2, 3);
	setColor(quitBox2, (ObjColor){53, 58, 68, 255});
	menu1[4]=quitBox2;
	
	Object* quit=newObject();
	setDrawType(quit, DRAW_TEXT);
	setDrawString(quit, "DATA/conthrax-sb.ttf");
	setDrawScale(quit, newVector(0, 50));
	setPos(quit, newVector((SIZE_X-UI_SIZE-300)/2+10, (SIZE_Y-60)/2));
	addObject(s, quit, 3);
	setDrawText(quit, "quit");
	setColor(quit, (ObjColor){152, 242, 215, 255});
	menu1[5]=quit;
	
	int selected1 = -1;
	
	void menuOn(Object* menu[], int n) {
		int i;
		for (i=0; i<n; i++) {
			setDrawType(menu[3*i], DRAW_RECT);
			setDrawType(menu[3*i+1], DRAW_RECT);
			setDrawType(menu[3*i+2], DRAW_TEXT);
		}
	}
	
	void menuOff(Object* menu[], int n) {
		int i;
		for (i=0; i<n; i++) {
			setDrawType(menu[3*i], DRAW_NONE);
			setDrawType(menu[3*i+1], DRAW_NONE);
			setDrawType(menu[3*i+2], DRAW_NONE);
		}
	}
	
	menuOff(menu1, 2);
	
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
	
	void initPlayer () {
		player = createSpaceShipe(0);
		vit=getVit(player);
		pos=getPos(player);
		setY(pos, 300);
		*vit=(Vector){0, 0};
		addObject(s, player, 2);
		((Carac*)getCarac(player))->fireTime=0;
		majBar();
	}
	initPlayer();

	void vitEnemy(void* o) {
		moveObj((Object*)(o));
	}
	
	int type;
	void* object;
	int wait=0;
	int prec;
	Object* objToWait=NULL;
	int textToWait=0;
	
	int paused=0;
	int pausedPressed=0;
	
	int dead() {
		return getObject(player)==NULL;
	}
	
	while (!(selected1==1 && isPressed("RETURN"))) {
		if (isPressed("ESCAPE") && pausedPressed==0 && !dead()) {
			paused=!paused;
			pausedPressed=3;
		} else {
			if (pausedPressed>0 && !isPressed("ESCAPE")) pausedPressed--;
		}
		if (paused) {
		} else {
			while (wait==0 && textToWait==0 && getObject(objToWait)==NULL) {
				if (readLine(&type, &object, &wait, &prec)<=0) break;
				Object* o;
				switch (type) {
					case 1:
						o = (Object*)object;
						addObject(s, o, 1);
						addToSet(enemy, o);
						((Carac*)getCarac(o))->init=*getPos(o);
						((Carac*)getCarac(o))->target=player;
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
				setDrawString(face, "");
			}
			if (!isPressed("RETURN")) {
				enterPressed=0;
			} else {
				if (textBoxAdvance<(sizeText*2) && !enterPressed) {
					enterPressed=1;
					if (textBoxAdvance<sizeText) {
						textBoxAdvance=sizeText;
						strcpy(partialText, text);
					} else {
						textBoxAdvance = 2*sizeText;
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
						destroySpaceShip(player);
						player=NULL;
					}
					majBar();
				}
			}
			eachSet(enemyM, colEM);
			
			void colP(void** e) {
				if (getObject(*e)==NULL || getObject(player)==NULL) return;
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
		}
		
		totalFrame++;
		last=new;
		clock_gettime(CLOCK_REALTIME, &new) ;
		accum=(new.tv_sec-last.tv_sec)+((new.tv_nsec-last.tv_nsec)/1000000000.0);
		fps=(1/accum)*(1-smoothing)+fps*smoothing;
		if (totalFrame%10==0) sprintf(strfps, "%.1f", fps);
		
		if (paused || dead()) {
			menuOn(menu1, SIZE_MENU_1);
			for (i=0; i<SIZE_MENU_1; i++) {
				setColor(menu1[3*i], (ObjColor){80, 86, 99, 255});
			}
			if (isPressed("DOWN")) {
				if (selected1==-1) {
					selected1=SIZE_MENU_1-1;
				} else {
					if (selected1<SIZE_MENU_1-1)selected1++;
				}
			}
			if (isPressed("UP")) {
				if (selected1==-1) {
					selected1=0;
				} else {
					if (selected1>0)selected1--;
				}
			}
			if (selected1!=-1) setColor(menu1[selected1*3], (ObjColor){152, 242, 215, 255});
		} else {
			menuOff(menu1, 2);
			selected1=-1;
			setColor(quitBox1, (ObjColor){80, 86, 99, 255});
		}
	
		void destroyObj1(void** o) {
			destroyObject((Object*)*o, 1);
		}
		
		if (selected1==0 && isPressed("RETURN")) {
			resetLevel();
			eachSet(enemy, destroyObj1);
			eachSet(enemyM, destroyObj1);
			eachSet(allyM, destroyObj1);
			emptySet(enemy);
			emptySet(enemyM);
			emptySet(allyM);
			destroyObject(player, 1);
			initPlayer();
			majBar();
			paused=0;
			wait=0;
			objToWait=NULL;
			textToWait=0;
		}
		
		renderScene(s);
		
		waitEndFrame();
	}
	
	void destroyObj2(Object* o) {
		destroyObject(o, 1);
	}
	
	closeLevel();
	renderEnd();
	eachObjectScene(s, destroyObj2);
	destroyScene(s);
	destroySet(enemy);
	destroySet(enemyM);
	destroySet(allyM);
	destroySet(stars);
	
	exit(0);
}
