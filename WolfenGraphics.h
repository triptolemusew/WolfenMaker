#ifndef WOLFENGRAPHICS_H
#define WOLFENGRAPHICS_H

#include <SDL.h>
#include <SDL_image.h>

#include <string>
#include <vector>
#include <algorithm>

namespace WolfenGraphics {
	//Colors
	struct ColorRGB8bit;
	struct ColorRGB
	{
		int r;
		int g;
		int b;

		ColorRGB(Uint8 r, Uint8 g, Uint8 b);
		ColorRGB(const ColorRGB8bit& color);
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

	//a color with 3 components: r, g and b
	struct ColorRGB8bit
	{
		Uint8 r;
		Uint8 g;
		Uint8 b;

		ColorRGB8bit(Uint8 r, Uint8 g, Uint8 b);
		ColorRGB8bit(const ColorRGB& color);
		ColorRGB8bit();
	};

	//Global vars for the height and width of the screen;
	extern int g_Width;
	extern int g_Height;

	//Creating screen
	void screen(int width = 640, int height = 400, const std::string& text = " ");
	
	//Screen Manipulation
	void update();	
	void clear(const ColorRGB& color = RGB_Black);
	void draw(Uint32* buffer);
	void setColor(int x, int y, const ColorRGB& color);
	ColorRGB getColor(int x, int y);

	//Timer & Delay
	void sleep(double seconds);
	unsigned long getTicks();
	inline double getTime() { return getTicks() / 1000.00; };
	bool done(bool quit_if_esc = true, bool delay = true);
	void end();

	//Graphics - Basic 2D Shapes
	bool horLine(int y, int x1, int x2, const ColorRGB& color);
	bool verLine(int x, int y1, int y2, const ColorRGB& color);
	
	//Input - Keyboard & Mouse 
	void readKeys();
	void getMouseState(int&, int&);

	//Audio - TODO
}
#endif // !WOLFENGRAPHICS_H
