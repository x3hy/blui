#ifndef BLUI
#define BLUI
#ifdef __cplusplus
extern "C" {
#endif


enum {
	BLUI_SUCCESS = -1,
	BLUI_NOT_INITIALISED = -2,
	BLUI_OUT_OF_SPACE = -3
};

struct blui_e {
	int x;
	int y;
	int w;
	int h;
	char *desc;
	int _id;
	int _txt;
	int _offset;
};

struct blui {
	struct blui_e *clients;
	int *windows;
	int index;

	// meta
	int _idx; // client count
	int _size; // client maximum
};

static struct blui _BLUI; // global BLUI app structure
#define ANSI_SPACE_SIZE 32

// initializes an app
static int _blui_c_app (struct blui_e *e, int c)
{ // Initialises a window and/or the _BLUI app.
	_BLUI._idx = 0;
	_BLUI.clients = e;
	_BLUI._size = c;

	// Initialise a client
	return 0;
}

// Appends a new client to the app
#define blui_c_client(id, ...) _blui_c_client(id, (struct blui_e){__VA_ARGS__})
static int _blui_c_client(int id, struct blui_e e)
{
	// Handle errors
	if (!_BLUI._size) return BLUI_NOT_INITIALISED;
	else if (_BLUI._size <= (_BLUI._idx+1))
		return BLUI_OUT_OF_SPACE;

	// Append the new client
	e._id = id;
	_BLUI.clients[_BLUI._idx] = e;
	_BLUI._idx++;

	return _BLUI._idx;
}

typedef int (*print_func)(const char *, ...);
#define _blui_draw_text(x, y, str, printf) \
	printf("\033[%d;%dH%s", y, x, str)

static void _blui_draw_box (int x, int y, int w, int h, print_func printf)
{
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

static int _blui_strlen(const char *str)
{
	if (!str) return 0;
	int str_s = 0;

	while(str[str_s])
		str_s++;

	return str_s;
}

// Draw clients
static int blui_draw (int id, print_func printf)
{
	BLUI_FOR_EACH  {
		struct blui_e e = BLUI_CURRENT;
		if (e._id == id || id == -1)
		  {
			if (e._txt)
				_blui_draw_text(e.x, e.y, e.desc, printf);
			else _blui_draw_box(e.x, e.y, e.w, e.h, printf);
		  }
	}

	return BLUI_SUCCESS;
}

// Creates a left aligned text inside an
// element.
//   id = id given to the new title.
//   ix = index of element to shadow.
//   of = offset, x offset.
//    l = line number, line number of ix.
// text = text you want to display.
//
#define TEXT(id, ix, of, l, text)\
	_blui_c_client(id, (struct blui_e){\
		.x = _BLUI.clients[ix].x + of,\
		.y = l,\
		.desc = text,\
		._txt = 1\
		._offset = of\
	})

// Move text to the right
#define TEXT_RIGHT(px, ex)\
	do {\
		_BLUI.clients[ex].x =\
			+ _BLUI.clients[px].x\
			+ _BLUI.clients[px].w\
			- _blui_strlen(BLUI.clients[ex].desc)\
			- _BLUI.clients[ex]._offset;\
	while (0)

// Move text to the bottom
#define TEXT_CENTER(px, ex)\
	do {\
		_BLUI.clients[ex].x =\
			_BLUI.clients[px].x + \
			(\
				+ _BLUI.clients[px].w\
				+ _blui_strlen(\
					_BLUI.clients[ex].desc\
				)\
			) / 2\
			- _BLUI.clients[ex]._offset; \
	while (0)

// modifier for other text elements, that moves
// them to the bottom of the parent element.
#define TEXT_DOWN(px, ex) \
	_BLUI.clients[ex].h = _BLUI.clients[px].h-1


#ifdef __cplusplus
}
#endif
#endif
