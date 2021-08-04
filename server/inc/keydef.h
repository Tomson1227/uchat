#ifndef KEYDEF_H
#define KEYDEF_H

#define BUFSIZE     1024

#define READ_OPT(reg, flag) !!(reg & (flag))

#define KEY_BACKSPASE   0x7F
#define KEY_NEW_LINE    0x0A
#define KEY_ESCAPE      0x1B
#define KEY_CTRL_SEQ    0x5E
#define KEY_TAB         0x09

#define ESC                 "\033"

/*    C1 set element ANSI escape sequences    */
#define CSI     ESC "["     //Controll Sequence Initiator
#define SS2     ESC "N"     //Single Shift Two
#define SS3     ESC "O"     //Single Shift Three
#define DCS     ESC "P"     //Device Control String
#define ST      ESC "\\"    //String Terminator
#define OSC     ESC "]"     //Operating System Command
#define SOS     ESC "X"     //Start Of String
#define PM      ESC "^"     //Privacy Message
#define APC     ESC "_"     //Aplication Program Command

/*    SGR(Select Grafical Rendition) parametrs    */
#define SGR_END             "m"

#define BOLD                CSI "1"  SGR_END
#define FAINT               CSI "2"  SGR_END
#define ITALIC              CSI "3"  SGR_END
#define UNDERLINE           CSI "4"  SGR_END
#define INVERT_COLOR        CSI "7"  SGR_END
#define TEXT_HIDE           CSI "8"  SGR_END
#define DEFAULT_FONT        CSI "10" SGR_END

/* FG - ForeGround */
#define FG_COLOR_RED        CSI "31" SGR_END
#define FG_COLOR_GREEN      CSI "32" SGR_END
#define FG_COLOR_ORANGE     CSI "33" SGR_END
#define FG_COLOR_BLUE       CSI "34" SGR_END
#define FG_COLOR_MAGENTA    CSI "35" SGR_END
#define FG_COLOR_CYAN       CSI "36" SGR_END
#define FG_COLOR_LIGHT_GRAY CSI "37" SGR_END
#define FG_COLOR_RESET      CSI "39" SGR_END

/* BG - BackGround */
#define BG_COLOR_BLACK      CSI "40" SGR_END
#define BG_COLOR_RED        CSI "41" SGR_END
#define BG_COLOR_GREEN      CSI "42" SGR_END
#define BG_COLOR_ORANGE     CSI "43" SGR_END
#define BG_COLOR_BLUE       CSI "44" SGR_END
#define BG_COLOR_MAGENTA    CSI "45" SGR_END
#define BG_COLOR_CYAN       CSI "46" SGR_END
#define BG_COLOR_LIGHT_GRAY CSI "47" SGR_END
#define BG_COLOR_RESET      CSI "49" SGR_END

#define RESET_ALL           CSI "0"  SGR_END

/* r,g,b can be 0 - 255 each */
#define SET_FG_RGB(r,g,b)   printf(CSI "38:2:%d:%d:%dm", r, g, b)
/* r,g,b can be 0 - 255 each */
#define SET_BG_RGB(r,g,b)   printf(CSI "48:2:%d:%d:%dm", r, g, b)
/* n can be 0 - 255 */
#define SET_FG_NUM(n)       printf(CSI "38:5:%dm", n)
/* n can be 0 - 255 */
#define SET_BG_NUM(n)       printf(CSI "48:5:%dm", n)
/* r -row  c - column */
#define MV_CURSOR(r, c)     printf(CSI "%d;%df", r, c)

/*    Terminal input sequences    */
#define INSERT_KEY          CSI "2~"
#define DELETE_KEY          CSI "3~"
#define PG_UP               CSI "5~"
#define PG_DOWN             CSI "6~"
#define F1_KEY              CSI "1P"
#define F2_KEY              CSI "1Q"
#define F3_KEY              CSI "1R"
#define F4_KEY              CSI "1S"

#define SAVE_CURSOR_POS     printf(CSI "s")
#define RESTORE_CURSOR_POS  printf(CSI "u")
#define HIDE_CURSOR         printf(CSI "?25l")
#define SHOW_CURSOR         printf(CSI "?25h")

#define ERASE_RIGHT         printf(CSI "0K")
#define ERASE_LEFT          printf(CSI "1K")
#define ERASE_LINE          printf(CSI "2K")

#define ERASE_DOWN          printf(CSI "0J")
#define ERASE_UP            printf(CSI "1J")
#define ERASE_ALL           printf(CSI "2J")

#define SCROLL_UP           printf(CSI "1S")
#define SCROLL_DOWN         printf(CSI "1T")

#define CURSUR_RIGHT        printf(CSI "1C")
#define CURSUR_LEFT         printf(CSI "1D")
#define CURSOR_UP           printf(CSI "1A")
#define CURSOR_DOWN         printf(CSI "1B")

#define CURSOR_NEW_LINE     printf(CSI "1E")
#define CURSOR_PREV_LINE    printf(CSI "1F")

/* x = [0 ... 9]*/
#define CURSOR_N_UP(x)      printf(CSI "%dA", x)
/* x = [0 ... 9]*/
#define CURSOR_N_DOWN(x)    printf(CSI "%dB", x)
/* x = [0 ... 9]*/
#define CURSOR_N_RIGHT(x)   printf(CSI "%dC", x)
/* x = [0 ... 9]*/
#define CURSOR_N_LEFT(x)    printf(CSI "%dD", x)

#define GET_CURSOR          printf(CSI "6n")

#endif /* KEYDEF_H */
