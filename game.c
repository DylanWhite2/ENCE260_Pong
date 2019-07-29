/**
    @file       game.c
    @authors    Dylan White    29861039
                Alex Tompkins  79004580
                * TEAM334
    @date       7 Oct 2017
    @brief      Pong game for ENCE260 assignment. Designed to be played by two 
                UCFK4s using IR comms.
*/


#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "display.h"
#include "led.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "constants.h"
#include "paddle.h"
#include "ball.h"
#include "text.h"
#include "ir.h"
#include "../fonts/font5x7_1.h"


#define TEXT_SPEED 20
#define PACER_RATE 500
#define DELAY 100
#define TIME_DIVISOR 150
#define FONT &font5x7_1


enum {START_UP, PLAYER_SET, ROUND_ACTIVE, ROUND_END, GAME_OVER};
enum {STATUS_ACTIVE, STATUS_OPPONENT, STATUS_TRANSMIT, STATUS_LOST};


/**
    Sets all parameters to default values to reset the round. Also sets the 
    ball to its initial position and clears the display.
    @param ballActive bool representing whether the ball is in play
    @param ballOnSide bool representing whether the ball is on the player's side
    @param paddlePos int8_t storing the current position of the paddle
    @param ticks int storing the number of ticks since last update
    @param time int storing the number of ticks since the round began
*/
void startRound(bool* ballActive, bool* ballOnSide, int8_t* paddlePos, int* ticks, int* time) 
{
    tinygl_clear();
    *paddlePos = initPaddle();
    *ballActive = false;
    *ballOnSide = false;
    *ticks = 0;
    *time = 1;
}

