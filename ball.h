/**
    @file       ball.h
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


#ifndef BALL_H_   
#define BALL_H_


/** 
    Structure defining the position of a ball.
    col: index of the col the ball is in [COL1, COL5]
    row: index of the row the ball is in [ROW1, ROW7]
*/
typedef struct ball_pos_s
{
    int8_t col;
    int8_t row;
} BallPos;

/**
    Structure defining the direction/velocity of a ball
    colInc: the vertical velocity of the ball {-1, 0, 1}
    rowInc: the horizontal velocity of the ball {-1, 1}
*/
typedef struct ball_dir_s
{
    int8_t colInc;
    int8_t rowInc;
} BallDir;

/** 
    Structure defining the state of a ball. 
    pos: the current position of the ball.
    dir: the current direction of the ball.
*/
typedef struct ball_s
{
    BallPos pos;
    BallDir dir;
} Ball;


/** 
    Updates the state of the given ball, changing its pos/dir to account for 
    bouncing off the edge of the screen and being redirected by the paddle.
    @param ball pointer to the Ball to update
    @param paddlePos the current col of the leftmost (closest to origin) part 
    of the paddle
    @return pointer to the Ball that was updated
*/
Ball* updateBall(Ball* ball, int paddlePos);

/**
    Creates a Ball struct with the given pos & dir
    @return the new Ball struct that was created
*/
Ball initBall(int8_t col, int8_t row, int8_t colInc, int8_t rowInc);


#endif // BALL_H_
