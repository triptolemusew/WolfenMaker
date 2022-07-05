#ifndef WOLFENGRAPHICS_H
#define WOLFENGRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <vector>
#include <algorithm>

namespace WolfenGraphics {
	//Colors
	struct ColorRGB
	{
		int r;
		int g;
		int b;

		ColorRGB(Uint8 r, Uint8 g, Uint8 b);
		ColorRGB();
	};

	static const ColorRGB RGB_Black(0, 0, 0);
	static const ColorRGB RGB_Red(255, 0, 0);
	static const ColorRGB RGB_Green(0, 255, 0);
	static const ColorRGB RGB_Blue(0, 0, 255);
	static const ColorRGB RGB_Cyan(0, 255, 255);
	static const ColorRGB RGB_Magenta(255, 0, 255);
	static const ColorRGB RGB_Yellow(255, 255, 0);
	static const ColorRGB RGB_White(255, 255, 255);
	static const ColorRGB RGB_Gray(128, 128, 128);
	static const ColorRGB RGB_Grey(192, 192, 192);
	static const ColorRGB RGB_Maroon(128, 0, 0);
	static const ColorRGB RGB_Darkgreen(0, 128, 0);
	static const ColorRGB RGB_Navy(0, 0, 128);
	static const ColorRGB RGB_Teal(0, 128, 128);
	static const ColorRGB RGB_Purple(128, 0, 128);
	static const ColorRGB RGB_Olive(128, 128, 0);

	//Global vars for the height and width of the screen;
	extern int Width;
	extern int Height;

	//Creating screen
	void screen(int width = 640, int height = 400, const std::string& text = " ");
	
	//Screen Manipulation
	void updateScreen();	
	void clearScreen(const ColorRGB& color);
	void draw(Uint32* buffer);
	void setPixel(int x, int y, const ColorRGB& color);
	ColorRGB getPixel(int x, int y);

	//Timer & Delay
	void sleep();
	void sleep(double seconds);
	inline unsigned long getTicks() { return SDL_GetTicks(); };
	inline double getTime() { return getTicks() / 1000.00; };
	bool run();

	//Graphics - Basic 2D Shapes
	bool Draw_HorLine(int y, int x1, int x2, const ColorRGB& color);
	bool Draw_VerLine(int x, int y1, int y2, const ColorRGB& color);
	
	//Input - Keyboard & Mouse 
	void readKeys();
	void getMouseState(int&, int&);

	//Cleanup
	void cleanUp();

	//Audio - TODO
}
#endif // !WOLFENGRAPHICS_H
