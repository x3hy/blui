#ifndef INPUT_H
#define INPUT_H
#include <stdio.h>
#define INPUT_BUF_SIZE 32
/**
 * Who needs unistd.h
 * 0 = STDIN_FILENO
 * 1 = STDOUT_FILENO
 * 2 = STDERR_FILENO
 **/
#define FILENO 0
// Checks if (char *)b starts with "\033[<" and if over the (int)l of 6
#define is_sgr(b, l) ((l) >= 6 && (b[0]) == 27 && (b[1]) == 91 && (b[2]) == 60)
// Input Struct
typedef struct {
/* If you REALLY wanted to conserve stack memory here,
 * you could change this datatype over to a unsigned
 * char which is only 1 byte instead of an ints 4.
 *
 * This would only allow for mouse positions within
 * 0-256.
 */
unsigned int d1, d2, d3;
} _input;
typedef enum {
INPUT_ERROR, // flushes d1 and d2
/*
 * When this code is returned the x and y are stored in
 * d1 and d2. The individual mouse click event type is
 * held in d3. they mean the following:
 * d3 = 3: mouse is hovering on x,y
 * d3 = 77: mouse has clicked x,y
 * d3 = 109: mouse is releasing click on x,y
 */
INPUT_CLICK,
/*
 * on this return code the prior INPUT_CLICK behavior
 * is inherited, the only difference is that the scroll
 * amount is written to the d3 variable. If the event
 * was a scroll upward the value will be 1, if it was
 * downwards then d3 will be 2 (0 is reserved for
 * errors) */
INPUT_SCROLL,
INPUT_KEYCODE // writes key code to d1
} input_type;
// Assumes FILENO is streaming SGR style mouse data and is in ICANON ECHO mode
static input_type __input(_input *out){
out->d1 = out->d2 = 0;
char buf[INPUT_BUF_SIZE];
// Open stream and read code from FILENO ...
FILE *stream = fdopen(FILENO, "r");
if (fgets(buf, sizeof(buf) - 1, stream) == NULL)
// Unlikley: error caused during input
return INPUT_ERROR;
// ..get length and null-terminate
size_t len = 0;
while (buf[len]) len++;
buf[len] = '\0';
/* Don't need to check for zerolen len
 * as fgets handles this. */
// Mouse event handling
if (is_sgr(buf, len)){
char raw_btn;
// Copy the values from SGR FILENO
if (sscanf(buf, "\033[<%d;%d;%d%c",
(int *)&raw_btn, &out->d1, &out->d2, (char *)&out->d3) != 4)
// Malformed SGR
return INPUT_ERROR;
// Get the mouse button and set output
// const unsigned int btn = raw_btn & 0b11;
if (raw_btn == 64 || raw_btn == 65){
out->d3 = (raw_btn == 64) ? 1 : 2;
return INPUT_SCROLL;
} else return INPUT_CLICK;
// Regular keypress (does not detect state)
}
if (len == 1){
out->d1 = buf[0];
return INPUT_KEYCODE;
}
// No pattern recognisd
return INPUT_ERROR;
}
#endif // INPUT_H
