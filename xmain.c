/* $Id: xmain.c,v 1.7 2012-06-18 18:07:40 sverrehu Exp $ */
/**************************************************************************
 *
 *  FILE            xmain.c
 *  MODULE OF       The board game Mancala
 *
 *  DESCRIPTION     Main function / frontend for using an X/XForms-based
 *                  display.
 *
 *                  This code is _really_ (I mean: REALLY) dirty...
 *                  The main purpose of this is to make someone play the
 *                  game. For those interested in the gaming (as I am),
 *                  please take a look at the other sourcefiles instead.
 *                  I'm not at all proud of this file, it's my first
 *                  attempt on using the XForms library, and it was done
 *                  in a hurry.
 *
 *                  I don't like event driven programming...
 *
 *  WRITTEN BY      Sverre H. Huseby
 *
 **************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <forms.h>

#include "minimax.h"
#include "mancala.h"
#include "xform.h"

#define flushForms fl_check_only_forms

extern char *textRules;

/**************************************************************************
 *                                                                        *
 *                       P R I V A T E    D A T A                         *
 *                                                                        *
 **************************************************************************/

static FD_mancala *frm;
static FD_rules   *frmRules;
static FL_OBJECT  *frmMancala[2];
static FL_OBJECT  *frmHole[2][MAX_HOLES];
static FL_OBJECT  *frmLight[2][MAX_HOLES];
static const char *playerName[2] = { "the human player", "the computer" };
static int        maxPly[2] = { 0, 4 };
static int        rulesDisplayed = 0;
static int        stones_pr_hole = STONES_PR_HOLE;

/*
 *  Player side and hole chosen by human player.
 */
static Player movePlayer;
static int    moveHole = -1;

static Player player;
static int    status;


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
    Player p;
    int    q;
    char   s[10];

    fl_freeze_form(frm->mancala);
    for (p = 0; p < 2; p++) {
	sprintf(s, "%d", getMancala(p));
	fl_set_object_label(frmMancala[p], s);
	for (q = 0; q < MAX_HOLES; q++) {
	    sprintf(s, "%d", getHole(p, q));
	    fl_set_object_label(frmHole[p][q], s);
	}
    }
    fl_unfreeze_form(frm->mancala);
    flushForms();
}

/*-------------------------------------------------------------------------
 *
 *  NAME          setMessage
 *
 *  FUNCTION      Set the message of the status line.
 *
 *  INPUT         format, ...
 *                        Arguments used as with printf().
 */
static void setMessage(const char *format, ...)
{
    va_list ap;
    char    s[120];

    va_start(ap, format);
    vsprintf(s, format, ap);
    va_end(ap);
    fl_set_object_label(frm->textStatus, s);
    flushForms();
}

/* set hole to -1 to turn all off */
static void setLastMove(Player player, int hole)
{
    static Player   lastPlayer;
    static int      lastHole = -1;
#ifdef WORKS_ON_SGI_NOW
    static int      colorSet = 0;
#endif
    static FL_COLOR origCol1 = FL_COL1, origCol2 = FL_BOTTOM_BCOL;

    /*
     *  For some reason, SGI fucks up with segmenation fault during
     *  the next few lines. Harcoding it instead...
     */
#ifdef WORKS_ON_SGI_NOW
    if (!colorSet) {
	origCol1 = frmLight[player][hole]->col1;
	origCol2 = frmLight[player][hole]->col2;
	colorSet = 1;
    }
#endif
    if (lastHole >= 0)
	fl_set_object_color(frmLight[lastPlayer][lastHole],
			    origCol1, origCol2);
    if (hole >= 0)
	fl_set_object_color(frmLight[player][hole],
			    FL_RED, FL_BOTTOM_BCOL);
    lastPlayer = player;
    lastHole = hole;
    flushForms();
}

static void flashMove(Player player, int hole)
{
    int q;

    setLastMove(0, -1);
    for (q = 5; q > 0; q--) {
	setLastMove(player, hole);
	fl_msleep(100);
	setLastMove(0, -1);
	fl_msleep(100);
    }
}

static Player computerPlayer(void)
{
    if (maxPly[0])
	return 0;
    if (maxPly[1])
	return 1;
    return -1;
}

static void formInit(void)
{
    char s[20];
    
    frm = create_form_mancala();
    sprintf(s, "version %s", VERSION);
    fl_set_object_label(frm->textVer, s);
    frmMancala[0] = frm->mancalaB;
    frmHole[0][0] = frm->holeB0;
    frmHole[0][1] = frm->holeB1;
    frmHole[0][2] = frm->holeB2;
    frmHole[0][3] = frm->holeB3;
    frmHole[0][4] = frm->holeB4;
    frmHole[0][5] = frm->holeB5;
    frmMancala[1] = frm->mancalaT;
    frmHole[1][0] = frm->holeT0;
    frmHole[1][1] = frm->holeT1;
    frmHole[1][2] = frm->holeT2;
    frmHole[1][3] = frm->holeT3;
    frmHole[1][4] = frm->holeT4;
    frmHole[1][5] = frm->holeT5;
    frmLight[0][0] = frm->lightB0;
    frmLight[0][1] = frm->lightB1;
    frmLight[0][2] = frm->lightB2;
    frmLight[0][3] = frm->lightB3;
    frmLight[0][4] = frm->lightB4;
    frmLight[0][5] = frm->lightB5;
    frmLight[1][0] = frm->lightT0;
    frmLight[1][1] = frm->lightT1;
    frmLight[1][2] = frm->lightT2;
    frmLight[1][3] = frm->lightT3;
    frmLight[1][4] = frm->lightT4;
    frmLight[1][5] = frm->lightT5;
    fl_set_slider_bounds(frm->slidLevel, 1.0, 9.0);
    fl_set_slider_step(frm->slidLevel, 1.0);
    fl_set_slider_precision(frm->slidLevel, 0);
    /* HERE: change if not always human vs. computer */
    fl_set_slider_value(frm->slidLevel, maxPly[computerPlayer()]);

    frmRules = create_form_rules();
    fl_set_browser_fontsize(frmRules->textRules, FL_NORMAL_SIZE);
    fl_set_browser_fontstyle(frmRules->textRules, FL_NORMAL_STYLE);
    fl_add_browser_line(frmRules->textRules, textRules);
    
    fl_show_form(frm->mancala, FL_PLACE_SIZE, FL_FULLBORDER, "Mancala");
}

