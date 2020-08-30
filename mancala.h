/* $Id: mancala.h,v 1.2 2007-06-13 18:29:52 sverrehu Exp $ */
#ifndef MANCALA_H
#define MANCALA_H

/*
 *  Mancala board setup. According to tradition, there are 6 small holes
 *  and 4 stones pr hole initially.
 */
#define MAX_HOLES  6
#define STONES_PR_HOLE 4

/*
 *  We use board stack position 0 as the `real' board, so we need a stack
 *  that is one level deeper than the max recursion depth.
 */
#define MAX_DEPTH  20
#define STACK_LEVELS (MAX_DEPTH + 1)

/*
 *  A Move is identified by a hole number.
 */
typedef struct Move {
    int hole;
} Move;

/*
 *  The Board is described by the number of stones in each of the small
 *  holes, and in the mancalas (larger holes). In this setup, hole[x][0]
 *  is the hole closest to the mancala[x] for player x.
 */
typedef char StoneCount;
typedef struct Board {
    StoneCount hole[2][MAX_HOLES];
    StoneCount mancala[2];
} Board;

void       initGame(int stones_pr_hole);
StoneCount getHole(Player player, int hole);
StoneCount getMancala(Player player);
int        legalMove(Player player, PMove move);
int        checkAndFixWin(Player *winner);
PMove      getBestMove(Player player, int maxPly);
Player     doMove(Player player, PMove move);

#endif
