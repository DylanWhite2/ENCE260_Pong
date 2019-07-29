/**
    @file       paddle.c
    @authors    Dylan White    29861039
                Alex Tompkins  79004580
                * TEAM334
    @date       7 Oct 2017
    @brief      Handles initialising, updating and displaying the paddle.
*/


#include "system.h"
#include "display.h"
#include "navswitch.h"
#include "constants.h"
#include "paddle.h"


/**
    Creates a new paddle with default position, displays it, and returns its 
    position.
    @return the row that the topmost part of the new paddle occupies
*/
int8_t initPaddle(void)
{
    int8_t paddlePos = DEFAULT_POS;
    display_pixel_set(COL5, paddlePos, ON);
    display_pixel_set(COL5, paddlePos + 1, ON);
    display_pixel_set(COL5, paddlePos + 2, ON);

    return paddlePos;
}

/**
    Updates the paddle's position based on navswitch position, displays it, and
    returns its positon.
    @param the row that the topmost part of the paddle occupies before updating
    @return the row that the topmost part of the paddle occupies after updating
*/
int8_t updatePaddle(int8_t paddlePos)
{
    if ((navswitch_push_event_p(NAVSWITCH_SOUTH)) && (paddlePos < ROW5)) {
        display_pixel_set(COL5, paddlePos, OFF);
        display_pixel_set(COL5, paddlePos + 3, ON);
        paddlePos++;
    }

    else if ((navswitch_push_event_p(NAVSWITCH_NORTH)) && (paddlePos > ROW1)) {
        display_pixel_set(COL5, paddlePos + 2, OFF);
        display_pixel_set(COL5, paddlePos - 1, ON);
        paddlePos--;
    }
    
    return paddlePos;
}
