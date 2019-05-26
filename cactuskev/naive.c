// ESTE CÓDIGO É PARA CLASSIFICAR TODAS AS MÃOS DE PÔQUER

/*********************************************************
 * From C PROGRAMMING: A MODERN APPROACH, Second Edition *
 * By K. N. King                                         *
 * Copyright (c) 2008, 1996 W. W. Norton & Company, Inc. *
 * All rights reserved.                                  *
 * This program may be freely distributed for class use, *
 * provided that this copyright notice is retained.      *
 *********************************************************/

/* poker.c (Chapter 10, page 233) */
/* Classifies a poker hand */

#include <stdbool.h>   /* C99 only */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define NUM_RANKS 13
#define NUM_SUITS 4
#define NUM_CARDS 5

/* external variables */
int num_in_rank[NUM_RANKS];
int num_in_suit[NUM_SUITS];
int categorias[9];
bool straight, flush, four, three;
int pairs;   /* can be 0, 1, or 2 */

/* prototypes */
void read_cards(int *hand);
void init_deck(int *deck);
void sum_result(void);
void analyze_hand(void);
void print_result(void);
void shuffle_deck(int *deck);

void init_deck( int *deck )
{
    int i, j, n = 0, suit = 0x8000;
    int primes[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41 };

    for ( i = 0; i < 4; i++, suit >>= 1 )
        for ( j = 0; j < 13; j++, n++ )
            deck[n] = primes[j] | (j << 8) | suit | (1 << (16+j));
}

void shuffle_deck( int *deck )
{
    int i, n, temp[52];

    for ( i = 0; i < 52; i++ )
        temp[i] = deck[i];

    for ( i = 0; i < 52; i++ )
    {
        do {
            n = rand() % 52;
        } while ( temp[n] == 0 );
        deck[i] = temp[n];
        temp[n] = 0;
    }
}

void sortCards(int *hand, int *sorted){
  int i, j, tmp;
  for(i = 0; i < 5; i++)
  {
    sorted[i]=hand[i];
  }
  for(i = 0; i < 5; i++)
  {
   for(j = i+1; j < 5; j++)
   {
     if((sorted[j]&0xF00)>>8 < (sorted[i]&0xF00)>>8)
     {
       tmp = sorted[i];
       sorted[i] = sorted[j];
       sorted[j] = tmp;
     }
   }
  }
}

/**********************************************************
 * read_cards: Reads the cards into the external          *
 *             variables num_in_rank and num_in_suit;     *
 *             checks for bad cards and duplicate cards.  *
 **********************************************************/
void read_cards(int *hand)
{
  bool card_exists[NUM_RANKS][NUM_SUITS];
  char ch, rank_ch, suit_ch;
  int rank, suit;
  bool bad_card;
  int cards_read = 0;

    int i, r;
    static char *rank_array = "23456789TJQKA";

  for (rank = 0; rank < NUM_RANKS; rank++) {
    num_in_rank[rank] = 0;
    for (suit = 0; suit < NUM_SUITS; suit++)
      card_exists[rank][suit] = false;
  }

  for (suit = 0; suit < NUM_SUITS; suit++)
    num_in_suit[suit] = 0;

  while (cards_read < NUM_CARDS) {
    bad_card = false;

    rank_ch = rank_array[(*hand >> 8) & 0xF];
    switch (rank_ch) {
      case '0':           exit(EXIT_SUCCESS);
      case '2':           rank = 0; break;
      case '3':           rank = 1; break;
      case '4':           rank = 2; break;
      case '5':           rank = 3; break;
      case '6':           rank = 4; break;
      case '7':           rank = 5; break;
      case '8':           rank = 6; break;
      case '9':           rank = 7; break;
      case 't': case 'T': rank = 8; break;
      case 'j': case 'J': rank = 9; break;
      case 'q': case 'Q': rank = 10; break;
      case 'k': case 'K': rank = 11; break;
      case 'a': case 'A': rank = 12; break;
      default:            bad_card = true;
    }

    if ( *hand & 0x8000 )
            suit_ch = 'c';
        else if ( *hand & 0x4000 )
            suit_ch = 'd';
        else if ( *hand & 0x2000 )
            suit_ch = 'h';
        else
            suit_ch = 's';

    switch (suit_ch) {
      case 'c': case 'C': suit = 0; break;
      case 'd': case 'D': suit = 1; break;
      case 'h': case 'H': suit = 2; break;
      case 's': case 'S': suit = 3; break;
      default:            bad_card = true;
    }
      if (bad_card)
      printf("Bad card; ignored.\n");
    else if (card_exists[rank][suit])
      printf("Duplicate card; ignored.\n");
    else {
      num_in_rank[rank]++;
      num_in_suit[suit]++;
      card_exists[rank][suit] = true;
      cards_read++;
      hand++;
    }
  }
}

