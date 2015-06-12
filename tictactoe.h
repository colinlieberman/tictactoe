#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEADER_TEXT "%s                       \n\
        //////////////////////////////      \n\
       //                          //       \n\
      //    %stic%s   /     /         //        \n\
     //    ----- /-----/-----    //         \n\
    //          / %stac%s /         //          \n\
   //     -----/-----/-----    //           \n\
  //          /     / %stoe%s     //            \n\
 //                          //             \n\
//////////////////////////////              \n\
%s\n"

#define MENU_OPT_NONE       0
#define MENU_OPT_PLAY       1
#define MENU_OPT_EXIT       2
#define MENU_OPT_SAVE       3

#define MENU_OPT_RETRIEVE   4

#define GAME_ACTIVE         0
#define GAME_PLAYER_WON     1
#define GAME_PLAYER_LOST    2
#define GAME_DRAWN          3

#define INIT_STATUS         0
#define PLAYER_MOVE         1
#define COMPUTER_MOVE       2

#define BOARD_X             3
#define BOARD_Y             3

/* http://stackoverflow.com/questions/3585846/color-text-in-terminal-aplications-in-unix */
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define PLAYER_COLOR        KYEL
#define COMPUTER_COLOR      KGRN

#define TRUE                1
#define FALSE               0

#define WINNING             0
#define BLOCKING            1

#define WINNING_LINES              {   \
           /* from the top left */     \
/* 0 */    { {0,0}, {0,1}, {0,2} }     \
/* 1 */   ,{ {0,0}, {1,1}, {2,2} }     \
/* 2 */   ,{ {0,0}, {1,0}, {2,0} }     \
                                       \
          /* from the top center */    \
/* 3 */   ,{ {0,1}, {1,1}, {2,1} }     \
                                       \
           /* from the top right */    \
/* 4 */   ,{ {0,2}, {1,2}, {2,2} }     \
/* 5 */   ,{ {0,2}, {1,1}, {2,0} }     \
                                       \
          /* from the middle left */   \
/* 6 */   ,{ {1,0}, {1,1}, {1,2} }     \
                                       \
          /* from the bottom left */   \
/* 7 */   , { {2,0}, {2,1}, {2,2} }    \
}

#define LINES_BY_CELL    {   \
    { "012",   "03", "045" } \
   ,{  "26", "1356",  "46" } \
   ,{ "257",   "37", "147" } \
}

#define NUM_WINNING_LINES   8

#define PLAY_MOVE_SIG       char[BOARD_Y][BOARD_X], int *, int *, char, char, char *
#define PLAY_MOVE_ARGS      char board[BOARD_Y][BOARD_X], int *to_move_ptr, int *game_status_ptr, \
                                char player_mark, char computer_mark, char *display_text
#define PLAY_MOVE_CALL      board, to_move_ptr, game_status_ptr, player_mark, computer_mark, display_text

void clear_stdin( void );
void collect_string( char *, int );
