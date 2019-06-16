#include "other.h"

#include <MLV/MLV_all.h>

void changeFrameRate(int framerate) {
	MLV_change_frame_rate(framerate);
}

void waitEndFrame() {
	MLV_delay_according_to_frame_rate();
}

int randomInt(int min, int max) {
	return MLV_get_random_integer(min, max);
}

double randomDouble(int min, int max) {
	return MLV_get_random_double(min, max);
}
