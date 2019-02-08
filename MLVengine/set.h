#ifndef __SET__
#define __SET__

typedef struct set Set;

Set* newSet();
void destroySet(Set* s);
int addToSet(Set* s, void* o);
int removeFromSet(Set* s, void* o);
void eachSet(Set* s, void (*function)(void**));

#endif
