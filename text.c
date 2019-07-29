/**
    @file       text.c
    @authors    Dylan White    29861039
                Alex Tompkins  79004580
                * TEAM334
    @date       7 Oct 2017
    @brief      Handles displaying generic text and formatted score summaries.
*/


#include "system.h"
#include "tinygl.h"
#include "text.h"


/**
	Displays the given text until the display is cleared.
	@param text the string of text to display
*/
void displayText(const char *text)
{
    tinygl_clear();
    tinygl_text(text);
}

/**
	Displays the current score of the game until the display is cleared.
	@param scores an int array of players' scores to display
	@param player an int representing which player controls this board (1 or 2)
*/
void displayScore(const int8_t scores[], const int8_t player)
{
    char* scoreLabel = "YOU:~ OPP:~";
    scoreLabel[4] = '0' + scores[player-1];
    scoreLabel[10] = '0' + scores[player % 2];
    displayText(scoreLabel);
}
