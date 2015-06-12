#include "tictactoe.h"
#include "strategy.h"

void setup_game( int *to_move_ptr, char *player_mark_ptr, char *computer_mark_ptr ) {
    /* scanf for input on chars is problematic-seeming */
    char user_input[255];

    while( *player_mark_ptr != 'x' && *player_mark_ptr != 'o' ) {
        printf( "Do you want x or o? " );
       
        collect_string( user_input, 255 );

        *player_mark_ptr = user_input[0];

        if( strlen( user_input ) > 1 
            || ( *player_mark_ptr != 'x' && *player_mark_ptr != 'o' ) ) {
            printf( "I don't understand %s\n", user_input );
        }
    }
    
    if( *player_mark_ptr == 'x' ) {
        *computer_mark_ptr = 'o';
    }
    else {
        *computer_mark_ptr = 'x';
    }

    while( *to_move_ptr != PLAYER_MOVE && *to_move_ptr != COMPUTER_MOVE ) {
        printf( "Who goes first? %s%i you%s, or %s%i me%s? ",
            PLAYER_COLOR
          , PLAYER_MOVE
          , KNRM
           
          , COMPUTER_COLOR
          , COMPUTER_MOVE
          , KNRM);
        
        scanf( "%i", to_move_ptr );
        if( *to_move_ptr != PLAYER_MOVE && *to_move_ptr != COMPUTER_MOVE ) {
            printf( "I don't understand %i\n", *to_move_ptr );
            clear_stdin();
        }
    } 
}

void display_board( char board[BOARD_Y][BOARD_X], char player_mark, char computer_mark ) {
    /*****************************
     * board is 3x3 array of chars,
     * from top-to-bottom to ease
     * drawing, shown here with
     * coordinate markings:
     *
     *  3  [0][0], [0][1], [0][2]
     *  2  [1][0], [1][1], [1][2]
     *  1  [2][0], [2][1], [2][2]
     *         a       b       c
     *
     */

    int x, y;

    for( y = 0; y < BOARD_Y; y++ ) {
        printf(     "\n%s%i%s   "
            , KMAG
            , 3 - y /* y axis for display 3, 2, 1 veritically down */ 
            , KNRM );
        for( x = 0; x < BOARD_X; x++ ) {
            printf(         "   %s%c%s   %s%c%s"
                /* taken cell */
                , board[y][x] == player_mark
                    ? PLAYER_COLOR
                    : COMPUTER_COLOR
                , board[y][x]
                , KNRM

                /* cell division */
                , KBLU
                , x != 2 ? '|' : '\0'
                , KNRM );
        }
        if( y != 2 ) {
            printf( "\n      %s-------------------%s"
                , KBLU
                , KNRM );
        }
    }
    printf(         "\n%s       a       b      c%s\n"
        , KMAG
        , KNRM );
}

int is_legal_move( char *move_str, int *move_x_ptr, int *move_y_ptr ) {
    int len = strlen( move_str );

    if( len != 2 ) {
        return FALSE;
    }
    
    switch( move_str[0] ) {
        case 'a':
        case 'A':
        case '0':
            *move_x_ptr = 0;
            break;

        case 'b':
        case 'B':
        case '1':
            *move_x_ptr = 1;
            break;

        case 'c':
        case 'C':
        case '2':
            *move_x_ptr = 2;
            break;

        default:
            return FALSE;
    }
    
    switch( move_str[1] ) {
        case '3':
            *move_y_ptr = 0;
            break;

        case '2':
            *move_y_ptr = 1;
            break;

        case '1':
            *move_y_ptr = 2;
            break;

        default:
            return FALSE;
    }

    return TRUE;
}

int is_available_move( char board[BOARD_Y][BOARD_X], int move_x, int move_y ) {
    return board[move_y][move_x] == ' ';
}

/* returns text of computer move */
const char* computer_move( PLAY_MOVE_ARGS ) {
    char *computer_move = malloc( sizeof(char) * 2 );
    /* how to play:
     * 1. play a winning move
     * 2. check if oponent has a winning move and block that
     * 3. check if center is available
     * 4. check if a corner is available
     * 5. take anything left
     */
    
    if( win_or_block( PLAY_MOVE_CALL, computer_move ) ) {
        return (const char *)computer_move;
    }

    if( play_center( PLAY_MOVE_CALL, computer_move ) ) {
        return (const char *)computer_move;
    }

    if( play_corner( PLAY_MOVE_CALL, computer_move ) ) {
        return (const char *)computer_move;
    }

    play_first_available( PLAY_MOVE_CALL, computer_move );

    return (const char *)computer_move;
}

/* returns text of player move */
const char* player_move( PLAY_MOVE_ARGS ) {
    
    char *player_move = malloc( sizeof( char ) * 255 );
    int move_x, move_y;

    /* loop to ask the player for a legal move until either the game is
     * over the player plays a legal move and play shifts to computer */
    while( *to_move_ptr == PLAYER_MOVE && *game_status_ptr == GAME_ACTIVE ) { 
        printf( "Your move (eg b1, c2): " );
        collect_string( player_move, 255 );
    
        if( !is_legal_move( player_move, &move_x, &move_y ) 
                || ! is_available_move( board, move_x, move_y )
        ) {
            printf( "\n%s\"%s\" is not a legal move.%s\n"
                , KRED
                , player_move
                , KNRM);
            
            strcpy( player_move, "" );
            break;
        }
        
        board[move_y][move_x] = player_mark;
    
        evaluate_board( PLAY_MOVE_CALL );
        *to_move_ptr = COMPUTER_MOVE;
    }
    return (const char *)player_move;
}

void new_game( void ) {
    int game_status = GAME_ACTIVE;
    char player_mark = INIT_STATUS, computer_mark = INIT_STATUS;
    char *display_text = malloc( sizeof(char) * 255 );
    int to_move = INIT_STATUS;
    
    char board[BOARD_Y][BOARD_X] = { { ' ', ' ', ' ' }, { ' ', ' ', ' ' }, { ' ', ' ', ' ' } };
    char *last_move = malloc( sizeof(char) * 2 );
    char *last_move_text = malloc( sizeof(char) * 24 );
    char *move_history = malloc( sizeof(char) * 255 );
    int last_move_text_len;

    printf( "Let's play!\n" );

    setup_game( &to_move, &player_mark, &computer_mark );

    display_board( board, player_mark, computer_mark );
    while( game_status == GAME_ACTIVE ) {
        printf( "\n" );
        if( to_move == PLAYER_MOVE ) {
            strcpy( last_move, player_move( board, &to_move, &game_status
                                , player_mark, computer_mark, display_text ) );
            
            sprintf( last_move_text, "%s%s%s, ", PLAYER_COLOR, last_move, KNRM ); 
        }
        else {
            strcpy( last_move, computer_move( board, &to_move, &game_status
                                , player_mark, computer_mark, display_text ) );
            
            sprintf( last_move_text, "%s%s%s, ", COMPUTER_COLOR, last_move, KNRM ); 
        }
       
        display_board( board, player_mark, computer_mark );
        
        /* display the move history - HACKETY HACK HACK here to 
         * not display the trailing comma by temporarily 
         * replacing it with a null terminator */
        if( strlen( last_move ) > 0 ) {
            strcat( move_history, last_move_text );
        }

        last_move_text_len = strlen( move_history );
        move_history[ last_move_text_len - 2 ] = '\0'; 
        
        printf( "Move history: %s\n", move_history );
        
        move_history[ last_move_text_len - 2 ] = ',';
    }
    printf( "%s", display_text );
    printf( "\n" );
}
