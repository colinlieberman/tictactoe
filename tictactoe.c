#include "tictactoe.h"
#include "menu.h"

void collect_string( char *str, int max_len ) {
    
    scanf( "%s", str );

    if( strlen( str ) >= max_len ) {
        printf( "\nSorry! That's more than I can handle. Try again next time!\n" );
        exit( 255 );
    }
}

void clear_stdin( void ) {
    /* http://stackoverflow.com/questions/14099473/how-to-scanf-only-integer-and-repeat-reading-if-the-user-enter-non-numeric-chara */
    while (getchar()!='\n');
}

int main( void ) {
    
    show_welcome();

    do_menu();

    return 0;
}
