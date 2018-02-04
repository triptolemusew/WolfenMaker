#include <SDL.h>
#include "WolfenGraphics.h"
using namespace WolfenGraphics;

int main(int argc, char* args[]) {
	screen(640, 480, "Example");

	while (!done()) {
		setColor(10, 10, RGB_Black);
		clear();
		update();
	}
}