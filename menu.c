#include "tictactoe.h"
#include "play.h"

void show_welcome( void ) {
    char *header_color = KBLU;

    printf( HEADER_TEXT
        /* start of header */
        , header_color

        /* start of tic */
        , KRED

        /* end of tic */
        , header_color

        /* start of tac */
        , KYEL

        /* end of tac */
        , header_color

        /* start of toe */
        , KGRN

        /* end of toe */
        , header_color

        /* end of header */
        , KNRM );
}

void do_menu( void ) {
    int user_opt = MENU_OPT_NONE;

    while( user_opt == MENU_OPT_NONE ) {

        printf( "Menu: %i: Play\t%i: Exit\n"
            , MENU_OPT_PLAY
            , MENU_OPT_EXIT );

        printf( "Your choice: " );
        scanf( "%d", &user_opt );

        switch( user_opt ) {
            case MENU_OPT_PLAY:
                new_game();
                break;

            case MENU_OPT_EXIT:
                printf( "Bye bye!\n" );
                return;
                break;

            default:
                printf( "I don't understand %d\n", user_opt );
                clear_stdin();
                break;
        }

        user_opt = MENU_OPT_NONE;
    }
}

