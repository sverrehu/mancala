/* $Id: textmain.c,v 1.2 2007-06-13 18:29:52 sverrehu Exp $ */
/**************************************************************************
 *
 *  FILE            textmain.c
 *  MODULE OF       The board game Mancala
 *
 *  DESCRIPTION     Main function / frontend for using a textbased display.
 *
 *  WRITTEN BY      Sverre H. Huseby
 *
 **************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "minimax.h"
#include "mancala.h"

/**************************************************************************
 *                                                                        *
 *                   P R I V A T E    F U N C T I O N S                   *
 *                                                                        *
 **************************************************************************/

/*-------------------------------------------------------------------------
 *
 *  NAME          showBoard
 *
 *  FUNCTION      Display the current board on screen.
 *
 *  DESCRIPTION   Very simple thing.
 */
static void showBoard(void)
{
    int q;

    printf("    ");
    for (q = 0; q < MAX_HOLES; q++)
	printf((q < MAX_HOLES - 1) ? "%c:%2d | " : "%c:%2d\n",
	       'a' + MAX_HOLES - q - 1, getHole(1, q));
    printf(" %2d ", getMancala(1));
    for (q = 0; q < MAX_HOLES; q++)
	printf((q < MAX_HOLES - 1) ? "-----+-" : "----");
    printf(" %2d\n", getMancala(0));
    printf("    ");
    for (q = 0; q < MAX_HOLES; q++)
	printf((q < MAX_HOLES - 1) ? "%c:%2d | " : "%c:%2d\n",
	       'a' + q, getHole(0, MAX_HOLES - q - 1));
}



/**************************************************************************
 *                                                                        *
 *                    P U B L I C    F U N C T I O N S                    *
 *                                                                        *
 **************************************************************************/

int main(int argc, char *argv[])
{
    char   s[80];
    char   playerName[2][20] = { "player at bottom", "player at top" };
    int    maxPly[2] = { MAX_DEPTH, MAX_DEPTH }, status;
    int    stones_pr_hole = STONES_PR_HOLE;
    PMove  move;
    Move   move2;
    Player player, winner;
    
    if (argc == 2)
	maxPly[0] = maxPly[1] = atoi(argv[1]);
    else if (argc == 3) {
	maxPly[1] = atoi(argv[1]);
	maxPly[0] = atoi(argv[2]);
    } else if (argc == 4) {
	maxPly[1] = atoi(argv[1]);
	maxPly[0] = atoi(argv[2]);
	stones_pr_hole = atoi(argv[3]);
    } else {
	printf("usage: mancala max-ply\n"
	       "       mancala max-ply-top max-ply-bottom\n"
	       "       mancala max-ply-top max-ply-bottom number-of-stones\n"
	       "\n"
	       "max-ply of 0 indicates human user.\n"
	       "Player at top starts.\n"
	       "\n");
	return 1;
    }
    if (stones_pr_hole < 1) {
        fprintf(stderr,
                "mancala: the number of stones per hole must be greater "
                "than zero\n");
        return 1;
    }
    if (maxPly[0] < 0 || maxPly[1] < 0) {
        fprintf(stderr,
                "mancala: max-ply must be no less than zero\n");
        return 1;
    }
    initGame(stones_pr_hole);
    player = 1;
    for (;;) {
	showBoard();
	printf("\n");
	if ((status = checkAndFixWin(&winner)) != 0) {
	    printf("=== Game over; ");
	    if (status == 1)
		printf("%s wins", playerName[winner]);
	    else
		printf("we have a draw");
	    printf(" ===\n\n");
	    showBoard();
	    printf("\n");
	    return 0;
	}
	printf("Move for %s: ", playerName[player]);
	if (!maxPly[player]) {
	    move = &move2;
	    for (;;) {
		if (fgets(s, 80, stdin) == NULL) {
                    printf("\nBye.\n");
                    return 0;
                }
		move->hole = 'A' + (MAX_HOLES - 1) - toupper(s[0]);
		if (legalMove(player, move))
		    break;
		printf("not a legal move. try again: ");
	    }
	    printf("\n");
	} else {
	    move = getBestMove(player, maxPly[player]);
	    printf("%c\n\n", 'A' + (MAX_HOLES - 1) - move->hole);
	}
	player = doMove(player, move);
    }
    return 0;
}
