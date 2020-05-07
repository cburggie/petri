#ifndef _PETRI_H_
#define _PETRI_H_

#include <SDL2/SDL.h>

namespace petri
{
	namespace constants
	{
		extern const int width_in_cells;
		extern const int height_in_cells;
		extern const int cell_size_in_pixels;
		extern const char * window_name;
	}

	class Dish
	{
		private:
			int buf_width;
			int buf_height;
			int sur_width;
			int sur_height;

			int * buf1;
			int * buf2;

			SDL_Surface * surface;
			SDL_Window * window;

		public:
			Dish();
			~Dish();

			int incrementCell(int x, int y);
			int clear();
			int tick();
			int draw();
			int setWindow(SDL_Window * w);
	};

	class App
	{
		private:
			bool initialized, running, paused;

			int width, height;
			unsigned int second_timestamp;
			unsigned int frame_timestamp;


			SDL_Window * window;
			Dish * dish;

		public:
			App();
			~App();

			int init();
			int exec();
			int loop();
			int onEvent(SDL_Event * e);
			int onWindowEvent(SDL_Event * e);
			int onMouseEvent(SDL_Event * e);
			int onKeyEvent(SDL_Event * e);
			int render();
			int quit();
	};
}

#endif