/**********************************************************
 * analyze_hand: Determines whether the hand contains a   *
 *               straight, a flush, four-of-a-kind,       *
 *               and/or three-of-a-kind; determines the   *
 *               number of pairs; stores the results into *
 *               the external variables straight, flush,  *
 *               four, three, and pairs.                  *
 **********************************************************/
void analyze_hand()
{
  int num_consec = 0;
  int rank, suit;

  straight = false;
  flush = false;
  four = false;
  three = false;
  pairs = 0;

  /* check for flush */
  for (suit = 0; suit < NUM_SUITS; suit++)
    if (num_in_suit[suit] == NUM_CARDS)
      flush = true;

  /* check for straight */
  rank = 0;
  while (num_in_rank[rank] == 0) rank++;
  for (; rank < NUM_RANKS && num_in_rank[rank] > 0; rank++)
    num_consec++;
  if (num_consec == NUM_CARDS) {
    straight = true;
    return;
  }

  /* check for 4-of-a-kind, 3-of-a-kind, and pairs */
  for (rank = 0; rank < NUM_RANKS; rank++) {
    if (num_in_rank[rank] == 4) four = true;
    if (num_in_rank[rank] == 3) three = true;
    if (num_in_rank[rank] == 2) pairs++;
  }
}

/**********************************************************
 * print_result: Prints the classification of the hand,   *
 *               based on the values of the external      *
 *               variables straight, flush, four, three,  *
 *               and pairs.                               *
 **********************************************************/
void print_result(void)
{
  printf("Straight flush: %d\n", categorias[0]);
  printf("Four of a kind: %d\n", categorias[1]);
  printf("Full house: %d\n", categorias[2]);
  printf("Flush: %d\n", categorias[3]);
  printf("Straight: %d\n", categorias[4]);
  printf("Three of a kind: %d\n", categorias[5]);
  printf("Two pairs: %d\n", categorias[6]);
  printf("Pair: %d\n", categorias[7]);
  printf("High card: %d\n", categorias[8]);

  printf("\n");
}

void sum_result(void)
{
  if (straight && flush) categorias[0]++; //printf("Straight flush");
  else if (four)         categorias[1]++; //printf("Four of a kind");
  else if (three &&
           pairs == 1)   categorias[2]++; //printf("Full house");
  else if (flush)        categorias[3]++; //printf("Flush");
  else if (straight)     categorias[4]++; //printf("Straight");
  else if (three)        categorias[5]++; //printf("Three of a kind");
  else if (pairs == 2)   categorias[6]++; //printf("Two pairs");
  else if (pairs == 1)   categorias[7]++; //printf("Pair");
  else                   categorias[8]++; //printf("High card");
}

void naive(int* hand){
  int sorted[5];
  sortCards(hand, sorted);
  read_cards(sorted);
  analyze_hand();
  sum_result();
}

/**********************************************************
 * main: Calls read_cards, analyze_hand, and print_result *
 *       repeatedly.                                      *
 **********************************************************/
int main(void)
{
  int deck[52], hand[5], hand_ch, suit_ch;
  int a, b, c, d, e, i, j, tmp;
  double h = 0;
  struct timeval inicio, final;
  int tmili;

  srand((unsigned) time(NULL));

  for(a = 0; a < 9; a++)
  {
      categorias[a] = 0;
  }

  init_deck(deck);

  shuffle_deck(deck);

  gettimeofday(&inicio, NULL);

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
            naive(hand);
          }
        }
      }
    }
  }

  gettimeofday(&final, NULL);
  tmili = (int) (1000 * (final.tv_sec - inicio.tv_sec) + (final.tv_usec - inicio.tv_usec) / 1000);

  print_result();
  printf("Tempo de execução: %d\n", tmili);

  for(i = 0; i < 9; i++)
  {
    h+= categorias[i];
  }
  printf("%.0f mãos\n", h);
}
