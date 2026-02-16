#pragma once

/**
 * @file CodeMaker.h
 * @author Jason Hsiao
 * @date 12/27/2025
 * @version 1.0
 *
 * @brief Header for CodeMaker Class.
 *
 * This file allows you to initialize and manage the Code Maker in Mastermind.
 * @see config.h for definitions like colors.
 */

// #ifdef CODEMAKER

#include <Arduino.h>
#include "include/config.h"
#include <Wire.h>

#ifndef CODE_LENGTH
    #define CODE_LENGTH 4
#endif

/**
 * @brief Implements BlackJack CodeMaker functionality
 */
class CodeMaker{
    public:
        /**
         * @brief Construct a new CodeMaker object.
         * @note Does not initialize hardware. Use setup() for GPIO/PIO setup 
         * to ensure the hardware is ready after the system clock stabilizes.
         */
        CodeMaker();

        /**
         * @brief Set-up the new CodeMaker object.
         * @note Initializes objects.
         */
        void setup();

        /**
         * @brief Reset/stop for the CodeMaker object.
         * @return Whether the e-stop attempt was successful.
         * @note Not implemented due to looped game
         */
        void endGame();

        void generateCode();
        
        void generateTrainingData(int samplesPerStyle);

        void generateBiasedCode(uint8_t style);
        
        bool throwResultsFlag();    // signal when it's time to throw results flag

        void move_callBack();

        /**
         * @brief Read and print the current hand
         * @todo Need to switch here to show each player the card.
         */
        uint16_t showResults();


        void checkGuess(uint8_t* results, uint8_t* guess);


        void compare(uint8_t* results, uint8_t* correct, uint8_t* guess);

        void printCode();
        
        void printCode(uint8_t* code);
        
        void getCode(uint8_t* code);

    private:
        uint8_t secretCode[4];
};

// #endif