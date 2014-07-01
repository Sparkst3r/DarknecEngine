#include <SDL.h>

struct KeyBind {
	const char* name;
	int keyCode;
};

typedef void(*KeyFunction) (Uint32 time, bool** keysPressed);