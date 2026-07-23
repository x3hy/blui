#ifndef ANSI_H
#define ANSI_H
#define ESCAPE_CODE "\033"
#define ESC ESCAPE_CODE
#define ansi(seq) printf("%s%s", ESC, seq);
#define activate_terminal_buffer() ansi("[?1049h");
#define deactivate_terminal_buffer() ansi("[?1049l");
#define enable_mouse_reporting_ansi() ansi("[?1003h");
#define disable_mouse_reporting_ansi() ansi("[?1003l");
#define hide_cursor() ansi("[?25l");
#define show_cursor() ansi("[?25h");

// Special key codes (starting at 100+ to avoid overlap with ASCII)
#define KEY_ARROW_UP 100
#define KEY_ARROW_DOWN 101
#define KEY_ARROW_RIGHT 102
#define KEY_ARROW_LEFT 103
#define KEY_HOME 104
#define KEY_INSERT 105
#define KEY_DELETE 106
#define KEY_END 107
#define KEY_PAGE_UP 108
#define KEY_PAGE_DOWN 109
#define KEY_F1 110
#define KEY_F2 111
#define KEY_F3 112
#define KEY_F4 113
#define KEY_F5 114
#define KEY_F6 115
#define KEY_F7 116
#define KEY_F8 117
#define KEY_F9 118
#define KEY_F10 119
#define KEY_F11 120
#define KEY_F12 121

#define clear() printf("\033[H\033[J")
#define gotoxy(x, y)                                                           \
  printf("\033[%d;%dH", (y + 1),                                               \
         (x + 1)) // plus one because 1;1 is the same as 0;0
// #define clear_screen() ansi("[2J") DEPRICATED
#define cursor_home() ansi("[H")
#endif // ANSI_H
#define ANSI_H
