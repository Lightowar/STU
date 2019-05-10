#include "input.h"

#include <string.h>

#include <MLV/MLV_all.h>

int isPressed (const char *key_string) {
	char str[40]="MLV_KEYBOARD_";
	strcat(str, key_string);
	return MLV_get_keyboard_state(MLV_convert_string_to_keyboard_button(str))==MLV_PRESSED;
}

Vector getMousePos() {
	int x, y;
	MLV_get_mouse_position(&x, &y);
	return newVector(x, y);
}

int click (int pos) {
	switch(pos) {
		case 0: return MLV_get_mouse_button_state(MLV_BUTTON_LEFT)==MLV_PRESSED;
		case 1: return MLV_get_mouse_button_state(MLV_BUTTON_MIDDLE)==MLV_PRESSED;
		case 2: return MLV_get_mouse_button_state(MLV_BUTTON_RIGHT)==MLV_PRESSED;
	}
	return 0;
}
