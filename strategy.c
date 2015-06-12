#include "tictactoe.h"

/* returns one of GAME_ACTIVE, GAME_DRAWN
 *                  GAME_PLAYER_WON, GAME_PLAYER_LOST
 */
int evaluate_board( PLAY_MOVE_ARGS ) {

    int winning_lines[NUM_WINNING_LINES][3][2] = WINNING_LINES;
    int i, j, x, y, num_computer, num_player, is_on_line = FALSE;

    int num_available_lines = NUM_WINNING_LINES;
    int last_available_line = 0;

    const char *lines_by_cell[BOARD_Y][BOARD_X] = LINES_BY_CELL;
    const char *cur_line_ptr; /* for evaluating free cells for draws */

    for( i=0; i<NUM_WINNING_LINES; i++ ) {
        num_computer = 0;
        num_player   = 0;

        for( j=0; j<3; j++ ) {
            y = winning_lines[i][j][0];
            x = winning_lines[i][j][1];

            if( board[y][x] == player_mark ) {
                num_player++;
            }
            else if( board[y][x] == computer_mark ) {
                num_computer++;
            }
        }

        if( num_player == 3) {
            *game_status_ptr = GAME_PLAYER_WON;
            sprintf( display_text, "%sYou won!%s (%c%i, %c%i, %c%i)\n"
              , PLAYER_COLOR
              , KNRM
              , 'a' + winning_lines[i][0][1]
              , 3 - winning_lines[i][0][0]
              , 'a' + winning_lines[i][1][1]
              , 3 - winning_lines[i][1][0]
              , 'a' + winning_lines[i][2][1]
              , 3 - winning_lines[i][2][0]
                );
            return *game_status_ptr;
        }
        else if( num_computer == 3 ) {
            *game_status_ptr = GAME_PLAYER_LOST;
            sprintf( display_text, "%sI won!%s (%c%i, %c%i, %c%i)\n"
              , COMPUTER_COLOR
              , KNRM
              , 'a' + winning_lines[i][0][1]
              , 3 - winning_lines[i][0][0]
              , 'a' + winning_lines[i][1][1]
              , 3 - winning_lines[i][1][0]
              , 'a' + winning_lines[i][2][1]
              , 3 - winning_lines[i][2][0]
                );
            return *game_status_ptr;
        }
        else if( num_computer && num_player ) {
            num_available_lines--;
           /* printf( "line %i busted, %i remain\n", i, num_available_lines ); */
        }
        else {
/*            printf( "line %i available, %i total left\n", i, num_available_lines ); */
            last_available_line = i;
        }
    }


    if( num_available_lines == 1 ) {
        /* check if there are any cells available on other lines:
         * find all the cells not on last_available_line, and
         * if any are free, it's not a draw
         */

        for( y=0; y<BOARD_Y; y++ ) {
            for( x=0; x<BOARD_X; x++ ) {
                if( board[y][x] == ' ' ) {
                    cur_line_ptr = lines_by_cell[y][x];
                    is_on_line = FALSE;
                    while( *cur_line_ptr ) {
                        /* if this free cell is on the current line,
                         * it could still be a draw; if we get through
                         * all the lines for this cell, and it's not on
                         * the current line, we know it can't be a draw
                         */

                        /* hack to convert know numeric char to int */
                        if( (*cur_line_ptr) - '0' == last_available_line ) {
                            is_on_line = TRUE;
                            break;
                        }
                        cur_line_ptr++;
                    }
                    if( ! is_on_line ) {
                        /* none of this cell's lines are on the last
                         * active line, so this isn't a draw situation
                         */
                        return GAME_ACTIVE;
                    }
                }
            }
        }

    }
    
    /* if 1 line's available and it didn't break, it's a draw;
     * if 0 lines are available, it's a draw
     */
    if( num_available_lines <= 1 ) {
        *game_status_ptr = GAME_DRAWN;
        sprintf( display_text, "%sDraw!%s\n", KCYN, KNRM );
        return *game_status_ptr;
    }

    return GAME_ACTIVE;
}

