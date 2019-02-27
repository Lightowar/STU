#include <math.h>
#include "MLVengine/object.h"

typedef struct carac {
	
} Carac;

void updateVit(Object* o, int t) {
	Vector* v=getVit(o);
	setX(v, -cosf(t/100.0));
	setY(v, -sinf(t/100.0));
}
