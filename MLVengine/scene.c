#include "scene.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct maillon Maillon;

struct maillon {
	Object** elem;
	Maillon* next;
};

struct scene {
	Maillon* chaine;
};

Scene* initScene() {
	return (Scene*)malloc(sizeof(Scene));
}

Scene* newScene() {
	Scene* s = initScene();
	s->chaine=NULL;
	return s;
}

int addObject(Scene* s, Object* o) {
	Maillon* m;
	for (m=s->chaine; m!=NULL; m=m->next) {
		if (*(m->elem)==o) {
			return 0;
		}
	}
	Maillon* m2=(Maillon*)malloc(sizeof(Maillon));
	Object** obj=(Object**)malloc(sizeof(Object*));
	*obj=o;
	m2->next=s->chaine;
	m2->elem=obj;
	s->chaine=m2;
	return 1;
	
}

int removeObject(Scene* s, Object* o) {
	Maillon* m;
	Maillon* prec=NULL;
	for (m=s->chaine; m!=NULL; m=m->next) {
		if (*(m->elem)==o) {
			if (prec==NULL) {
				s->chaine=m->next;
			} else {
				prec->next=m->next;
			}
			free(m->elem);
			free(m);
			return 1;
		}
		prec=m;
	}
	return 0;
}

void eachObjects(Scene* s, void (*function)(Object**)) {
	Maillon* m;
	Maillon* prec = NULL;
	for (m=s->chaine; m!=NULL;) {
		if (*(m->elem)!=NULL) {
			(*function)(m->elem);
			prec=m;
			m=m->next;
		} else {
			Maillon* m2 = m->next;
			free(m->elem);
			free(m);
			if (prec==NULL) {
				s->chaine=m2;
			} else {
				prec->next=m2;
			}
			m=m2;
		}
	}
}

void applyVitPtr(Object** o) {
	applyVit(*o);
}

void iterate(Scene* s) {
	eachObjects(s, applyVitPtr);
	eachObjects(s, liveAndDie);
}
