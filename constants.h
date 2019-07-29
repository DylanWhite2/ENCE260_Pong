/**
    @file       constants.h
    @authors    Dylan White    29861039
                Alex Tompkins  79004580
                * TEAM334
    @date       7 Oct 2017
    @brief      Contains game-wide constants used by multiple modules.
*/


#ifndef CONSTANTS_H_   
#define CONSTANTS_H_


/*
	Constants used to set LEDs ON or OFF.
*/
#define OFF 0
#define ON 1


/*
    Origin (COL1, ROW1) is upper-leftmost corner where the paddle is in the 
    rightmost col.
*/
enum {COL1, COL2, COL3, COL4, COL5};
enum {ROW1, ROW2, ROW3, ROW4, ROW5, ROW6, ROW7};


#endif // CONSTANTS_H_
