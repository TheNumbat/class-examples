
#include <iostream>
#include <vector>
#include <unordered_map>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

const float pixels_per_chunk = 250;

struct entity {
	entity() {
		x = y = w = h = 0;
		texture = nullptr;
	}
	float x, y, w, h;
	SDL_Texture* texture;
};

struct chunkpos {
	chunkpos() {
		tx = ty = 0;
	}
	chunkpos(int x, int y) {
		tx = x;
		ty = y;
	}
	bool operator==(const chunkpos& other) const {
		return tx == other.tx && ty == other.ty;
	}
	int tx, ty;
};
struct chunk {
	chunkpos pos;
	vector<entity> entities;
};
namespace std {
	template<>
	struct hash<chunkpos> {
		Uint64 operator()(const chunkpos& key) const {
			Uint64 one = hash<int>()(key.tx);
			Uint64 two = hash<int>()(key.ty);
			return (one << 1) ^ two;
		}
	};
}

struct camera {
	camera() {
		x = y = lastUpdate = 0;
	}
	float x, y;
	Uint32 lastUpdate;
};

unordered_map<chunkpos, chunk> chunks;
SDL_Window* window;
SDL_Renderer* renderer;
camera cam;

int w = 1280, h = 720;

bool init();
void loop();
void kill();
void add_entity(entity e, int tx, int ty);

int main(int, char**) {

	if (!init()) {
		system("pause");
		return 1;
	}

	loop();

	kill();
	return 0;
}

void loop() {
	bool running = true;
	const unsigned char* keys = SDL_GetKeyboardState(nullptr);
	while (running) {

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_RenderClear(renderer);

		SDL_Event ev;
		while (SDL_PollEvent(&ev) != 0) {
			switch (ev.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_WINDOWEVENT:
				if (ev.window.event == SDL_WINDOWEVENT_RESIZED ||
					ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					w = ev.window.data1;
					h = ev.window.data2;
				}
				break;
			case SDL_KEYDOWN:
				break;
			}
		}
		float dT = (SDL_GetTicks() - cam.lastUpdate) / 1000.0f;
		float cam_speed = 150.0f;
		if (keys[SDL_SCANCODE_W]) {
			cam.y -= dT * cam_speed;
		}
		if (keys[SDL_SCANCODE_A]) {
			cam.x -= dT * cam_speed;
		}
		if (keys[SDL_SCANCODE_S]) {
			cam.y += dT * cam_speed;
		}
		if (keys[SDL_SCANCODE_D]) {
			cam.x += dT * cam_speed;
		}
		cam.lastUpdate = SDL_GetTicks();

		int tx_min = floor(cam.x / pixels_per_chunk - w / 2 / pixels_per_chunk);
		int tx_max = ceil(cam.x / pixels_per_chunk + w / 2 / pixels_per_chunk);
		int ty_min = floor(cam.y / pixels_per_chunk - h / 2 / pixels_per_chunk);
		int ty_max = ceil(cam.y / pixels_per_chunk + h / 2 / pixels_per_chunk);
		for(int tx = tx_min; tx <= tx_max; tx++)
		{
			for (int ty = ty_min; ty <= ty_max; ty++)
			{
				auto centry = chunks.find(chunkpos(tx, ty));
				if (centry != chunks.end()) {
					SDL_Rect r;
					r.x = round(tx * pixels_per_chunk - cam.x + w / 2);
					r.y = round(ty * pixels_per_chunk - cam.y + h / 2);
					r.w = round(pixels_per_chunk);
					r.h = round(pixels_per_chunk);
					SDL_SetRenderDrawColor(renderer, 50, 50, 50, 100);
					SDL_RenderFillRect(renderer, &r);
					for (entity& e : centry->second.entities) {
						SDL_Rect r;
						r.x = round(w / 2 - cam.x + tx * pixels_per_chunk + e.x - e.w / 2);
						r.y = round(h / 2 - cam.y + ty * pixels_per_chunk + e.y - e.h / 2);
						r.w = round(e.w);
						r.h = round(e.h);
						SDL_RenderCopy(renderer, e.texture, nullptr, &r);
					}
				}
			}
		}
		SDL_Rect c;
		c.x = round(w / 2 - 5);
		c.y = round(h / 2 - 5);
		c.w = 10;
		c.h = 10;
		SDL_SetRenderDrawColor(renderer, 50, 0, 0, 255);
		SDL_RenderFillRect(renderer, &c);

		SDL_RenderPresent(renderer);
	}
}

void add_entity(entity e, int tx, int ty) {
	auto centry = chunks.find(chunkpos(tx, ty));
	if (centry == chunks.end()) {
		chunk newc;
		newc.pos.tx = tx;
		newc.pos.ty = ty;
		newc.entities.push_back(e);
		chunks.insert({ chunkpos(tx, ty), newc });
	}
	else {
		centry->second.entities.push_back(e);
	}
}

bool init() {
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	window = SDL_CreateWindow("chunk stuff", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
	SDL_Surface* load = IMG_Load("dankdude.png");
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, load);
	
	{
		entity e;
		e.texture = tex;
		e.x = 50;
		e.y = 50;
		e.w = load->w;
		e.h = load->h;
		add_entity(e, 0, 0);
	}

	{
		entity e;
		e.texture = tex;
		e.x = 150;
		e.y = 150;
		e.w = load->w;
		e.h = load->h;
		add_entity(e, 0, 0);
	}

	{
		entity e;
		e.texture = tex;
		e.x = 100;
		e.y = 100;
		e.w = load->w;
		e.h = load->h;
		add_entity(e, 1, 0);
	}

	{
		entity e;
		e.texture = tex;
		e.x = 50;
		e.y = 50;
		e.w = load->w;
		e.h = load->h;
		add_entity(e, 0, -1);
	}

	SDL_FreeSurface(load);

	return true;
}

void kill() {
	SDL_Quit();
}
