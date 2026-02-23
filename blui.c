#ifndef BLUI_LIB
#define BLUI_LIB
#ifdef __cplusplus
extern "C" {
#endif

// element struct
typedef struct BLUI_elem (*BLUI_resp)(void);
struct BLUI_elem {
	struct {
		int x;
		int y;
		int w;
		int h;
	} size;
	int _rendered; // toggle for if the element is rendered
	BLUI_resp *resp; // responsive element given.
} BLUI_elem;

// main app struct
typedef struct BLUI_app {
	struct {
		int count;
		int index;
		struct BLUI_elem *list;
	} scenes;
	int index;
} BLUI_app;

/** 
 * TODO:
 * init application function
 * append scene to app
 * draw elements based off of scene
 **/  



#ifdef __cplusplus
}
#endif
#endif
