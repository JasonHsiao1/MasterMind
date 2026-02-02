# pragma once
/**
 * @file Board.h
 * @author Jason Hsiao
 * @date 12/27/2025
 * @version 1.0
 *
 * @brief Header for Board Class.
 *
 * This file allows you to initialize and manage the board in Mastermind.
 * @see config.h for definitions like colors.
 */
#include "include/config.h"
#include <Arduino.h>


const int GUESS_NUM = 12;
const int COLS = 4;

class Board {
    public:
        void addGuess(uint8_t* input){
            if (sizeof(input) != 4){    // ensures that there are 4 characters in the input
                Serial.println("Incorrectly sized input");
                return;
            }
            for (uint8_t i = 0; i < COLS; i++){
                board[guess][i] = input[i];
            }
        }

        void printBoard() {
            for (uint8_t i = 0; i < guess; i++) {
                Serial.print("Row ");
                Serial.print(guess);
                Serial.print(": ");
                for (uint8_t j = 0; j < COLS; j++) {
                    Serial.print(colorChar[board[i][j]]);
                    Serial.print(" | ");
                }
                Serial.println();
            }
        };  // For loop

        void printColor(Colors color) {
            // alternatively do something like char colorChar[color] to map
            Serial.print(colorChar[color]);
            // switch (color) {
            //     case RED:
            //         Serial.print('R');
            //         break;
            //     case BLUE:
            //     case GREEN:
            //     case YELLOW:
            //     case PURPLE:
            //     case ORANGE: 
            // }       
        }
    
    private:
        uint8_t guess = 0;
        uint8_t board[GUESS_NUM][COLS];
        // bool results[GUESS_NUM][COLS];  // True, False, or null
        uint8_t results[GUESS_NUM][2];  // Black and White peg count

};