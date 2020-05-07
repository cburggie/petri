
#include <petri.h>
#include <iostream>

petri::App::App()
{
	initialized = false;
	running = false;
	paused = false;
	window = nullptr;
	width = 0;
	height = 0;
	dish = nullptr;
}

petri::App::~App()
{
	if (initialized || window != nullptr)
	{
		quit();
	}
}

int petri::App::init()
{
	if (initialized) return -1;

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER)) return -1;

	width  = constants::width_in_cells  * constants::cell_size_in_pixels;
	height = constants::height_in_cells * constants::cell_size_in_pixels;

	window = SDL_CreateWindow(
			constants::window_name,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			width,
			height,
			SDL_WINDOW_HIDDEN);

	if (window == nullptr) return -1;

	dish = new Dish();
	dish->setWindow(window);

	initialized = true;
	return 0;
}

int petri::App::exec()
{
	running = true;
	paused = true;

	SDL_Event e;
	SDL_ShowWindow(window);

	while (running)
	{
		loop();
		render();

		while (SDL_PollEvent(&e) == 1)
		{
			onEvent(&e);
		}
	}

	quit();

	return 0;
}

int petri::App::loop()
{
	static int frames_this_second = 0;

	unsigned int ticks = SDL_GetTicks();
	frames_this_second++;

	if (ticks - second_timestamp >= 1000)
	{
		std::cout << frames_this_second << " fps" << std::endl;
		second_timestamp = ticks;
		frames_this_second = 0;
	}

	if (paused)
	{
		SDL_Delay(50);
		dish->draw();
	}

	else
	{
		dish->tick();
		dish->draw();

		int draw_time = SDL_GetTicks() - frame_timestamp;
		if (draw_time < 1000)
		{
			std::cout << "delaying for " << 1000 - draw_time << std::endl;
			SDL_Delay(1000 - draw_time);
			frame_timestamp = SDL_GetTicks();
		}
	}

	return 0;
}


int petri::App::render()
{
	SDL_UpdateWindowSurface(window);
	return 0;
}

int petri::App::quit()
{
	if (!initialized) return -1;

	if (dish != nullptr)
	{
		delete dish;
	}

	if (window != nullptr)
	{
		SDL_DestroyWindow(window);
		window = nullptr;
	}

	initialized = false;

	SDL_Quit();

	return 0;
}

int petri::App::onEvent(SDL_Event * e)
{
	if (e == nullptr) return -1;

	switch (e->type)
	{
		case SDL_QUIT:
			paused = true;
			running = false;
			break;
		case SDL_WINDOWEVENT:
			onWindowEvent(e);
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			onMouseEvent(e);
			break;
		case SDL_KEYDOWN:
			onKeyEvent(e);
			break;
		default:
			break;
	}

	return 0;
}

int petri::App::onWindowEvent(SDL_Event * e)
{
	int ret = 0;
	switch (e->window.event)
	{
		case SDL_WINDOWEVENT_HIDDEN:
		case SDL_WINDOWEVENT_MINIMIZED:
			paused = true;
		default:
			ret = -1;
			break;
	}

	return ret;
}

int petri::App::onMouseEvent(SDL_Event * e)
{
	if (e->type != SDL_MOUSEBUTTONDOWN) return -1;
	if (!paused) return -1;
	if (e->button.button != SDL_BUTTON_LEFT) return -1;

	int x = e->button.x / constants::cell_size_in_pixels;
	int y = e->button.y / constants::cell_size_in_pixels;

	std::cout << "incrementing cell at (" << x << ',' << y << ")" << std::endl;

	return dish->incrementCell(x,y);
}

int petri::App::onKeyEvent(SDL_Event * e)
{
	if (e->type != SDL_KEYDOWN) return -1;

	switch (e->key.keysym.sym)
	{
		case SDLK_q:
		case SDLK_ESCAPE:
			running = false;
			break;
		case SDLK_SPACE:
			frame_timestamp = SDL_GetTicks();
			paused = paused ? false : true;
			break;
		default:
			break;
	}

	return 0;
}