static void formFinish(void)
{
    fl_hide_form(frm->mancala);
    if (rulesDisplayed)
	fl_hide_form(frmRules->rules);
}

static void ourInitGame(Player starter)
{
    initGame(stones_pr_hole);
    player = starter;
    status = 0;
    fl_set_object_label(frm->playerB, playerName[0]);
    fl_set_object_label(frm->playerT, playerName[1]);
    fl_set_object_label(frm->startB, starter == 0 ? "Starter" : "");
    fl_set_object_label(frm->startT, starter == 1 ? "Starter" : "");
    fl_set_object_label(frm->winB, "");
    fl_set_object_label(frm->winT, "");
}

static void formHandler(void)
{
    PMove  move;
    Move   move2;
    Player winner;

    ourInitGame(0);
    for (;;) {
	showBoard();
	if (!status) {
	    if ((status = checkAndFixWin(&winner)) != 0) {
		if (status == 1) {
		    setMessage("Game over; %s wins", playerName[winner]);
		    fl_set_object_label(winner == 0 ? frm->winB : frm->winT,
					"Winner");
		} else
		    setMessage("Game over; we have a draw");
	    } else {
		if (!maxPly[player]) {
		    setMessage("It's %s's turn", playerName[player]);
		    fl_do_forms();
		    move = &move2;
		    if (moveHole >= 0) {
			if (movePlayer == player) {
			    move->hole = moveHole;
			    if (legalMove(player, move)) {
				flashMove(player, move->hole);
				player = doMove(player, move);
			    } else {
				setMessage("Not a legal move. Try again!");
				fl_msleep(1500);
			    }
			} else {
			    setMessage("Stick to your side, please!");
			    fl_msleep(1500);
			}
			moveHole = -1;
		    }
		} else {
		    setMessage("%s is thinking...", playerName[player]);
		    move = getBestMove(player, maxPly[player]);
		    setMessage("");
		    flashMove(player, move->hole);
		    player = doMove(player, move);
		}
	    }
	} else
	    fl_do_forms();
    }
}

/**************************************************************************
 *                                                                        *
 *                    P U B L I C    F U N C T I O N S                    *
 *                                                                        *
 **************************************************************************/

/*------------------------------------------------------------------------*
 |                Callbacks for the objects in the form                   |
 *------------------------------------------------------------------------*/

void doRules(FL_OBJECT *ob, long arg)
{
    if (rulesDisplayed)
	return;
    fl_show_form(frmRules->rules, FL_PLACE_FREE, FL_FULLBORDER,
		 "Mancala Rules");
    fl_deactivate_object(frm->buttRules);
    rulesDisplayed = 1;
}

void doRulesDone(FL_OBJECT *ob, long arg)
{
    if (!rulesDisplayed)
	return;
    fl_hide_form(frmRules->rules);
    fl_activate_object(frm->buttRules);
    rulesDisplayed = 0;
}

void doHole(FL_OBJECT *ob, long arg)
{
    Player player;
    int    hole;

    if (moveHole >= 0) {
	setMessage("Hold your horses! "
		   "I haven't done your previous move yet!");
	fl_msleep(1000);
    } else {
	player = arg / 100;
	hole = arg % 100;
	movePlayer = player;
	moveHole = hole;
    }
    fl_trigger_object(frm->dummyButton);
}

void doLevel(FL_OBJECT *ob, long arg)
{
    double v;
    Player comp;

    if ((comp = computerPlayer()) < 0)
	return;
    v = fl_get_slider_value(ob);
    maxPly[comp] = (int) v;
}

void doNewGame(FL_OBJECT *ob, long arg)
{
    ourInitGame(arg);
    fl_trigger_object(frm->dummyButton);
}

void doQuit(FL_OBJECT *ob, long arg)
{
    formFinish();
    exit(0);
}

/**************************************************************************/
#define stringify(a) #a
#define stringize(a) stringify(a)

int main(int argc, char *argv[])
{
    static FL_CMD_OPT cmdopt [] = {
        { "-stones", "*.stones", XrmoptionSepArg, 0 }};
    static FL_resource res [] = {
        { "stones", "XMancala", FL_INT, &stones_pr_hole,
          stringize(STONES_PR_HOLE), 0 }};
    fl_initialize(&argc, argv, "XMancala",
                  cmdopt, sizeof(cmdopt) / sizeof(FL_CMD_OPT));
    fl_get_app_resources(res, sizeof(res ) / sizeof(FL_resource));
    if (stones_pr_hole < 1) {
        fprintf(stderr,
                "xmancala: the number of stones per hole must be "
                "greater than zero\n");
        return 1;
    }
    formInit();
    formHandler();
    formFinish();
    return 0;
}
