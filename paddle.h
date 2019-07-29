/**
    @file       paddle.h
    @authors    Dylan White    29861039
                Alex Tompkins  79004580
                * TEAM334
    @date       7 Oct 2017
    @brief      Handles initialising, updating and displaying the paddle.
*/


#ifndef PADDLE_H_   
#define PADDLE_H_


#define DEFAULT_POS ROW3


/**
    Creates a new paddle with default position, displays it, and returns its 
    position.
    @return the row that the topmost part of the new paddle occupies
*/
int8_t initPaddle(void);

/**
    Updates the paddle's position based on navswitch position, displays it, and
    returns its positon.
    @param the row that the topmost part of the paddle occupies before updating
    @return the row that the topmost part of the paddle occupies after updating
*/
int8_t updatePaddle(int8_t paddlePos);


#endif // PADDLE_H_
