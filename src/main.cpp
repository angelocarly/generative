#include "rtweekend.h"
#include "vec3.h"
#include <cstdlib>
#include <iostream>
#include "color.h"
#include <vector>
#include <cmath>

struct line {
	vec3 a;
	vec3 b;
};

// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines 
// intersect the intersection point may be stored in the floats i_x and i_y.
char get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y, 
    float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y)
{
    float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    float s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        if (i_x != NULL)
            *i_x = p0_x + (t * s1_x);
        if (i_y != NULL)
            *i_y = p0_y + (t * s1_y);
        return 1;
    }

    return 0; // No collision
}

void generate_lines(std::vector<line> *lines, int width, int height)
{
	srand(101);
	for(int i=0; i<40000; i++)
	{
		line l = { 
			vec3(random_double() * width, random_double() * height, 0), 
			vec3(random_double() * width, random_double() * height, 0)
		};

		bool intersect = false;
		for(line lc : *lines)
		{
			if (get_line_intersection(l.a.x(), l.a.y(), l.b.x(), l.b.y(), lc.a.x(), lc.a.y(), lc.b.x(), lc.b.y(), nullptr, nullptr) == 1)
			{
				intersect = true;
				break;
			}
		}

		if (intersect)
		{
			i--;
			continue;
		}

		lines->push_back(l);
		std::cerr << i << "\n";
	}

}


int main() 
{
	// Image
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 4096;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 1;
	const int max_depth = 50;

	std::vector<line> *lines = new std::vector<line>();
	lines->resize(10);
	generate_lines(lines, image_width, image_height);

	color* screen = new color[image_height * image_width];

	for(line l : *lines)
	{
		vec3 delta = l.b - l.a;
		delta /= delta.length();
		float total_length = (l.b - l.a).length();

		for(int t=0; t<=total_length / delta.length(); t++)
		{
			vec3 p = l.a + delta * t;
			int x = floor(p.x());
			int y = floor(p.y());
			screen[y * image_width + x] += color(0.3, 0.3, 0.3);
		}
	}

	// Render
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	for(int i=0; i<image_width * image_height; i++)
	{
		write_color(std::cout, screen[i], 1);
	}

	std::cerr << "\nDone.\n";
}
