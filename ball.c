/**
    @file       ball.c
    @authors    Dylan White    29861039
                Alex Tompkins  79004580
                * TEAM334
    @date       7 Oct 2017
    @brief      Handles initialising and updating the ball.
    @note       Much of this module was heavily inspired by / borrows from the 
                bounce1 and boing modules provided to us in the apps and utils 
                directories respectively. These modules were created by Michael 
                P Hayes @ University of Canterbury and all credit for these is 
                due to him.
*/


#include "system.h"
#include "constants.h"
#include "ball.h"


/** 
    Updates the state of the given ball, changing its pos/dir to account for 
    bouncing off the edge of the screen and being redirected by the paddle.
    @param ball pointer to the Ball to update
    @param paddlePos the current col of the leftmost (closest to origin) part 
    of the paddle
    @return pointer to the Ball that was updated
*/
Ball* updateBall(Ball* ball, int paddlePos)
{
    // Within interaction range with paddle, determine redirection (if any)
    if (ball->pos.col == COL4) {
        if (ball->pos.row == paddlePos) {
            ball->dir.colInc = -ball->dir.colInc;
            ball->dir.rowInc = -1;
        } 
        else if (ball->pos.row == paddlePos + 1) {
            ball->dir.colInc = -ball->dir.colInc;
            ball->dir.rowInc = 0;
        } 
        else if (ball->pos.row == paddlePos + 2) {
            ball->dir.colInc = -ball->dir.colInc;
            ball->dir.rowInc = 1;
        }
    }

    // Apply movement
    ball->pos.col += ball->dir.colInc;
    ball->pos.row += ball->dir.rowInc;

    // Bounce off top & bottom edges
    if (ball->pos.row > ROW7 || ball->pos.row < ROW1) {
        ball->pos.row -= ball->dir.rowInc * 2;
        ball->dir.rowInc = -ball->dir.rowInc;
    }

    return ball;
}

/**
    Creates a Ball struct with the given pos & dir
    @return the new Ball struct that was created
*/
Ball initBall(int8_t col, int8_t row, int8_t colInc, int8_t rowInc)
{
    Ball ball;

    ball.pos.col = col;
    ball.pos.row = row;

    ball.dir.colInc = colInc;
    ball.dir.rowInc = rowInc;

    return ball;
}
