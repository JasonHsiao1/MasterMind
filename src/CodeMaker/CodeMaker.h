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

// #ifdef CODEMAKER     // Old build flag but take care of this in Mastermind.h

#ifndef CodeMaker_h     // Traditional Header Guard just in case but feel free to remove
#define CodeMaker_h

#include <Arduino.h>
#include "include/config.h"
// #include <Wire.h>

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
         * @note Gets a random seed to work with.
         */
        void setup();

        /**
         * @brief Generate a random code
         * @note random seed is based on analog values
         */
        void generateCode();
        
        /**
         * @brief Compare the user's guess against the results
         * @param results the object that holds the returned results
         * @param guess the users guess
         * @note is a wrapper for more general compare function, but must use this because code is private
         */
        void checkGuess(uint8_t* results, uint8_t* guess);

        /**
         * @brief Compare the user's guess against a reference
         * @param results the object that holds the returned results
         * @param correct the reference against which the correct is compared against
         * @param guess the users guess
         * @note this function becomes important in the AI/ML portions of Lab 4
         */
        void compare(uint8_t* results, uint8_t* correct, uint8_t* guess);

        /**
         * @brief Print the correct code for reference/debugging
         * @note use this to print the code, because the code is secret so it's private
         */
        void printCode();
        
        /**
         * @brief Helper function to print any code
         * @param code the code that you want to print out
         */
        void printCode(uint8_t* code);
        
        /**
         * @brief Return the correct code for reference/debugging
         * @param code the object that you want to populate with the secret code
         * @note use this as a getter to retrieve the code, because the code is secret so it's private
         */
        void getCode(uint8_t* code);
        

        /* --------------------------------------------------------------------------
        * SECTION: Helper functions for Lab 4 ML section
        * -------------------------------------------------------------------------- */
        /**
         * @brief Helper function for generating training data for ML training
         * @param samplesPerStyle the number of samples that you wish to generate
         * @note use only for Lab 4, implemented in the starter code
         */
        void generateTrainingData(int samplesPerStyle);

        /**
         * @brief Helper function for generating biased codes for ML recognition
         * @param style which biased approach: 1: Low Numbers, 2: Unique & Descending, 3: Pairs
         * @note use only for Lab 4
         */
        void generateBiasedCode(uint8_t style);


        /* --------------------------------------------------------------------------
        * SECTION: DUMMY METHODS FOR LAB 3 STARTER CODE
        * -------------------------------------------------------------------------- */
        /**
         * @brief Dummy function for mimicking BLE flow, used and implemented in starter code
         * @note use only for Lab 3
         */
        bool throwResultsFlag();
        
        /**
         * @brief Dummy function for mimicking BLE callback on Client side, used and implemented in starter code
         * @note use only for Lab 3
         */
        void move_callBack();
        
        /**
         * @brief Reset/stop for the CodeMaker object.
         * @note Do not need to actually use outside of demo, extra credit requires you to make your own.
         */
        void endGame();

    private:
        uint8_t secretCode[4];
};

#endif
// #endif