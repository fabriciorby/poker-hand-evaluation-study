#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include "poker.h"
#include "arrays.h"
#include "eval.c"
#include "fast_eval.c"

void print_hand( int *hand )
{
    int i, r;
    char suit;
    static char *rank = "23456789TJQKA";

    for ( i = 0; i < 5; i++ )
    {
        r = (*hand >> 8) & 0xF;
        if ( *hand & 0x8000 )
            suit = 'c';
        else if ( *hand & 0x4000 )
            suit = 'd';
        else if ( *hand & 0x2000 )
            suit = 'h';
        else
            suit = 's';

        printf( "%c%c ", rank[r], suit );
        hand++;
    }
}

int main()
{
    int deck[52], hand[5];
    int a, i, j, value1, value2;
    struct timeval inicio, final;
    int tmili;
    
    srand((unsigned) time(NULL));

    // initialize the deck
    init_deck( deck );
    
    shuffle_deck( deck );

    printf("Perfect Hashing Cactus Kev's\n");
    
    gettimeofday(&inicio, NULL);

    for(a = 0; a < 5; a++)
    {
        hand[a] = deck[a];
    }
    value1 = eval_5hand_fast( hand );
    j = hand_rank(value1);
    
    printf("\nMão do jogador 1: ");
    print_hand(hand);
    printf( "\n%s: %d\n", value_str[j], value1 );
    
    shuffle_deck( deck );
    
    for(a = 0; a < 5; a++)
    {
        hand[a] = deck[a];
    }
    value2 = eval_5hand_fast( hand );
    j = hand_rank(value2);
    
    printf("\nMão do jogador 2: ");
    print_hand(hand);
    printf( "\n%s: %d\n", value_str[j], value2 );
    
    if(value1 > value2)
        printf("\nO jogador 1 ganhou.");
    else
        printf("\nO jogador 2 ganhou.");
    
    printf("\n");
	
	gettimeofday(&final, NULL);
    tmili = (int) (1000 * (final.tv_sec - inicio.tv_sec) + (final.tv_usec - inicio.tv_usec) / 1000);
    
    printf("Tempo de execução: %d\n", tmili);
}