
#include <iostream>
#include <SDL.h>

using namespace std;

bool init();
void kill();

int main(int, char**) {

	if (!init()) {
		system("pause");
		return 1;
	}

	SDL_Surface* buf = SDL_LoadBMP("bitmap.bmp");
	SDL_Surface* image = SDL_ConvertSurfaceFormat(buf, SDL_PIXELFORMAT_RGB888, 0);
	SDL_FreeSurface(buf);

	for (int y = 0; y < image->h; y++) {
		for (int x = 0; x < image->w; x++) {
			int index = (y * image->pitch) + (x * image->format->BytesPerPixel);
			unsigned char* pixels = (unsigned char*)(image->pixels);

			if (pixels[index] == 0 && pixels[index + 1] == 0 && pixels[index + 2] == 0) {
				cout << "1 ";
			}
			else {
				cout << "0 ";
			}
		}
		cout << endl;
	}

	SDL_FreeSurface(image);

	kill();
	system("pause");
	return 0;
}

bool init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error initializing SDL: " << SDL_GetError() << endl;
		return false;
	}
	return true;
}

void kill() {
	SDL_Quit();
}
