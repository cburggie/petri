#include <petri.h>

#include <iostream>

static const Uint32 palette[4] = {
	0xff000000,
	0xff505050,
	0xffa0a0a0,
	0xfff0f0f0
};

// sum of values in adjacent cells
//  0 -  2:  -  1
//  3 -  5: +/- 0
//  6 -  8:  +  1
//  9 - 11: +/- 0
// 12 - 14:  -  1
/*
static const int val_map[4][25] = {
	{0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,1,1,1,2,2,2,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,1,1,2,2,2,3,3,3,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{2,2,2,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2}
};
*/
static int val_map(int a, int b)
{
	if (b < 3) return a > 0 ? a - 1 : 0;
	if (b < 6) return a;
	if (b < 9) return a < 3 ? a + 1 : 3;
	if (b < 12) return a;
	return a > 0 ? a - 1 : 0;
}

static const int offset_1 = -1 - (petri::constants::width_in_cells + 2);
static const int offset_2 =    - (petri::constants::width_in_cells + 2);
static const int offset_3 =  1 - (petri::constants::width_in_cells + 2);
static const int offset_4 = -1;
static const int offset_5 =  1;
static const int offset_6 = -1 + (petri::constants::width_in_cells + 2);
static const int offset_7 =    + (petri::constants::width_in_cells + 2);
static const int offset_8 =  1 + (petri::constants::width_in_cells + 2);

petri::Dish::Dish()
{
	buf_width  = constants::width_in_cells  + 2;
	buf_height = constants::height_in_cells + 2;
	sur_width  = constants::width_in_cells  * constants::cell_size_in_pixels;
	sur_height = constants::height_in_cells * constants::cell_size_in_pixels;

	buf1 = nullptr;
	buf1 = new int[buf_width * buf_height];

	buf2 = nullptr;
	buf2 = new int[buf_width * buf_height];

	surface = nullptr;
	window = nullptr;

	clear();
}

petri::Dish::~Dish()
{
	if (buf1 != nullptr) delete [] buf1;
	if (buf2 != nullptr) delete [] buf2;
}

int petri::Dish::incrementCell(int x, int y)
{
	if (x < 0 
			|| constants::width_in_cells <= x
			|| y < 0
			|| constants::height_in_cells <= y)
	{
		return -1;
	}

	buf1[buf_width * (1+y) + 1 + x] += 1;
	buf1[buf_width * (1+y) + 1 + x] %= 4;

	return 0;
}

int petri::Dish::clear()
{
	int count = 0;
	if (buf1 != nullptr)
	{
		for (int i = 0; i < (buf_width * buf_height); i++)
		{
			count++;
			buf1[i] = 0;
		}
	}

	count = 0;
	if (buf2 != nullptr)
	{
		for (int i = 0; i < (buf_width * buf_height); i++)
		{
			count++;
			buf2[i] = 0;
		}
	}

	return 0;
}

int petri::Dish::setWindow(SDL_Window * w)
{
	if (w == nullptr) return -1;

	window = w;
	surface = SDL_GetWindowSurface(w);

	return 0;
}


int petri::Dish::tick()
{
	for (int y = 1; y < (1 + constants::height_in_cells); y++)
	{
		for (int x = 1; x < (1 + constants::width_in_cells); x++)
		{
			int pos = y * buf_width + x;
			/*
			std::cout << "updating cell (" << x << ',' << y << "), offset ";
			std::cout << pos << std::endl;

			int map_y = buf1[pos];
			std::cout << "val_map row: " << map_y << std::endl;

			int map_x = buf1[pos + offset_1] +
				buf1[pos + offset_2] +
				buf1[pos + offset_3] +
				buf1[pos + offset_4] +
				buf1[pos + offset_5] +
				buf1[pos + offset_6] +
				buf1[pos + offset_7] +
				buf1[pos + offset_8];
			std::cout << "val_map col: " << map_x << std::endl;

			int map_val = val_map[map_y][map_x];
			std::cout << "val_map value: " << map_val << std::endl;

			buf2[pos] = map_val;
			*/
			//buf2[pos] = val_map[buf1[pos]][
			buf2[pos] = val_map(buf1[pos],(
				buf1[pos + offset_1] +
				buf1[pos + offset_2] +
				buf1[pos + offset_3] +
				buf1[pos + offset_4] +
				buf1[pos + offset_5] +
				buf1[pos + offset_6] +
				buf1[pos + offset_7] +
				buf1[pos + offset_8]
			//];
				));
		}
	}

	//std::cout << "updating cells done" << std::endl;

	int * buftmp = buf1;
	buf1 = buf2;
	buf2 = buftmp;

	return 0;
}

int petri::Dish::draw()
{
	if (surface == nullptr) return -1;

	Uint32 * sur_buf = (Uint32*) surface->pixels;
	int pitch = surface->pitch / (sizeof (int));

	for (int y = 0; y < surface->h; y++)
	{
		int surface_offset = y * pitch;
		int buffer_offset = 1 + (1 + (y/constants::cell_size_in_pixels)) * buf_width;

		for (int x = 0; x < surface->w; x++)
		{
		/*
			int bo = buffer_offset+(x/constants::cell_size_in_pixels);
			int so = surface_offset + x;
			int po = buf1[bo];
			int color = palette[po];
			sur_buf[so] = color;
		*/
			sur_buf[surface_offset + x] = palette[buf1[buffer_offset+(x/constants::cell_size_in_pixels)]];
		}
	}

	return 0;
}