/**
    Main game function.
*/
int main(void)
{
    // Declare and initialise variables
    int ticks = 0;
    int time = 1;
    uint8_t rcvd;
    int8_t mode = START_UP;
    int8_t player = 0;
    int8_t server = 1;
    int8_t paddlePos = -1;
    int8_t scores[] = {0, 0};
    Ball ball;
    bool ballActive = false;
    bool ballOnSide = false;

    // Initialise system, navswitch, IR, LED and pacer
    system_init();
    navswitch_init();
    ir_uart_init();
    led_init();
    pacer_init(PACER_RATE);

    // Initialise tinygl and set text options
    tinygl_init (PACER_RATE);
    tinygl_font_set(FONT);
    tinygl_text_speed_set(TEXT_SPEED);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);

    // Set LED1 off initially
    led_set(LED1, OFF);

    // Display title screen text
    displayText(" PONG");

    while (true) {
        // Wait for the pacer and then update display/navswitch every loop
        pacer_wait();
        tinygl_update();
        navswitch_update();

        // Get received token every loop (will be -1 if no data)
        rcvd = recvIR();

        if (mode == START_UP) {
            /* If PLAYER_SET token received, the other player must be player 1
                so set this player to player 2 */
            if (rcvd == 'P') {
                player = 2;
            }

            if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
                if (player == 2) {
                    displayText(" P2");
                    mode = PLAYER_SET;
                } 
                else {
                    player = 1;
                    displayText(" P1");
                    mode = PLAYER_SET;
                    sendIR('P');
                }
            }
        }

        else if (mode == PLAYER_SET) {
            // Player ready to begin: go to ROUND_ACTIVE and let opponent know
            if (player == 1 && navswitch_push_event_p(NAVSWITCH_PUSH)) {
                startRound(&ballActive, &ballOnSide, &paddlePos, &ticks, &time);
                mode = ROUND_ACTIVE;
                sendIR('R');
            }
            // Opponent ready to begin: go to ROUND_ACTIVE
            else if (player == 2 && rcvd == 'R') {
                startRound(&ballActive, &ballOnSide, &paddlePos, &ticks, &time);
                mode = ROUND_ACTIVE;
            }
        }

        else if (mode == ROUND_ACTIVE) {
            /* Every tick, increment both ticks and time, and update paddle
                based on player input and current position */
            ticks++;
            time++;
            paddlePos = updatePaddle(paddlePos);

            /* If ball active, turn LED on for the player that currently has it 
                on their side */
            if (ballActive) {
                if (ballOnSide) {
                    led_set(LED1, ON);
                } else {
                    led_set(LED1, OFF);
                }
            }
            // If ball inactive, turn LED on for player serving this round
            else {
                if (player == server) {
                    led_set(LED1, ON);
                } else {
                    led_set(LED1, OFF);
                }
            }

            if (!ballActive) {
                // Player is ready to serve: start ball and let opponent know
                if (player == server && navswitch_push_event_p(NAVSWITCH_PUSH)) {
                    ballActive = true;
                    ballOnSide = true;
                    ball = initBall(COL1, COL1, 1, 1);
                    display_pixel_set(ball.pos.col, ball.pos.row, ON);
                    sendIR('B');
                }
                // Opponent has served: set ball to active
                else if (player != server && rcvd == 'B') {
                    ballActive = true;
                    ballOnSide = false;
                }
            }

            /* Update ball status at an interval determined by how many ticks
                have passed since the round started. Hence the speed of the
                ball will increase as the round goes on. */
            else if (ballOnSide && ticks >= DELAY - (time / TIME_DIVISOR)) {
                display_pixel_set(ball.pos.col, ball.pos.row, OFF);
                updateBall(&ball, paddlePos);
                display_pixel_set(ball.pos.col, ball.pos.row, ON);

                /* Ball reached our end: we lost, therefore send LOST token, 
                    add one to opponent's score and prepare for next round */
                if (ball.pos.col > COL5 && ball.dir.colInc > 0) {
                    scores[player % 2]++;
                    server = (server % 2) + 1;
                    displayScore(scores, player);
                    mode = ROUND_END;
                    sendIR('L');
                }

                /* Ball has reached partition: send TRANSMIT token to opponent
                    then send ball's pos/dir data */
                else if (ball.pos.col < COL1 && ball.dir.colInc < 0) {
                    ballOnSide = false;
                    sendIR('T');
                    sendIR(ball.pos.row);
                    sendIR(ball.dir.rowInc);
                }

                // Reset ticks since last update
                ticks = 0;
            }

            else if (!ballOnSide) {
                /* Opponent's TRANSMIT token received, make blocking calls to
                    receive ball's pos/dir data */
                if (rcvd == 'T') {
                    int8_t row = recvIRBlocking();
                    row = 6 - row;

                    int8_t rowInc = recvIRBlocking();
                    rowInc = -rowInc;

                    ballOnSide = true;
                    /* Ball is started in column -1 (beyond the display) so 
                        that the timing works well as it immediately updates 
                        to column 0. To compensate for the update moving it 
                        up/down, it is started in row-rowInc */
                    ball = initBall(-1, row-rowInc, 1, rowInc);
                    display_pixel_set(ball.pos.col, ball.pos.row, ON);

                /* Opponent's LOST token received: add 1 to our score, prepare
                    for next round, display scores and go to ROUND_END mode */
                } else if (rcvd == 'L') {
                    scores[player - 1]++;
                    server = (server % 2) + 1;
                    displayScore(scores, player);
                    mode = ROUND_END;
                }
            }
        }

        else if (mode == ROUND_END) {
            led_set(LED1, OFF);

            // Check if anyone has won enough rounds yet, if so go to GAME_OVER
            if (scores[player - 1] >= 5) {
                displayText(" WINNER!");
                mode = GAME_OVER;
            }
            else if (scores[player % 2] >= 5) {
                displayText(" LOSER...");
                mode = GAME_OVER;
            }

            /* Check if either player wants to continue, if so return to 
                ROUND_ACTIVE */
            else if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
                startRound(&ballActive, &ballOnSide, &paddlePos, &ticks, &time);
                mode = ROUND_ACTIVE;
                sendIR('R');
            }
            else if (rcvd == 'R') {
                startRound(&ballActive, &ballOnSide, &paddlePos, &ticks, &time);
                mode = ROUND_ACTIVE;
            }
        }
    }

    return 0;
}
