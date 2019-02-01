#include "render.h"

#include <MLV/MLV_all.h>
#include "object.h"
#include "vector.h"

void renderInit() {
	MLV_create_window("test", "test", 400, 400);
}

void drawObject(Object** o) {
	Vector* v = getPos(*o);
	MLV_draw_filled_circle(getXInt(v), getYInt(v), 3, MLV_COLOR_RED);
}

void renderScene(Scene* s) {
	MLV_clear_window( MLV_COLOR_BLACK );
	eachObjects(s, drawObject);
	MLV_update_window();
}
