#pragma once

/**
 * @file CodeBreaker.h
 * @author Jason Hsiao
 * @date 12/27/2025
 * @version 1.0
 *
 * @brief Header for CodeBreaker Class.
 *
 * This file allows you to initialize and manage the Code Breaker in Mastermind
 * @see pinout.h for the hardware definitions like GPIO mapping.
 */

// #ifdef CODEBREAKER
#ifndef CodeBreaker_h     // Traditional Header Guard just in case but feel free to remove
#define CodeBreaker_h

#include <Arduino.h>
#include "include/config.h"

#include <Adafruit_NeoPixel.h>

extern Adafruit_NeoPixel pixel;

extern uint32_t pixelColors[6];

#ifndef BUTTON_PIN1
    #define BUTTON_PIN1 4
#endif
#ifndef BUTTON_PIN2
    #define BUTTON_PIN2 5
#endif
#ifndef BUTTON_PIN3
    #define BUTTON_PIN3 6
#endif
#ifndef BUTTON_PIN4
    #define BUTTON_PIN4 7
#endif
#ifndef ENTER
    #define ENTER 1
#endif

/**
 * @brief Implements MasterMind CodeBreaker functionality
 */
class CodeBreaker{
    public:
        /**
         * @brief Construct a new CodeBreaker object.
         * @note Does not initialize hardware. Use setup() for GPIO/PIO setup 
         * to ensure the hardware is ready after the system clock stabilizes.
         */
        CodeBreaker();

        /**
         * @brief Set-up the new CodeBreaker object.
         * @note Sets up the button as pull-down pins
         */
        void setup();

        /**
         * @brief Placeholder Function that toggles peg colors on button presses
         * @note debounced using blocking while loops, don't do this for your ISR or it will crash
         * @todo Replace this function with a version based on ISRs
         */
        bool makeGuess();

        /**
         * @brief Resets the Guess to default
         * @note Extraneous but just in case someone wanted to follow the actual online game style
         */
        void resetGuess();

        /**
         * @brief Print Current Guess.
         */
        void printGuess();
        
         /**
         * @brief The object holding the current state of the game, including guess, results, and turn
         */
        PlayerBuffer move;

        /* --------------------------------------------------------------------------
        * SECTION: DUMMY METHODS FOR LAB 3 STARTER CODE
        * -------------------------------------------------------------------------- */
        /**
         * @brief Dummy function for mimicking BLE callback on Client side, used and implemented in starter code
         * @note use only for Lab 3
         */
        bool waitForTurn();

        /**
         * @brief Dummy function for mimicking BLE callback on Client side, used and implemented in starter code
         * @note use only for Lab 3
         */
        void notify();

        /**
         * @brief Copy your guess into buffer to be transmitted
         * @param move the codebreakers guess
         * @param size the size of the codebreakers guess
         * @note Dummy function for mimicking BLE flow, use only for Lab 3
         */
        void loadGuess(PlayerBuffer* move, int size);


    private:

};

#endif
// #endif