void set_computer_move( PLAY_MOVE_ARGS, int x, int y, char *computer_move ){

    /* output the move coordinates */
    sprintf( computer_move, "%c%i", 'a' + x, 3 - y );
    printf( "\nMy Move: %s\n", computer_move );

    board[y][x] = computer_mark;
    if( evaluate_board( PLAY_MOVE_CALL ) != GAME_ACTIVE ) {
        return;
    }
    *to_move_ptr = PLAYER_MOVE;
}

/* returns bool whether a move was made */
int play_first_available( PLAY_MOVE_ARGS, char *computer_move ) {
    int x, y;

    /* though it should just be the non-corner edges,
     * check everything anyway as a fallback
     */
    for( y=0; y<BOARD_Y; y++ ) {
        for( x=0; x<BOARD_X; x++ ) {
            if( board[y][x] == ' ' ) {
                set_computer_move( PLAY_MOVE_CALL, x, y, computer_move );
                return TRUE;
            }
        }
    }

    return FALSE;
}

/* returns bool whether a move was made */
int play_corner( PLAY_MOVE_ARGS, char *computer_move ) {
    int corners[4][2] = {
        {0,0}, {0,2}
       ,{2,0}, {2,2}
    };
    int x, y, i, num_corners = 4;

    /* take first available corner */
    for( i=0; i<num_corners; i++ ) {
        x = corners[i][1];
        y = corners[i][0];

        if( board[y][x] == ' ' ) {
            set_computer_move( PLAY_MOVE_CALL, x, y, computer_move );
            return TRUE;
        }
    }

    return FALSE;
}

/* returns bool whether a move was made */
int play_center( PLAY_MOVE_ARGS, char *computer_move ) {
    if( board[1][1] == ' ' ) {
        set_computer_move( PLAY_MOVE_CALL, 1, 1, computer_move );
        return TRUE;
    }

    return FALSE;
}

/* returns bool whether a move was made */
int win_or_block( PLAY_MOVE_ARGS, char *computer_move ) {

    int winning_lines[NUM_WINNING_LINES][3][2] = WINNING_LINES;
    int move_one[2], move_two[2], move_three[2];
    char pos_one, pos_two, pos_three, mark_looking_for;
    int i, looking_for;

    /* iterate over winning lines, the first time we find a winner, play that
     * then go again, and look for blocking, and play the first of that
     */

    for( looking_for = WINNING; looking_for <= BLOCKING; looking_for++ ) {
        if( looking_for == WINNING ) {
            mark_looking_for = computer_mark;
        }
        else {
            mark_looking_for = player_mark;
        }

        for( i = 0; i < NUM_WINNING_LINES; i++ ) {

            move_one[0]   = winning_lines[i][0][0];
            move_one[1]   = winning_lines[i][0][1];
            pos_one       = board[move_one[0]][move_one[1]];

            move_two[0]   = winning_lines[i][1][0];
            move_two[1]   = winning_lines[i][1][1];
            pos_two       = board[move_two[0]][move_two[1]];

            move_three[0] = winning_lines[i][2][0];
            move_three[1] = winning_lines[i][2][1];
            pos_three     = board[move_three[0]][move_three[1]];

            /* win or block at pos three */
            if( pos_one == mark_looking_for && pos_two == mark_looking_for && pos_three == ' ' ) {

                set_computer_move( PLAY_MOVE_CALL, move_three[1], move_three[0], computer_move );
                return TRUE;
            }

            /* win or block at pos two */
            if( pos_one == mark_looking_for && pos_two == ' ' && pos_three == mark_looking_for ) {

                set_computer_move( PLAY_MOVE_CALL, move_two[1], move_two[0], computer_move );
                return TRUE;
            }

            /* win or block at pos three */
            if( pos_one == ' ' && pos_two == mark_looking_for && pos_three == mark_looking_for ) {

                set_computer_move( PLAY_MOVE_CALL, move_one[1], move_one[0], computer_move );
                return TRUE;
            }
        }
    }
    return FALSE;
}
