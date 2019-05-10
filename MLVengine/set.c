#include "set.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct maillon Maillon;

struct maillon {
	void* elem;
	Maillon* next;
};

struct set {
	int (*equ)(void*, void*);
	Maillon* chaine;
};

Set* initSet() {
	return (Set*)malloc(sizeof(Set));
}
Set* newSet() {
	Set* s=initSet();
	s->chaine=NULL;
	s->equ=NULL;
	return s;
}
void freeSet(Set* s) {
	free(s);
}
void destroyChaine(Set* s, Maillon* m) {
	if (m!=NULL) {
		destroyChaine(s, m->next);
		free(m);
	}
}
void destroySet(Set* s) {
	destroyChaine(s, s->chaine);
	freeSet(s);
}

void* isInSetRec(Maillon* m, void* o, int (*equ)(void*, void*)) {
	if (m==NULL) return NULL;
	if (m->elem==o || (equ!=NULL && equ(m->elem, o))) return m->elem;
	return isInSetRec(m->next, o, equ);
}

void* isInSet(Set* s, void* o) {
	return isInSetRec(s->chaine, o, s->equ);
}

int addToSet(Set* s, void* o) {
	Maillon** m;
	for (m=&(s->chaine); (*m)!=NULL; m=&(*m)->next) {
		if ((*m)->elem==o || (s->equ!=NULL && s->equ((*m)->elem, o))) {
			return 0;
		}
	}
	Maillon* m2=(Maillon*)malloc(sizeof(Maillon));
	m2->next=NULL;
	m2->elem=o;
	(*m)=m2;
	return 1;
}

int removeFromSet(Set* s, void* o) {
	Maillon* m;
	Maillon* prec=NULL;
	for (m=s->chaine; m!=NULL; m=m->next) {
		if (m->elem==o || (s->equ!=NULL && s->equ(m->elem, o))) {
			if (prec==NULL) {
				s->chaine=m->next;
			} else {
				prec->next=m->next;
			}
			free(m);
			return 1;
		}
		prec=m;
	}
	return 0;
}

int almostRemoveFromSet(Set* s, void* o) {
	Maillon* m;
	for (m=s->chaine; m!=NULL; m=m->next) {
		if (m->elem==o || (s->equ!=NULL && s->equ(m->elem, o))) {
			m->elem=NULL;
			return 1;
		}
	}
	return 0;
}

void eachSet(Set* s, void (*function)(void**)) {
	Maillon* m;
	Maillon* prec = NULL;
	for (m=s->chaine; m!=NULL;) {
		if (!(m->elem==NULL || (s->equ!=NULL && s->equ(m->elem, NULL)))) {
			(*function)(&(m->elem));
			prec=m;
			m=m->next;
		} else {
			Maillon* m2 = m->next;
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

void setEqual(Set* s, int (*equ)(void*, void*)) {
	s->equ=equ;
}
