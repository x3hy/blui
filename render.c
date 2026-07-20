#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "blui.h"

int usleep(__useconds_t usec);

// Get the screen size
struct screen _init_screen(){
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return init_screen(w.ws_row, w.ws_col);
}

// Action that occurs per-frame
double angle = 0.0;
struct pixel frame(struct pixel px){
	/* px = rotate_z(px, angle); */
	px = rotate_z(px, angle);
	px = rotate_y(px, angle);

	// initial Distance from camera
	px.z += 60;
	return px;
}

// main display loop
int main(){
	const int VEC_LEN = sizeof(vectors) / sizeof(vectors[0]);
	const int EDGE_LEN = sizeof(edges) / sizeof(edges[0]);
	int fps = 120;

	for (;;angle += 0.015){
		clear_screen();
		struct screen term = _init_screen();
		printf("\033[HScreen %dx%d\n", term.max_x, term.max_y);
		
		for (int j = 0; j < EDGE_LEN; j++){
			int a_idx = edges[j][0];
			struct pixel a = {
				.x = vectors[a_idx][0],
				.y = vectors[a_idx][1],
				.z = vectors[a_idx][2],
			};

			int b_idx = edges[j][1];
			struct pixel b = {
				.x = vectors[b_idx][0],
				.y = vectors[b_idx][1],
				.z = vectors[b_idx][2],
			};
;
			a = frame(a);
			b = frame(b);
			draw_line(term, a, b, (struct pixel){
				.fill = '#',
				.pre = "\033[32m",
				.suf = "\033[0m",
			}, (double)term.max_y*4);
		}

		// Draw each point in vector
		for (int j = 0; j < VEC_LEN; j++){
			struct pixel px = {
				.x = vectors[j][0],
				.y = vectors[j][1],
				.z = vectors[j][2],
				.fill = '$',
				.color = 1,
				.red = 255,
				.green = 0,
				.blue = 0,
			};

			// Rotate xz
			px = frame(px);
			draw_pixel(term, project(term, px, (double)term.max_y*4));
		}

		usleep(1000000/fps);
	}

	return 0;
}
