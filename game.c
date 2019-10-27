#import "dealer.c"

int START = 50; //starting pot for new players.
int GOAL = 21;

typedef struct player
{
	int pot;
	int bank;
	int value; //total value of hand
	int numOfAce;
	int id;
	char personality;
	int finish;
}PLAYER;

typedef struct dealer
{
	CARD* deck;
}DEALER;

PLAYER createPlayer(int id)
{	
	PLAYER* player = (PLAYER*)calloc(1, sizeof(PLAYER));
	player->bank = START;
	player->id = id;
	int random = rand() % 100 + 1;
	if(random < 6)
	{
		player->personality = 'i';
	}
	else if (random >= 6 && random < 27)
	{
		player->personality = 'c';
	}
	else if (random >= 27 && random < 48)
	{
		player->personality = 'p';
	}
	else
	{
		player->personality = 'a';
	}
	printf("Player %d has been made\n", player->id);
	
	return *player;
}

void startDeal(CARD* deck, PLAYER* player)
{
	CARD dealtCard = hit(deck); //first card
	printf("Player %d was dealt %c of %c\n", player->id, dealtCard.face, dealtCard.suit);
	player->value += dealtCard.value;
	if (player->value == 11)
	{
		player->numOfAce++;
	}
	dealtCard = hit(deck); //second card
	printf("Player %d was dealt %c of %c\n", player->id, dealtCard.face, dealtCard.suit);
	player->value += dealtCard.value;
	if (player->value > GOAL)
	{
		player->value -= 10;
	}
}
/*
PLAYER* table(int numOfPlayer)
{
	PLAYER* table = (PLAYER*)calloc(numOfPlayer, sizeof(PLAYER) * (numOfPlayer));

	for (int i = 0; i < numOfPlayer; i++)
	{
		table[i] = createPlayer();
	}
	return table;
}
*/
void bet(PLAYER* player)
{
	int bet = 10; //fixed bet value
	player->bank -= bet;
	player->pot += bet;
	printf("Player %d bet $%d Current player's pot: %d\n", player->id, bet, player->pot);
}
/*
void game(PLAYER* table, int numOfPlayer)
{
	if (numOfPlayer == 0)
	{
		printf("THERE ARE NO PLAYERS!\n");
	}
	else
	{
		int finish = 0;
		int loserFound = 0;
		printf("Starting new round....\nA round will continue until one player goes bankrupt.\n");
		CARD* deck = generateCards(numOfPlayer);
		shuffle(deck);
		while(loserFound == 0)
		{
			printf("\nStarting game.\nDealing cards.\n");
			for (int i = 0; i < numOfPlayer; i++)
			{
				startDeal(deck, &table[i]);
			}
			printf("All players have been dealt cards.\nBeginning the bets.\n");
			while(finish < numOfPlayer)//this loop will go until all players do not want anymore cards.
			{
				for (int i = 0; i < numOfPlayer; i++)
				{
					CARD deal = hit(deck);
					printf("Will Player %d with personality %c bet?\n", table[i].id, table[i].personality);
					switch (table[i].personality)
					{
						case 'a':
							if (table[i].value < 17)
								{
									bet(&table[i]);
									table[i].value += deal.value;
									if (table[i].value > GOAL && table[i].numOfAce != 0)
									{
										table[i].value -= 10;
									}
									printf("Player %d was has a total value of %d\n", table[i].id, table[i].value);
								}
							else
							{	
								printf("Player %d final value is %d\n", table[i].id, table[i].value);
								finish++;
							}
							break;
						case 'c':
							if(table[i].value != GOAL)
							{
							bet(&table[i]);
							table[i].value +=deal.value;
								if (table[i].value > GOAL && table[i].numOfAce != 0)
								{
									table[i].value -= 10;
								}
								printf("Player %d was has a total value of %d\n", table[i].id, table[i].value);
							}
							else
							{	
								printf("Player %d final value is %d\n", table[i].id, table[i].value);
								finish++;
							}
							break;
						case 'i':
							if (table[i].bank != 0)
							{
								bet(&table[i]);
								table[i].value +=deal.value;
								if (table[i].value > GOAL && table[i].numOfAce != 0)
								{
									table[i].value -= 10;
								}
								printf("Player %d was has a total value of %d\n", table[i].id, table[i].value);
							}
							else
							{	
								printf("Player %d final value is %d\n", table[i].id, table[i].value);
								finish++;
							}
							break;
						case 'p':
							if (table[i].value < 13)
							{
								bet(&table[i]);
								table[i].value +=deal.value;
								if (table[i].value > GOAL && table[i].numOfAce != 0)
								{
									table[i].value -= 10;
								}
								printf("Player %d was has a total value of %d\n", table[i].id, table[i].value);
							}
							else
							{	
								printf("Player %d final value is %d\n", table[i].id, table[i].value);
								finish++;
							}
							break;
						default:
							break;
					}
				}
			}
			finish = 0;	
			//calculating winnings and resetting hands for new round
			printf("\nCalculating winnings.\n");
			for (int i = 1; i < numOfPlayer; i++)
			{
				printf("Dealer has %d\n",table[0].value);
				if (table[i].value <= GOAL)
				{
					if (table[i].value > table[0].value)
					{
						printf("Player %d has %d\n", table[i].id, table[i].value);
						table[i].bank += (table[i].pot + table[0].pot);
						table[i].pot = 0;
						table[i].value = 0;
						printf("Player %d has won! CONGRATULATIONS!!!!\nPlayer %d's bank now has $%d!\n",table[i].id, table[i].id, table[i].bank);
					}
					else
					{
						printf("Player %d has %d\n", table[i].id, table[i].value);
						table[i].pot = 0;
						table[i].value = 0;
						printf("Player %d lost. Fork over your money.\nPlayer %d's bank now has $%d.\n",table[i].id, table[i].id, table[i].bank);
						if(table[i].bank <= 0)
						{
							printf("Player %d is bankrupt. How unfortunate.\n",table[i].id);
							loserFound = 1;
						}
					}
				}
				else
				{
					table[i].pot = 0;
					table[i].value = 0;
					printf("Player %d BUSTED. Fork over your money.\nPlayer %d's bank now has $%d.\n",table[i].id, table[i].id, table[i].bank);
					if(table[i].bank <= 0)
						{
							printf("Player %d is bankrupt. How unfortunate.\n",table[i].id);
							loserFound = 1;
						}
				}
				//resets the dealer's hand and pot. The dealer will never run out of money.
			}
			table[0].pot = 0;
			table[0].value = 0;
			//resets the marker in the hit method such that when a new deck is created, the deck will draw cards from the top of the deck once again.
			marker = 0;
			//reshuffle deck for replay.
			shuffle(deck);
		}
	}
}
*/