#include "WolfenGraphics.h"
#include <iostream>
using namespace WolfenGraphics;

int main(int argc, char * args[]) {
	screen(640, 480, "Example");

	while (!run())
	{
		setPixel(10, 10, ColorRGB(255, 255, 255));
		Draw_HorLine(50, 2, 400, ColorRGB(255, 255, 255));
		Draw_VerLine(100, 100, 200, ColorRGB(255, 255, 255));
		readKeys();
		updateScreen();
	}
	
	sleep();
}
