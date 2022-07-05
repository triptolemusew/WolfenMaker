#include "WolfenGraphics.h"
#include <iostream>
#include <vector>
#include <map>

namespace WolfenGraphics {
	int Height, Width;

	//SDL Variables
	SDL_Window *window;			
	SDL_Renderer *renderer;
	SDL_Surface *surface;
	SDL_Event event = { 0 };
	const Uint8* keys;

	//Individual array of pixels
	Uint32* pixels;

	//Color Structs
	ColorRGB::ColorRGB(Uint8 r, Uint8 g, Uint8 b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}

	ColorRGB::ColorRGB() {
		this->r = 0;
		this->g = 0;
		this->b = 0;
	}
	
	//Screen Manipulation - Pixel based
	void screen(int width, int height, const std::string& text) {
		Height = height;
		Width = width;

		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			std::cout << "Failed to initialize! " << SDL_GetError() << std::endl;
			SDL_Quit();
			std::exit(1);
		}
		else {
			window = SDL_CreateWindow(text.c_str(), SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
			//renderer = SDL_CreateRenderer(window, -1, 0);
			surface = SDL_GetWindowSurface(window);
			if (SDL_MUSTLOCK(surface))
				SDL_LockSurface(surface);
			/*pixels = (Uint32**)malloc(height * sizeof(Uint32*));
			for (int i = 0; i < height; i++)
				pixels[i] = (Uint32*)malloc(width * sizeof(Uint32));
			memset(pixels, 0, sizeof(pixels[0][0]) * height * width);*/
			pixels = new Uint32[width * height];
			memset(pixels, 0, width * height * sizeof(Uint32));
			//Renderer flags - SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE
		}
	}

	void updateScreen() {
		//SDL_UpdateWindowSurface(window);
		SDL_UpdateWindowSurfaceRects(window, 0, 0);
	}

	void clearScreen(const ColorRGB& color) {
		//SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		//SDL_RenderClear(renderer);
		SDL_FillRect(surface, NULL, 65536 * color.r + 256 * color.g + color.b);
	}

	bool run() {
		int done = 0;
		if (!SDL_PollEvent(&event))
			return 0;
		readKeys();
		if (keys[SDLK_ESCAPE])
			done = 1;
		if (event.type == SDL_QUIT)
			done = 1;
		return done;
	}

	void setPixel(int x, int y, const ColorRGB& color) {
		if (x < 0 || y < 0 || x >= Width || y >= Height) 
			return;
		Uint32 colors = SDL_MapRGB(surface->format, color.r, color.g, color.b);
		Uint32 *buffer = (Uint32 *)surface->pixels + y * surface->pitch + x * sizeof(*buffer);
		*buffer = colors;
	}

	ColorRGB getPixel(int x, int y) {
		if (x < 0 || y < 0 || x >= Width || y >= Height)
			return RGB_Black;
		Uint32* bufp = (Uint32*)surface->pixels+y * surface->pitch + x * sizeof(*bufp);
		Uint32 colors = *bufp;
		ColorRGB colorRGB;
		SDL_GetRGB(colors, surface->format,(Uint8*) &colorRGB.r, (Uint8*)&colorRGB.g, (Uint8*)&colorRGB.b);
		return ColorRGB(colorRGB);
	}

	void draw(Uint32* buffer) {
		Uint32* bufp;
		bufp = (Uint32*)surface->pixels;
		for (int y = 0; y < Height; y++) {
			for (int x = 0; x < Width; x++) {
				*bufp = buffer[y*Width + x];
				bufp++;
			}
			bufp += surface->pitch / 4;
			bufp -= Width;
		}
	}

	//Timer & Delay
	void sleep(double seconds) {
		SDL_Delay(seconds * 1000);
	}

	void sleep() {
		int done = 0;
		SDL_PollEvent(&event);
		while (done == 0)
		{
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT) done = 0;
				if (event.type == SDL_KEYDOWN) done = 1;
			}
			SDL_Delay(5); //so it consumes less processing power
		}
	}

	void cleanUp() {
		delete[] pixels;
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_FreeSurface(surface);
		SDL_Quit();
	}

	//Inputs - Mouse & Keyboard
	void getMouseState(int& mouseX, int& mouseY) {
		SDL_GetMouseState(&mouseX, &mouseY);
	}
	
	void readKeys() {
		SDL_PollEvent(&event);
		keys = SDL_GetKeyboardState(NULL);
		if (keys[SDL_GetScancodeFromKey(SDLK_UP)]) {
			std::cout << "Read W!" << std::endl;
		}
	}

	//2D Graphics Callback
	bool Draw_HorLine(int y, int x1, int x2, const ColorRGB& color) {
		if (x2 < x1) {
			x1 += x2;
			x2 = x1 - x2;
			x1 -= x2;
		}

		if (x2 < 0 || x1 > Width || y < 0 || y >= Height)
			return 0;

		if (x1 < 0) x1 = 0;
		if (x2 >= Width) x2 = Width = 1;

		Uint32 colors = SDL_MapRGB(surface->format, color.r, color.g, color.b);
		Uint32 *buffers = (Uint32*)surface->pixels + y * surface->pitch + x1 * sizeof(*buffers);
		for (int x = x1; x <= x2; x++)
		{
			*buffers = colors;
			buffers++;
		}
		return 1;
	}

	bool Draw_VerLine(int x, int y1, int y2, const ColorRGB& color) {
		if (y2 < y1) {
			y1 += y2;
			y2 = y1 - y2;
			y1 -= y2;
		}

		if (y2 < 0 || y1 >= Height || x < 0 || x >= Width)
			return 0;

		if (y1 < 0) y1 = 0;
		if (y2 >= Width) y2 = Height - 1;

		Uint32 colors = SDL_MapRGB(surface->format, color.r, color.g, color.b);
		Uint32 *buffers = (Uint32*)surface->pixels + y1 * surface->pitch / 4 + x;
		for (int y = y1; y <= y2; y++)
		{
			*buffers = colors;
			buffers += surface->pitch / 4;
		}
		return 1;
	}
}