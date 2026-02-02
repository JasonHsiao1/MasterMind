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

// #ifdef HAS_SENSORS

#include <Arduino.h>
#include "include/config.h"
#include <Wire.h>


#define CODE_LENGTH 4


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
        CodeMaker(){

        };

        /**
         * @brief Set-up the new CodeMaker object.
         * @note Initializes objects.
         */
        void setup() {
            randomSeed(analogRead(0));
        };

        /**
         * @brief Reset/stop for the CodeMaker object.
         * @return Whether the e-stop attempt was successful.
         * @note Not implemented due to looped game
         */
        void endGame(){
            Serial.println("Congrats you won the game!");
            Serial.println("Now resetting your code.");
            generateCode();
        };

        void generateCode(){
            for (uint8_t i = 0; i < 4; i++){
                secretCode[i] = static_cast<uint8_t>(random(0, 6));
            }
        };
        
        void generateTrainingData(int samplesPerStyle);

        void generateBiasedCode(uint8_t style){
            switch (style){
                case 0: { // Low numbers
                    for (uint8_t i = 0; i < 4; i++){
                        secretCode[i] = static_cast<uint8_t>(random(0, 3));
                    }
                    break;
                }
                case 1: { // Always Unique and Descending
                    uint8_t pool[6] = {0, 1, 2, 3, 4, 5};
                    
                    // shuffle randomly, I might've vibe-coded this
                    for (int i = 5; i > 0; i--) {
                        int j = static_cast<uint8_t>(random(0, i + 1));
                        uint8_t temp = pool[i];
                        pool[i] = pool[j];
                        pool[j] = temp;
                    }

                    // Simple bubble sort of the first 4 numbers
                    for (int i = 0; i < 4; i++) {
                        for (int j = i + 1; j < 4; j++) {
                            if (pool[i] < pool[j]) {
                                uint8_t temp = pool[i];
                                pool[i] = pool[j];
                                pool[j] = temp;
                            }
                        }
                    }

                    // 
                    for (int i = 0; i < 4; i++) {
                        secretCode[i] = pool[i];
                    }
                    break;
                }
                case 2: { // Two Duplicates
                    uint8_t duplicate1 = static_cast<uint8_t>(random(3, 6)); // Pick a duplicate
                    uint8_t duplicate2 = static_cast<uint8_t>(random(1, 6)); // Pick a second duplicate in the high side to distinguish from lows
                    if (duplicate1 == duplicate2){
                        duplicate2--;
                    }
                    uint8_t pool[4] = {duplicate1, duplicate2, duplicate1, duplicate2};
                    
                    // shuffle randomly, I might've vibe-coded this
                    for (int i = 3; i > 0; i--) {
                        int j = static_cast<uint8_t>(random(0, i + 1));
                        uint8_t temp = pool[i];
                        pool[i] = pool[j];
                        pool[j] = temp;
                    }
                     
                    for (int i = 0; i < 4; i++) {
                        secretCode[i] = pool[i];
                    }
                    
                    break;
                }
            }
        };
        
        
        bool throwResultsFlag();    // signal when it's time to throw results flag

        void move_callBack();

        /**
         * @brief Read and print the current hand
         * @todo Need to switch here to show each player the card.
         */
        uint16_t showResults();


        void checkGuess(uint8_t* results, uint8_t* guess) {
            uint8_t black = 0;
            uint8_t white = 0;

            bool secretUsed[CODE_LENGTH] = {false};
            bool guessUsed[CODE_LENGTH]  = {false};

            // 1. Count black pegs (correct position & color)
            for (uint8_t i = 0; i < CODE_LENGTH; i++) {
                if (guess[i] == secretCode[i]) {
                    black++;
                    secretUsed[i] = true;
                    guessUsed[i] = true;
                }
            }

            // 2. Count white pegs (correct color, wrong position)
            for (uint8_t i = 0; i < CODE_LENGTH; i++) {
                if (guessUsed[i]) continue;

                for (uint8_t j = 0; j < CODE_LENGTH; j++) {
                    if (!secretUsed[j] && guess[i] == secretCode[j]) {
                        white++;
                        secretUsed[j] = true;
                        break;
                    }
                }
            }

            results[0] = black;
            results[1] = white;
        }


        void compare(uint8_t* results, uint8_t* correct, uint8_t* guess) {
            uint8_t black = 0;
            uint8_t white = 0;

            bool correctUsed[CODE_LENGTH] = {false};
            bool guessUsed[CODE_LENGTH]  = {false};

            // 1. Count black pegs (correct position & color)
            for (uint8_t i = 0; i < CODE_LENGTH; i++) {
                if (guess[i] == correct[i]) {
                    black++;
                    correctUsed[i] = true;
                    guessUsed[i] = true;
                }
            }

            // 2. Count white pegs (correct color, wrong position)
            for (uint8_t i = 0; i < CODE_LENGTH; i++) {
                if (guessUsed[i]) continue;

                for (uint8_t j = 0; j < CODE_LENGTH; j++) {
                    if (!correctUsed[j] && guess[i] == correct[j]) {
                        white++;
                        correctUsed[j] = true;
                        break;
                    }
                }
            }

            results[0] = black;
            results[1] = white;
        }


        // void checkGuess(PlayerBuffer* attempt) {
        //     uint8_t black = 0;
        //     uint8_t white = 0;

        //     bool secretUsed[CODE_LENGTH] = {false};
        //     bool guessUsed[CODE_LENGTH]  = {false};

        //     // 1. Count black pegs (correct position & color)
        //     for (uint8_t i = 0; i < CODE_LENGTH; i++) {
        //         if (attempt.playersGuess[i] == secretCode[i]) {
        //             black++;
        //             secretUsed[i] = true;
        //             guessUsed[i] = true;
        //         }
        //     }

        //     // 2. Count white pegs (correct color, wrong position)
        //     for (uint8_t i = 0; i < CODE_LENGTH; i++) {
        //         if (guessUsed[i]) continue;

        //         for (uint8_t j = 0; j < CODE_LENGTH; j++) {
        //             if (!secretUsed[j] && guess[i] == secretCode[j]) {
        //                 white++;
        //                 secretUsed[j] = true;
        //                 break;
        //             }
        //         }
        //     }

        //     results[0] = black;
        //     results[1] = white;
        // }

        // uint8_t checkCorrect(uint8_t* guess);
        // uint8_t checkCorrectColor(uint8_t* guess);

        void printCode(){
            for (uint8_t i = 0; i < 4; i++){
                Serial.print(colorChar[secretCode[i]]);
                Serial.print(" ");
            }
            Serial.println();
        };
        
        void getCode(uint8_t* code){
            for (uint8_t i = 0; i < 4; i++){
                code[i] = secretCode[i];
            }
        }
        // void printCode() {
        //     for (uint8_t i = 0; i < 4; i++){
        //         Serial.print(secretCode[i]);
        //         Serial.print(" ");
        //     }
        //     Serial.println();
        // }

    private:
        uint8_t deck[13]; // eventually use Card struct, for now just keep track
        uint8_t secretCode[4];
        uint8_t players[5]; // Players at table, change this to servers or BLE UUID later.
};

// #endif