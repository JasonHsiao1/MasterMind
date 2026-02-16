/**
 * @file CodeMaker.cpp
 * @author Jason Hsiao
 * @date 12/24/2025
 * @version 1.0
 *
 * @brief Implementation of CodeMaker.
 *
 * This file allows you to initialize and control PWM outputs,
 * essentially acting as a motor controller.
 * 
 * @todo This was designed for the SparkFun H-Bridge Motor Driver, 
 * so this will need to be converted to work with ESCs, but the core 
 * concept of outputting PWMs remains. 
 * @see config.h for the hardware definitions like GPIO mapping
 */


#include "CodeMaker.h"

// Constructor
CodeMaker::CodeMaker() {
    // Initialize internal variables if needed

}

void CodeMaker::setup() {
    randomSeed(analogRead(0));
};

void CodeMaker::endGame(){
    Serial.println("Congrats you won the game!");
    Serial.println("Now resetting your code.");
    generateCode();
};

void CodeMaker::generateCode(){
    for (uint8_t i = 0; i < 4; i++){
        secretCode[i] = static_cast<uint8_t>(random(0, 6));
    }
};

// void generateTrainingData(int samplesPerStyle);

void CodeMaker::generateBiasedCode(uint8_t style){
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

void CodeMaker::checkGuess(uint8_t* results, uint8_t* guess) {
    compare(results, secretCode, guess);
}


void CodeMaker::compare(uint8_t* results, uint8_t* correct, uint8_t* guess) {
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

void CodeMaker::printCode(){
    for (uint8_t i = 0; i < 4; i++){
        Serial.print(colorChar[secretCode[i]]);
        Serial.print(" ");
    }
    Serial.println();
};

void CodeMaker::printCode(uint8_t* code) {
    for (uint8_t i = 0; i < 4; i++){
        Serial.print(colorChar[code[i]]);
        Serial.print(" ");
    }
    Serial.println();
}

void CodeMaker::getCode(uint8_t* code){
    for (uint8_t i = 0; i < 4; i++){
        code[i] = secretCode[i];
    }
}
