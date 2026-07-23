#include "input.h"
#include "ansi.h"
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

termsize get_term_size(void) {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return (termsize){.WIDTH = w.ws_col, .HEIGHT = w.ws_row};
}

termsize await_resize() {
  termsize glob = get_term_size();
  int old = glob.HEIGHT * glob.WIDTH;
  while (old == glob.HEIGHT * glob.WIDTH) {
    glob = get_term_size();
    usleep(50);
  }

  // termsize changed
  return glob;
}

int MOUSE_SCROLL_Y = 0;
int MOUSE_X = 0;
int MOUSE_Y = 0;

struct termios orig_termios;

// === Terminal Setup ===
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

static const struct {
  const char *seq;
  int key;
} key_map[] = {{"A", KEY_ARROW_UP},
               {"B", KEY_ARROW_DOWN},
               {"C", KEY_ARROW_RIGHT},
               {"D", KEY_ARROW_LEFT},
               {"1~", KEY_HOME},
               {"2~", KEY_INSERT},
               {"3~", KEY_DELETE},
               {"4~", KEY_END},
               {"5~", KEY_PAGE_UP},
               {"6~", KEY_PAGE_DOWN},
               {"11~", KEY_F1},
               {"12~", KEY_F2},
               {"13~", KEY_F3},
               {"14~", KEY_F4},
               {"15~", KEY_F5},
               {"17~", KEY_F6},
               {"18~", KEY_F7},
               {"19~", KEY_F8},
               {"20~", KEY_F9},
               {"21~", KEY_F10},
               {"23~", KEY_F11},
               {"24~", KEY_F12},
               {NULL, 0}};

int input(InputEvent *local) {
  enable_raw_mode();
  enable_mouse_reporting();

  *local = (InputEvent){0};
  local->type = ERROR;

  char buf[32];
  ssize_t len = read(STDIN_FILENO, buf, sizeof(buf) - 1);
  disable_raw_mode();

  if (len <= 0)
    return local->type;

  buf[len] = '\0';

  // === SGR Mouse Input (e.g. "\x1b[<0;50;20M") ===
  if (len >= 6 && buf[0] == '\033' && buf[1] == '[' && buf[2] == '<') {
    int btn_full, x, y;
    char event_type;

    if (sscanf(buf, "\033[<%d;%d;%d%c", &btn_full, &x, &y, &event_type) == 4) {
      int btn = btn_full & 0b11; // lower two bits for actual button
      int is_scroll =
          (btn_full & 0b1100000) >> 5; // 64 (scroll up), 65 (scroll down)

      MOUSE_X = local->data.mouse.x = x;
      MOUSE_Y = local->data.mouse.y = y;
      local->data.mouse.m = btn;

      if (btn_full == 64 || btn_full == 65) {
        MOUSE_SCROLL_Y += (btn_full == 64) ? 1 : -1;
        local->data.scroll_y = MOUSE_SCROLL_Y;
        local->type = MOUSE_SCROLL;
        return local->type;
      }

      if (event_type == 'M') {
        local->type = MOUSE_CLICK;
        if (local->data.mouse.m == 3)
          local->type = MOUSE_HOVER;
      } else if (event_type == 'm') {
        local->type = MOUSE_RELEASE;
      }

      return local->type;
    }

    // Invalid mouse sequence
    local->type = ERROR;
    return local->type;
  }

  // === Escape sequences for special keys ===
  if (len >= 2 && buf[0] == '\033' && buf[1] == '[') {
    const char *seq = buf + 2;
    size_t seq_len = len - 2;
    for (int i = 0; key_map[i].seq != NULL; i++) {
      if (strncmp(seq, key_map[i].seq, seq_len) == 0 &&
          strlen(key_map[i].seq) == seq_len) {
        local->data.special_key = key_map[i].key;
        local->type = SPECIAL_KEY;
        return local->type;
      }
    }
  }

  // === Single character input ===
  if (len == 1) {
    local->data.keychar[0] = buf[0];
    local->data.keychar[1] = '\0';
    local->type = KEY;
    return local->type;
  }

  // === Fallback unknown multi-byte sequence ===
  strncpy(local->data.keychar, buf, len);
  local->data.keychar[len] = '\0';
  return local->type;
}

int await_scroll() {
  const int old = MOUSE_SCROLL_Y;
  InputEvent buff;
  while (old == MOUSE_SCROLL_Y)
    input(&buff);
  return MOUSE_SCROLL_Y;
}

void log_InputEvent(InputEvent local) {
  printf("returnc: %d\n", local.type);
  switch (local.type) {
  case KEY:
    printf("keycode: %s\n", local.data.keychar);
    break;

  case SPECIAL_KEY:
    printf("keycode: %d\n", local.data.special_key);
    break;

  case MOUSE_SCROLL:
    printf("scrolly: %d\n", local.data.scroll_y);
    break;

  case MOUSE_HOVER:
    printf("mouse x: %d\n", local.data.mouse.x);
    printf("mouse y: %d\n", local.data.mouse.y);
    break;

  case MOUSE_CLICK:
    printf("mouse m: %d\n", local.data.mouse.m);
    break;

  default:
    printf("Unknown Input\n");
    break;
  }
}
