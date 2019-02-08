#include "set.h"

#include <stdlib.h>

typedef struct maillon Maillon;

struct maillon {
	void** elem;
	Maillon* next;
};

struct set {
	Maillon* chaine;
};

Set* initSet() {
	return (Set*)malloc(sizeof(Set));
}
Set* newSet() {
	Set* s=initSet();
	s->chaine=NULL;
	return s;
}
void freeSet(Set* s) {
	free(s);
}
void destroyChaine(Maillon* m) {
	if (m!=NULL) {
		destroyChaine(m->next);
		free(m);
	}
}
void destroySet(Set* s) {
	destroyChaine(s->chaine);
	freeSet(s);
}

int isInSet(Maillon* m, void* o) {
	if (m==NULL) return 0;
	if (*(m->elem)==o) return 1;
	return isInSet(m->next, o);
}

int addToSet(Set* s, void* o) {
	Maillon* m;
	for (m=s->chaine; m!=NULL; m=m->next) {
		if (*(m->elem)==o) {
			return 0;
		}
	}
	Maillon* m2=(Maillon*)malloc(sizeof(Maillon));
	void** obj=(void**)malloc(sizeof(void*));
	*obj=o;
	m2->next=s->chaine;
	m2->elem=obj;
	s->chaine=m2;
	return 1;
}

int removeFromSet(Set* s, void* o) {
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

void eachSet(Set* s, void (*function)(void**)) {
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
