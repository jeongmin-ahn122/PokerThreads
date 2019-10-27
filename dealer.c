#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int DECKSIZE = 52;
int marker = 0;

typedef struct Card
{
	char suit;
	char face;
	int value;
}CARD;


CARD hit(CARD* deck)
{
	//deals a card to a player
	CARD dealt = deck[marker];
	marker++;
	return dealt;
}


CARD* generateCards(int numOfPlayers)
{
	//populates an array with cards
	int numOfDecks = 1;
	if (numOfPlayers/7 >= 1 && numOfPlayers%7 != 0)
	{
		for(int i = 0; i < numOfPlayers/7; i++)
		{
			numOfDecks++;
		}
	}
	CARD* deck = (CARD*)malloc((DECKSIZE * sizeof(CARD)) * numOfDecks);

	for(int j = 0; j < numOfDecks; j++)
	{
		for (int i = 0; i < DECKSIZE; i++)
		{
			CARD* card = (CARD*) calloc(1, sizeof(CARD));
			
			int suitAssign = i % 4;
			int numAssign = i % 13;

			//This assigns a suit to the card
			switch (suitAssign)
			{
				case 0:
					card->suit = 's';
					break;
				case 1:
					card->suit = 'd';
					break;
				case 2:
					card->suit = 'c';
					break;
				case 3:
					card->suit = 'h';
					break;
				default:
					break;
			}

			//This assigns the value and face if needed to the card.
			switch (numAssign)
			{
				case 0:
					card->face = 'a';
					card->value = 11;
					break;
				case 1:
					card->face = '2';
					card->value = 2;
					break;
				case 2:
					card->face = '3';
					card->value = 3;
					break;
				case 3:
					card->face = '4';
					card->value = 4;
					break;
				case 4:
					card->face = '5';
					card->value = 5;
					break;
				case 5:
					card->face = '6';
					card->value = 6;
					break;
				case 6:
					card->face = '7';
					card->value = 7;
					break;
				case 7:
					card->face = '8';
					card->value = 8;
					break;
				case 8:
					card->face = '9';
					card->value = 9;
					break;
				case 9:
					card->face = 't';
					card->value = 10;
					break;
				case 10:
					card->face = 'j';
					card->value = 10;
					break;
				case 11:
					card->face = 'q';
					card->value = 10;
					break;
				case 12:
					card->face = 'k';
					card->value = 10;
					break;
				default:
					break;
			}
			deck[i] = *card;
		}
	}
	return deck;
}

CARD* shuffle(CARD* deck)
{
	//shuffles the deck
	srand(time(NULL));

	for (int i = DECKSIZE - 1; i > 0; i--)
	{
		int index = rand() % (i + 1);

		//printf("%d\n",index);

		CARD temp = deck[i];
		deck[i] = deck[index];
		deck[index] = temp;
	}

/*
	for (int j = 0; j < DECKSIZE; j++)
	{
		printf("Card value: %d Card suit: %c \n", deck[j].value, deck[j].suit);
	}
	*/
	return deck;
}
