#ifndef BLUI
#define BLUI
#ifdef __cplusplus
extern "C" {
#endif

// Structures and Functions ----------------------------------------------------
struct blui_e; // Clients within a window
struct blui; // Main app structure
int _blui_c_client (int, struct blui_e); // Creates a "client"
struct blui _BLUI; // global BLUI app structure
#define ANSI_SPACE_SIZE 32

// Enumerators -----------------------------------------------------------------
enum {
	BLUI_SUCCESS = -1,
	BLUI_NOT_INITIALISED = -2,
	BLUI_OUT_OF_SPACE = -3
};

// -----------------------------------------------------------------------------

struct blui_e {
	int x;
	int y;
	int w;
	int h;
	char *desc;
	int _id;
	int _txt;
	int _rgt;
	int _ctr;
};

struct blui {
	struct blui_e *clients;
	int *windows;
	int index;

	// meta
	int _idx; // client count
	int _size; // client maximum
};

// initializes an app
int _blui_c_app (struct blui_e *e, int c)
{ // Initialises a window and/or the _BLUI app.
	_BLUI._idx = 0;
  	_BLUI.clients = (void *)0;
	_BLUI.clients = e;
	_BLUI._size = c;

	// Initialise a client
	return 0;
}

// Appends a new client to the app
#define blui_c_client(id, ...) _blui_c_client(id, (struct blui_e){__VA_ARGS__})
int _blui_c_client(int id, struct blui_e e)
{
	// Handle errors
	if (!_BLUI._size) return BLUI_NOT_INITIALISED;
	else if (_BLUI._size <= (_BLUI._idx+1))
		return BLUI_OUT_OF_SPACE;

	// Append the new client
	e._id = id;
	_BLUI.clients[_BLUI._idx] = e;
	_BLUI._idx++;

	return _BLUI._idx++;
}

typedef int (*print_func)(const char *, ...);
void _blui_draw_text(int x, int y, const char *str, print_func printf){
	printf("\033[%d;%dH%s", y, x, str);
}

void _blui_draw_box (int x, int y, int w, int h, print_func printf){
	const int size = w+1+ANSI_SPACE_SIZE;
	char t[size], m[size], b[size];

	// top bottom and middle
	// rather bland, edit if you wanna
	t[0] = t[w-1] = b[0] = b[w-1] = '+';
	m[0] = m[w-1] = '|';
	for (int i = 1; i < (w-1); i++){
		t[i] = b[i] = '-';
		m[i] = ' ';
	}

	t[w] = m[w] = b[w] = '\0';

	// print top and bottom rows
	_blui_draw_text(x, y, t, printf);
	_blui_draw_text(x, (y+h-1), b, printf);
	
	// print middle
	for (int i = 1; i < (h-1); i++)
		_blui_draw_text(x, y+i, m, printf);
}


// Exits safely
#define blui_exit()\
	do { \
		_BLUI.clients = (void *)0; \
	} while (0)

// Loop through each client
#define BLUI_FOR_EACH \
	for (int _index = 0; _index < _BLUI._idx; _index++)

// Get looped item at index of loop
#define BLUI_CURRENT _BLUI.clients[_index]

int _blui_strlen(const char *str){
	if (!str) return 0;
	int str_s = 0;

	while(str[str_s])
		str_s++;

	return str_s;
}

// Draw clients
int blui_draw (int id, print_func printf){
	BLUI_FOR_EACH  {
		struct blui_e e = BLUI_CURRENT;
		if (e._id == id || id == -1)
		  {
			if (e._txt)
				if (e._rgt)      _blui_draw_text((e.w - _blui_strlen(e.desc) + 2 - e.x), e.y, e.desc, printf);
				else if (e._ctr) _blui_draw_text(e.x + (e.w/2) - (_blui_strlen(e.desc)/2) + 1, e.y, e.desc, printf);
				else             _blui_draw_text(e.x, e.y, e.desc, printf);
			else                 _blui_draw_box(e.x, e.y, e.w, e.h, printf);
		  }
	}

	return BLUI_SUCCESS;
}

#ifdef __cplusplus
}
#endif
#endif
