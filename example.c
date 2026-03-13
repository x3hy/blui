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
	TEXT (WINDOW_1, fr, 3, 2,
		"Track Name - Album"
	);

	// Artist name
	TEXT (WINDOW_1, fr, 0, 3,
		"Artist Name"
	);

	// Left controls
	TEXT_RIGHT(ef,
		   TEXT (WINDOW_1, fr, 3, 0,
		"file stop"
	));

	// right controls
	TEXT_DOWN(fr,
		TEXT_DOWN(fr,
			TEXT(WINDOW_1, fr, 3, 0,
				"01:00/01:59"
			)
		)
	);
	
	// Center controls
	TEXT_DOWN(fr,
		TEXT_CENTER(	
			TEXT(WINDOW_1, fr, 3, 0,
				"0.25"
			)
		)
	);

	
	// Draw the clients
	blui_draw(-1, printf);

	fflush(stdout);

	// wait infinitely
	while(1){}

	blui_exit();
	return 0;
}
