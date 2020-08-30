/* $Id: rulestxt.c,v 1.5 2012-06-18 18:11:52 sverrehu Exp $ */
/**************************************************************************
 *
 *  FILE            rulestxt.c
 *  MODULE OF       The board game Mancala
 *
 *  DESCRIPTION     Compiled-in instructions used by the X-version.
 *
 *  WRITTEN BY      Sverre H. Huseby
 *
 **************************************************************************/

/**************************************************************************
 *                                                                        *
 *                        P U B L I C    D A T A                          *
 *                                                                        *
 **************************************************************************/

char *textRules =
"Note that there are lots of different rule sets for Mancala. Our\n"
"implementation uses this one:\n"
"\n"
"Setup: The board consists of 2 rows of 6 bins and 2 mancalas. Each\n"
"   player is allocated the row of bins closest to him and the mancala\n"
"   to his right. Initially each bin contains 4 stones, and the\n"
"   mancalas are empty.\n"
"\n"
"Object: The object of the game is to place as many stones as possible\n"
"   in one's mancala before the game ends. This happens when a player,\n"
"   at his turn, has no legal moves, meaning that all his holes are\n"
"   empty. The other player then gets to put any remaining stones on\n"
"   his side into his mancala.\n"
"\n"
"Moving: At his turn, the player picks up all the stones in any of his\n"
"   non-empty bins and starts to sow them by placing one in each of the\n"
"   preceding bins, starting with the one on the right and continuing\n"
"   counterclockwise around the board, skipping the oponents mancala\n"
"   (but not his own).\n"
"\n"
"Extra move: If the last stone is sown in one's mancala one gets to\n"
"   move again.\n"
"\n"
"Capturing: If the last stone is sown in an empty bin belonging to the\n"
"   player, he captures all the stones lying in the bin directly across\n"
"   this bin. These stones and the capturing stone are then placed in\n"
"   the mancala. If the bin directly across is empty, nothing special\n"
"   happens.\n"
"\n"
"@_\n"
"\n"
"Comments? Send mail to either of\n"
"   shh@thathost.com\n"
"   glennli@simula.no\n"
"\n"
;

