/*
 * render.cpp
 *
 *  Created on: 11 de abr de 2016
 *      Author: Kawai
 */

#include "Render.h"

Render::Render() {
	exit = false;

	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		std::cout << "SDL_Init error #" << SDL_GetError() << std::endl;
		throw -1;
	}

	gWindow = SDL_CreateWindow("Perlin Noise",200,200,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	if(!gWindow){
		std::cout << "SDL_CreateWindow error #" << SDL_GetError() << std::endl;
		throw -2;
	}
	gScreenSurface = SDL_GetWindowSurface(gWindow);

	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	if(!gRenderer){
		std::cout << "SDL_CreateRenderer error #" << SDL_GetError() << std::endl;
		throw -3;
	}

	p = new PerlinNoise(SCREEN_WIDTH, SCREEN_HEIGHT);
}

Render::~Render() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	delete p;
}

void Render::run() {
//	Uint32 l = SDL_GetTicks();
//	Uint32 c = 0;
	double d = 0;
	setup();
	while(!exit){
//		c = SDL_GetTicks();
//		d = ((double)(c-l))/1000;
		input(d);
//		update(d);
//		render(d);
	}
}

void Render::setup() {
	int seed;
	int octaveCount;
	float persistence;
	float amplitude;
	std::cout << "Seed Persistence Amplitude OctaveCount" << std::endl;
	std::cin >> seed >> persistence >> amplitude >> octaveCount;
	p->amplitude = amplitude;
	p->persistance = persistence;

	float **base = p->generatePerlinNoise(seed,octaveCount);

	SDL_SetRenderDrawColor(gRenderer,0x00,0x00,0x00,0xFF);
	SDL_RenderClear(gRenderer);

	int c = 0;
	for(int i = 0; i < SCREEN_WIDTH; i++){
		for(int j = 0; j < SCREEN_HEIGHT; j++){
			SDL_Rect rect;
			rect.x = i;
			rect.y = j;
			rect.w = 1;
			rect.h = 1;
			c = base[i][j]*255;
			SDL_SetRenderDrawColor(gRenderer,c,c,c,0xFF);
			SDL_RenderFillRect(gRenderer,&rect);
			SDL_RenderDrawRect(gRenderer,&rect);
		}
	}

	Util::matrixDealloc(base,p->width,p->height);
	SDL_RenderPresent(gRenderer);
}

void Render::ss(char* title){
	SDL_Surface *sshot = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_RenderReadPixels(gRenderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
	SDL_SaveBMP(sshot, title);
	SDL_FreeSurface(sshot);
}

void Render::input(double delta) {
	while(SDL_PollEvent(&e)){
		if(e.type == SDL_QUIT){
			exit = true;
		}
		if(e.type == SDL_KEYDOWN){
			if(e.key.keysym.sym == SDLK_ESCAPE){
				exit = true;
			}
			else if(e.key.keysym.sym == SDLK_r){
				setup();
			}
		}
	}
}

void Render::update(double delta) {

}

void Render::render(double delta) {
}

