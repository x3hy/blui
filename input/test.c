#include "input2.h"
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void disable_raw_mode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
  printf("\033[?1000l\033[?1003l\033[?1006l");
  printf("\033[?25h");
  fflush(stdout);
}

void enable_raw_mode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON);
  raw.c_cc[VMIN] = 1;
  raw.c_cc[VTIME] = 0;
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void enable_mouse_reporting() {
  printf("\033[?1000h\033[?1003h\033[?1006h");
  printf("\033[?25l");
  fflush(stdout);
}

void disable_mouse_reporting() {
  printf("\033[?1000l\033[?1003l\033[?1006l");
  printf("\033[25h");
  fflush(stdout);
}


int main(){
	enable_raw_mode();
	enable_mouse_reporting();
	_input values = {0};
	while (1){
		int ret = __input(&values);
		printf("%d - %d %d %d\n",ret, values.d1, values.d2, values.d3);
		fflush(stdout);
	}

	return 0;
}
