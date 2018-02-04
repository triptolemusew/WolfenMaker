#include "WolfenGraphics.h"
#include <iostream>
#include <vector>
#include <map>

namespace WolfenGraphics {
	int g_Height, g_Width;

	//SDL Variables
	SDL_Window *window;
	SDL_Surface *surface;
	SDL_Event event = { 0 };
	const Uint8* inkeys;

	//Color Structs
	ColorRGB::ColorRGB(Uint8 r, Uint8 g, Uint8 b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}

	ColorRGB::ColorRGB(const ColorRGB8bit& color) {
		this->r = color.r;
		this->g = color.g;
		this->b = color.b;
	}

	ColorRGB::ColorRGB() {
		this->r = 0;
		this->g = 0;
		this->b = 0;
	}

	ColorRGB8bit::ColorRGB8bit(Uint8 r, Uint8 g, Uint8 b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}

	ColorRGB8bit::ColorRGB8bit(const ColorRGB& color) {
		this->r = color.r;
		this->g = color.g;
		this->b = color.b;
	}

	ColorRGB8bit::ColorRGB8bit()
	{
		this->r = 0;
		this->g = 0;
		this->b = 0;
	}
	
	void screen(int width, int height, const std::string& text) {
		g_Height = height;
		g_Width = width;

		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			std::cout << "Failed to initialize! " << SDL_GetError() << std::endl;
			SDL_Quit();
			std::exit(1);
		}
		else {
			window = SDL_CreateWindow(text.c_str(), SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
			surface = SDL_GetWindowSurface(window);
			if (SDL_MUSTLOCK(surface)) if (SDL_LockSurface(surface) < 0) return;
		}
	}

	void update() {
		SDL_UpdateWindowSurfaceRects(window, 0, 0);
	}

	void clear(const ColorRGB& color) {
		SDL_FillRect(surface, NULL, 65536 * color.r + 256 * color.g + color.b);
	}

	void setColor(int x, int y, const ColorRGB& color) {
		if (x < 0 || y < 0 || x >= g_Width || y >= g_Height)
			return;
		Uint32 colorSDL = SDL_MapRGB(surface->format, color.r, color.g, color.b);
		Uint32* bufp;
		bufp = (Uint32*)surface->pixels + y * surface->pitch / 4 + x;
		*bufp = colorSDL;
	}

	ColorRGB getColor(int x, int y) {
		if (x < 0 || y < 0 || x >= g_Width || y >= g_Height)
			return RGB_Black;
		Uint32* bufp;
		bufp = (Uint32*)surface->pixels + y * surface->pitch / 4 + x;
		Uint32 colorSDL = *bufp;
		ColorRGB8bit colorRGB;
		SDL_GetRGB(colorSDL, surface->format, &colorRGB.r, &colorRGB.g, &colorRGB.b);
		return ColorRGB(colorRGB);
	}

	void draw(Uint32* buffer){
		Uint32* bufp;
		bufp = (Uint32*)surface->pixels;
		for (int y = 0; y < g_Height; y++) {
			for (int x = 0; x < g_Width; x++) {
				*bufp = buffer[y*g_Width + x];
				bufp++;
			}
			bufp += surface->pitch / 4;
			bufp -= g_Width;
		}
	}

	void readKeys() {
		SDL_PollEvent(&event);
		inkeys = SDL_GetKeyboardState(NULL);
	}

	bool done(bool quit_if_esc, bool delay) {
		if (delay)
			SDL_Delay(5);
		int done = 0;
		if (!SDL_PollEvent(&event))
			return 0;
		readKeys();
		if (quit_if_esc && inkeys[SDLK_ESCAPE])
			done = 1;
		if (event.type == SDL_QUIT)
			done = 1;
		return done;
	}
}