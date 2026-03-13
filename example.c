#include <stdio.h>
#include "blui.h"

/*
 * A basic music player UI written in BLUI.
 * Read the README file for documentation.
 */

int main(void) {

	// initialize app:
	struct blui_e elements[10];
	_blui_c_app  (elements,10);
	enum { WINDOW_0, WINDOW_1, WINDOW_2 };

	const int w = 40, h = 20;

	// main window frame
	const int fr = blui_c_client(
		WINDOW_0,
		.x =  1, .y =  1,
		.w = w, .h = h,
		.desc = "First window",
	);

	// Song name
	/* TEXT_LEFT ( */
	/* 	WINDOW_1, fr, 2, */
	/* 	"Track Name - Album" */
	/* ); */

	// Artist name
	blui_c_client(
		WINDOW_1,
		.x = 3, .y = 3, .w = w,
		._txt = 1, ._rgt = 0,
		.desc = "Artist Name",
	);

	// left controls
	blui_c_client(
		WINDOW_1,
		.x = 3, .y = (h-1), .w = w,
		._txt = 1, ._rgt = 0,
		.desc = "file stop",
	);

	// right controls
	blui_c_client(
		WINDOW_1,
		.x = 3, .y = (h-1), .w = w,
		._txt = 1, ._rgt = 1,
		.desc = "01:00/01:59"
	);

	// Center controls
	blui_c_client(
		WINDOW_2,
		.x = 0 , .y = (h-1), .w = w,
		._txt = 1, ._ctr = 1,
		.desc = "0.25"
	);
	
	// Draw the clients
	blui_draw(-1, printf);

	fflush(stdout);

	// wait infinitely
	return 0;
}
