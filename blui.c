// Blui 0.1.2
#ifndef BLUI_LIB

enum {
	blui_success,
	blui_fail,
	blui_mem_error,
	__BLUI_END__
};

static const char *blui_error_descriptions[__BLUI_END__] = {	
	/** [enum] = (char*) **/
	[blui_success] = "Function returned sucessfully",
	[blui_fail] = "Fail",
	[blui_mem_error] = "Memory Error"
};

#define BLUI_LIB
#ifndef BLUI_NO_HEADER
// Dependancies
#include <unistd.h>
#endif
#ifdef BLUI_DEBUG
// Debug macros
#include <stdio.h>
#ifndef BLUI_LOG_FILE
#define BLUI_LOG_FILE "./.blui.log"
#endif
#define log(e) \
	do { \
		FILE *log = fopen(BLUI_LOG_FILE, "a"); \
		if (log){ \
			const char *err_str = (e > _BLUI_ERR_COUNT) ? BLUI_ERROR_DESC[e] : "Unknown Error"; \
			fprintf(log, "%s@%d - %s", __FUNCTION__,  __LINE__, err_str); \
		} \
	} while(0);
#define ret(e) \
	log(e) \
	return e 
#else 
#define log(e)
#define ret(e) \
	return e
#endif

// Definitions
#define BLUI_ESC ""

#define blui_loop(u) \
	char buf[64];\
	int n;\
	while((n=read(STDIN_FILENO, buf, sizeof(buf))) > 0)


// Structures
typedef struct {
	int a;
	int b;
} BLUI_VEC2;

typedef struct {
	BLUI_VEC2 pos;  // x and y
	BLUI_VEC2 size; // w and h
	int class;      // class identifier
	int id;         // unique element id
	char *prefix;   // printed before
	char *suffix;   // printed after
} BL_elem;


typedef struct { 
	union {
		BL_elem *elements;
		int idx;
		int cap;
	} elements;
	int index;
	int scenes;
} BL_app;

static BL_app _BLUI_APP;


int
_BL_strlen (char * str)
{ if (!str) return 0;
	int out = 0;
	while (str[out]) out++;
	return out+1;
}


// print an element
static void
_BL_place(BL_elem e)
{
	int size = 0;
	size += _BL_strlen(e.prefix) * e.size.b;
	//for (int i = 0; i < e.size.b; i++)
}

// init app
#define BL_create_app(...) _BL_create_app((BL_app){__VA_ARGS__})
static void
_BL_create_app (BL_app app)
{
	_BLUI_APP = app;
}

// init element
#define BL_create_elem(...) _BL_create_elem((BL_elem){__VA_ARGS__})
static BL_elem 
_BL_create_elem (BL_elem elem)
{
	return elem;
}

#endif


// termsize function
static void
get_term_size (int *w, int *h)
{
	*w = 10;
	*h = 10;
	return;
}

// Responsive element
static BL_elem
responsive_element(){
	return (BL_elem){
		// ...
	};
}



int
main (int argc, char *argv[])
{
	// Initalize app
	BL_create_app (
		.scenes=1,
		.term = get_term_size
	);
	
	// Create element
	BL_append(0, BL_create_elem (
		.pos = (BLUI_VEC2){10,10},
		.size = (BLUI_VEC2){10,10}
	));

	// Append responsive element
	BL_special(0, responsive_element);	

	BL_loop(){
		// Draw the 0th scene
		BL_draw(0);
		
		// Update class
		// my_app.update(0);
	}

	return 0;	
}


































