#ifndef BLUI_H
#define BLUI_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#define abs(num) ((num) < 0 ? -(num) : (num))
#define PI      3.14159265358979323846
#define TWO_PI  (2.0 * PI)
#define HALF_PI (0.5 * PI)
#define clear_screen() \
	fputs("\033[H\033[2J", stdout); \
	fflush(stdout);

struct pixel {
	char fill, *pre, *suf;
	int red, green, blue, color;
	double x, y, z;
};

struct screen {
	int max_x, max_y;
	int mean_x, mean_y;
	int min_x, min_y;
};

static inline double wrap_angle(double x){
	while (x > PI) x -= TWO_PI;
	while (x < -PI) x += TWO_PI;
	return x;
}

static double fast_sin(double x){
	x = wrap_angle(x);
	int sign = 1;
	
	if (x < 0) {
		sign = -1;
		x = -x;
	}
	
	if (x > HALF_PI) x = PI - x;
	double x2 = x * x;

	double y =
		(((-2.3889859e-08 * x2 +
			2.7525562e-06) * x2 -
			1.9840874e-04) * x2 +
			8.3333315e-03) * x2 -
			1.666666664e-01;
	
	return sign * (x + x * x2 * y);
}

static double fast_cos(double x){
	return fast_sin(x + HALF_PI);
}

static struct screen init_screen(int width, int height){
	struct screen out;
	out.max_x = width,
	out.max_y = height,

	out.mean_x = out.max_x / 2;
	out.mean_y = out.max_y / 2;

	out.min_x = -out.mean_x;
	out.min_y = -out.mean_y;

	return out;
}



// Converts relative co-ordinates into a 2d projection
static struct pixel project(struct screen dpy, struct pixel px, double focal){
	struct pixel out = px;
	if (px.z <= 0.1) return out;

	double zx = px.x / px.z;
	double zy = px.y / px.z;

	int relx = dpy.mean_x + (focal * zx);
	int rely = dpy.mean_y + (focal * zy);

	out.x = relx;
	out.y = rely;
	return out;
}

// Draw a pixel to the screen, uses the .fill char as the
// printed character. Prints in the rgb value inside the pixel
static int draw_pixel(struct screen dpy, struct pixel px){
	if (px.x > dpy.max_x || px.y > dpy.max_y || px.y < 0 || px.x < 0)
		return 1;

	printf("\033[%d;%dH", (int)px.x, (int)px.y);
	if (px.color){
		// Print the pixel at certain location
		printf("\033[38;2;%d;%d;%dm%c\033[0m", px.red, px.green, px.blue, px.fill);
	} else {
		printf("%s%c%s\n", px.pre, px.fill, px.suf);
	}

	fflush(stdout);
	return 0;
}

// Rotate a pixel on the Y axis
static struct pixel rotate_y(struct pixel px, double angle){
	double c = fast_cos(angle);
	double s = fast_sin(angle);
	struct pixel out = px;

	out.x = px.x * c + px.z * s;
	out.z = -px.x * s + px.z * c;
	return out;
}

// Rotate a pixel on the X axis
static struct pixel rotate_x(struct pixel px, double angle){
	double c = fast_cos(angle);
	double s = fast_sin(angle);
	struct pixel out = px;

	out.y = px.y * c - px.z * s;
	out.z = px.y * s + px.z * c;
	return out;
}

// Rotate a pixel on the Z axis
static struct pixel rotate_z(struct pixel px, double angle){
	double c = fast_cos(angle);
	double s = fast_sin(angle);
	struct pixel out = px;

	out.x = px.x * c - px.y * s;
	out.y = px.x * s + px.y * c;
	return out;
}

// Draw a line between two pixels (p1, p2). px is the style pixel, the fill and color is used from
// this pixel in the line.
static void draw_line(struct screen dpy, struct pixel p1, struct pixel p2, struct pixel px, double focal) {
	struct pixel a = project(dpy, p1, focal);
	struct pixel b = project(dpy, p2, focal);

	int x1 = (int)a.x, y1 = (int)a.y;
	int x2 = (int)b.x, y2 = (int)b.y;
	
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int sx = x1 < x2 ? 1 : -1;
	int sy = y1 < y2 ? 1 : -1;
	int err = dx - dy;

	while (1) {
		struct pixel out = px;
		out.x = x1;
		out.y = y1;
		draw_pixel(dpy, out);

		if (x1 == x2 && y1 == y2) break;
		int e2 = 2 * err;
		if (e2 > -dy) { err -= dy; x1 += sx; }
		if (e2 <  dx) { err += dx; y1 += sy; }
	}
}

#ifdef __cplusplus
}
#endif
#endif
