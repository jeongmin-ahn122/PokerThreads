/* Authors: Jeongmin Ahn and Alexander Alsabery
* Date: May 2, 2019
* Casino Final Project
*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#import "game.c"


int main (int argc, char *argv[])
{
	int numTotalPlayers, playerId, loserFound, finish;
	PLAYER* table;
	CARD* deck;

	printf("LET'S PLAY BLACKJACK!!!!\n");
		#pragma omp parallel private(numTotalPlayers, playerId, loserFound, finish)
		{
			playerId = omp_get_thread_num();
			#pragma omp master
			{
				numTotalPlayers = omp_get_num_threads(); //total number of threads (players)
				table = (PLAYER*)calloc(numTotalPlayers, sizeof(PLAYER)*numTotalPlayers);
				deck = generateCards(numTotalPlayers);
				shuffle(deck);
			}
			#pragma omp barrier
			{
				table[playerId] = createPlayer(playerId);
				#pragma omp barrier
				{
					while(table[playerId].bank > 0)
					{
						startDeal(deck, &table[playerId]);
						#pragma omp barrier
						{
							CARD deal = hit(deck);
							printf("Player %d was dealt %c of %c\n", table[playerId].id, deal.face, deal.suit);
							switch (table[playerId].personality)
							{
								case 'a':
									if (table[playerId].value < 17 || table[playerId].finish == 0)
										{
											bet(&table[playerId]);
											table[playerId].value += deal.value;
											if (table[playerId].value > GOAL && table[playerId].numOfAce != 0)
											{
												table[playerId].value -= 10;
											}
											printf("Player %d was has a total value of %d\n", table[playerId].id, table[playerId].value);
										}
									else
									{	
										printf("Player %d final value is %d\n", table[playerId].id, table[playerId].value);
										table[playerId].finish = 1;
									}
									break;
								case 'c':
									if(table[playerId].value != GOAL|| table[playerId].finish == 0)
									{
									bet(&table[playerId]);
									table[playerId].value +=deal.value;
										if (table[playerId].value > GOAL && table[playerId].numOfAce != 0)
										{
											table[playerId].value -= 10;
										}
										printf("Player %d was has a total value of %d\n", table[playerId].id, table[playerId].value);
									}
									else
									{	
										printf("Player %d final value is %d\n", table[playerId].id, table[playerId].value);
										table[playerId].finish = 1;
									}
									break;
								case 'i':
									if (table[playerId].bank != 0|| table[playerId].finish == 0)
									{
										bet(&table[playerId]);
										table[playerId].value +=deal.value;
										if (table[playerId].value > GOAL && table[playerId].numOfAce != 0)
										{
											table[playerId].value -= 10;
										}
										printf("Player %d was has a total value of %d\n", table[playerId].id, table[playerId].value);
									}
									else
									{	
										printf("Player %d final value is %d\n", table[playerId].id, table[playerId].value);
										table[playerId].finish = 1;
									}
									break;
								case 'p':
									if (table[playerId].value < 13|| table[playerId].finish == 0)
									{
										bet(&table[playerId]);
										table[playerId].value +=deal.value;
										if (table[playerId].value > GOAL && table[playerId].numOfAce != 0)
										{
											table[playerId].value -= 10;
										}
										printf("Player %d was has a total value of %d\n", table[playerId].id, table[playerId].value);
									}
									else
									{	
										printf("Player %d final value is %d\n", table[playerId].id, table[playerId].value);
										table[playerId].finish = 1;
									}
									break;
								default:
									break;
							}
							#pragma omp barrier
							{
								if (playerId != 0)
								{
									if (table[playerId].value <= GOAL)
									{
										if (table[playerId].value > table[0].value)
										{
											printf("Player %d has %d\n", table[playerId].id, table[playerId].value);
											table[playerId].bank += (table[playerId].pot + table[0].pot);
											table[playerId].pot = 0;
											table[playerId].value = 0;
											printf("Player %d has won! CONGRATULATIONS!!!!\nPlayer %d's bank now has $%d!\n",table[playerId].id, table[playerId].id, table[playerId].bank);
										}
										else
										{
											printf("Player %d has %d\n", table[playerId].id, table[playerId].value);
											table[playerId].pot = 0;
											table[playerId].value = 0;
											printf("Player %d lost. Fork over your money.\nPlayer %d's bank now has $%d.\n",table[playerId].id, table[playerId].id, table[playerId].bank);
											if(table[playerId].bank <= 0)
											{
												int random = rand() % 100 + 1;
												printf("Player %d is bankrupt. How unfortunate.\n",table[playerId].id);
												if (random < 11)
												{
													printf("Player %d flipped the table in RAGE!", table[playerId].id);
												}
												loserFound = 1;
											}
										}
									}
									else
									{
										table[playerId].pot = 0;
										table[playerId].value = 0;
										printf("Player %d BUSTED. Fork over your money.\nPlayer %d's bank now has $%d.\n",table[playerId].id, table[playerId].id, table[playerId].bank);
										if(table[playerId].bank <= 0)
										{
											int random = rand() % 100 + 1;
											printf("Player %d is bankrupt. How unfortunate.\n",table[playerId].id);
											if (random < 11)
											{
												printf("Player %d flipped the table in RAGE!", table[playerId].id);
											}
											loserFound = 1;
										}
									}
								}
							}
						}
					}
				}
			}
		marker = 0;
	}
	return 0;
}