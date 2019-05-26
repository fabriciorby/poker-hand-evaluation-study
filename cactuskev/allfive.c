#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include "poker.h"
#include "arrays.h"
#include "eval.c"
#include "fast_eval.c"

/*************************************************/
/*                                               */
/* This code tests my evaluator, by looping over */
/* all 2,598,960 possible five card hands, cal-  */
/* culating each hand's distinct value, and dis- */
/* playing the frequency count of each hand type */
/*                                               */
/* Kevin L. Suffecool, 2001                      */
/* suffecool@bigfoot.com                         */
/*                                               */
/*************************************************/

int main()
{
    int deck[52], hand[5], freq[10];
    int a, b, c, d, e, i, j, h;
    struct timeval inicio, final;
    int tmili;
    
    srand((unsigned) time(NULL));

    // initialize the deck
    init_deck( deck );
    
    shuffle_deck( deck );

    printf("Cactus Kev's com Binary Search\n");
    
    // zero out the frequency array
    for ( i = 0; i < 10; i++ )
        freq[i] = 0;

    gettimeofday(&inicio, NULL);

    // loop over every possible five-card hand
    h = 0;
    for(a=0;a<48;a++)
    {
	hand[0] = deck[a];
	for(b=a+1;b<49;b++)
	{
	    hand[1] = deck[b];
	    for(c=b+1;c<50;c++)
	    {
		hand[2] = deck[c];
		for(d=c+1;d<51;d++)
		{
		    hand[3] = deck[d];
		    for(e=d+1;e<52;e++)
		    {
			hand[4] = deck[e];

			i = eval_5hand( hand );
			j = hand_rank(i);
			freq[j]++;
			h++;
		    }
		}
	    }
	}
    }

    for(i=1;i<=9;i++)
	printf( "%15s: %8d\n", value_str[i], freq[i] );
	
	gettimeofday(&final, NULL);
    tmili = (int) (1000 * (final.tv_sec - inicio.tv_sec) + (final.tv_usec - inicio.tv_usec) / 1000);
    
    printf("Tempo de execução: %d\n", tmili);
    printf("%d mãos\n", h);
    
    printf("\n");
    
    printf("Cactus Kev's com Perfect Hashing\n");
    
    // zero out the frequency array
    for ( i = 0; i < 10; i++ )
        freq[i] = 0;

    gettimeofday(&inicio, NULL);

    // loop over every possible five-card hand
    h = 0;
    for(a=0;a<48;a++)
    {
	hand[0] = deck[a];
	for(b=a+1;b<49;b++)
	{
	    hand[1] = deck[b];
	    for(c=b+1;c<50;c++)
	    {
		hand[2] = deck[c];
		for(d=c+1;d<51;d++)
		{
		    hand[3] = deck[d];
		    for(e=d+1;e<52;e++)
		    {
			hand[4] = deck[e];

			i = eval_5hand_fast( hand );
			j = hand_rank(i);
			freq[j]++;
			h++;
		    }
		}
	    }
	}
    }

    for(i=1;i<=9;i++)
	printf( "%15s: %8d\n", value_str[i], freq[i] );
	
	gettimeofday(&final, NULL);
    tmili = (int) (1000 * (final.tv_sec - inicio.tv_sec) + (final.tv_usec - inicio.tv_usec) / 1000);
    
    printf("Tempo de execução: %d\n", tmili);
    printf("%d mãos\n", h);
}
