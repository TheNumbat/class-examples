
#include <iostream>
#include <string>
#include <vector>
#include <functional>

#include <SDL.h>
#include <SDL_ttf.h>

using namespace std;

bool init();
void kill();
void loop();

struct game_state;

struct widget {
	widget() {
		x = y = w = h = r = g = b = 0;
	}
	widget(int _x, int _y, int _w, int _h, int _r, int _g, int _b) {
		x = _x; y = _y; w = _w; h = _h; r = _r; g = _g; b = _b;
	}
	virtual ~widget() {};
	int x, y, w, h;
	unsigned char r, g, b;
	bool hover = false;
	virtual void click(game_state&) = 0;
};

struct button : public widget {
	button() : widget() {}
	button(int _x, int _y, int _w, int _h, int _r, int _g, int _b, string t) 
		: widget(_x, _y, _w, _h, _r, _g, _b) {
		text = t;
	}
	string text;
	function<void(game_state&)> callback;
	void click(game_state& s) {
		callback(s);
	}
};

struct static_text : public widget {
	static_text(int _x, int _y, int _w, int _h, int _r, int _g, int _b, string t)
		: widget(_x, _y, _w, _h, _r, _g, _b) {
		text = t;
	}
	string text;
	void click(game_state& s) {}
};

struct menu_screen {
	vector<widget*> widgets;
};

struct game_state {
	vector<menu_screen> screens;
	int current_screen = 0;
	bool running = true;
};

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;
game_state s;

int main(int argc, char** args) {

	if ( !init() ) {
		system("pause");
		return 1;
	}

	loop();
	
	kill();
	return 0;
}

void loop() {

	while (s.running) {

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		SDL_Event ev;
		while (SDL_PollEvent(&ev) != 0) {
			switch (ev.type) {
			case SDL_QUIT:	
				s.running = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				for (widget* w : s.screens[s.current_screen].widgets) {
					if (ev.button.x >= w->x && ev.button.x <= w->x + w->w &&
						ev.button.y >= w->y && ev.button.y <= w->y + w->h) {
						w->click(s);
					}
				}
				break;
			case SDL_MOUSEMOTION:
				for (widget* w : s.screens[s.current_screen].widgets) {
					if (ev.button.x >= w->x && ev.button.x <= w->x + w->w &&
						ev.button.y >= w->y && ev.button.y <= w->y + w->h) {
						w->hover = true;
					}
					else {
						w->hover = false;
					}
				}
				break;
			}
		}

		for (widget* w : s.screens[s.current_screen].widgets) {
			SDL_Rect r =  { w->x, w->y, w->w, w->h };
			SDL_Color c = { w->r, w->g, w->b, 255 };

			if (button* bt = dynamic_cast<button*>(w)) {
				if (w->hover) {
					if(c.r >= 50)
						c.r -= 50;
					if(c.g >= 50)
						c.g -= 50;
					if(c.b >= 50)
						c.b -= 50;
				}

				SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
				SDL_RenderFillRect(renderer, &r);

				SDL_Surface* surf = TTF_RenderText_Shaded(font, bt->text.c_str(), { 0, 0, 0, 0 }, c);
				SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, surf);

				SDL_Rect tr;
				tr.x = r.x + 10;
				tr.y = r.y + 10;
				tr.w = r.w - 20;
				tr.h = r.h - 20;
				SDL_RenderCopy(renderer, text, NULL, &tr);

				SDL_FreeSurface(surf);
				SDL_DestroyTexture(text);
			}
			else if (static_text* st = dynamic_cast<static_text*>(w)) {
				SDL_Surface* surf = TTF_RenderText_Shaded(font, st->text.c_str(), c, { 255, 255, 255, 255 });
				SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, surf);

				SDL_RenderCopy(renderer, text, NULL, &r);

				SDL_FreeSurface(surf);
				SDL_DestroyTexture(text);
			}
		}
		SDL_RenderPresent(renderer);
	}
}

bool init() {
	if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
		cout << "Error initializing SDL: " << SDL_GetError() << endl;
		return false;
	} 

	TTF_Init();

	window = SDL_CreateWindow( "Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	if ( !window ) {
		cout << "Error creating window: " << SDL_GetError()  << endl;
		return false;
	}

	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
	if ( !renderer ) {
		cout << "Error creating renderer: " << SDL_GetError() << endl;
		return false;
	}

	font = TTF_OpenFont("font.ttf", 24);
	if (!font) {
		cout << "Error loading font: " << TTF_GetError() << endl;
		return false;
	}

	{
		menu_screen ms;
		{
			button* b = new button(10, 10, 100, 50, 200, 200, 200, "play");
			b->callback = [](game_state& s) -> void { cout << "clicked play button" << endl; };
			ms.widgets.push_back(b);
		}
		{
			button* b = new button(200, 150, 100, 50, 200, 200, 200, "quit");
			b->callback = [](game_state& s) -> void { cout << "clicked quit button" << endl;
			s.running = false; };
			ms.widgets.push_back(b);
		}
		{
			button* b = new button(20, 300, 300, 75, 100, 250, 170, "next screen");
			b->callback = [](game_state& s) -> void { cout << "clicked next button" << endl;
													  s.current_screen++; };
			ms.widgets.push_back(b);
		}
		s.screens.push_back(ms);
	}
	{
		menu_screen ms;
		{
			button* b = new button(20, 300, 300, 75, 100, 250, 200, "prev screen");
			b->callback = [](game_state& s) -> void { cout << "clicked prev button" << endl;
													  s.current_screen--; };
			ms.widgets.push_back(b);
		}
		{
			static_text* st = new static_text(100, 50, 250, 50, 100, 100, 100, "wew lade");
			ms.widgets.push_back(st);
		}
		s.screens.push_back(ms);
	}
	return true;
}

void kill() {
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	window = NULL;
	renderer = NULL;

	for (menu_screen& ms : s.screens) {
		for (widget* w : ms.widgets) {
			delete w;
		}
	}

	TTF_Quit();
	SDL_Quit();
}