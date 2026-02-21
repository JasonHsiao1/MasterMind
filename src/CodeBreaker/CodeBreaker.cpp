/**
 * @file CodeBreaker.cpp
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
#include <Arduino.h>
#include "CodeBreaker.h"

CodeBreaker::CodeBreaker(){

};

void CodeBreaker::setup(){
    pixel.begin();
    pixel.setBrightness(20);

    pinMode(BUTTON_PIN1, INPUT_PULLDOWN);
    pinMode(BUTTON_PIN2, INPUT_PULLDOWN);
    pinMode(BUTTON_PIN3, INPUT_PULLDOWN);
    pinMode(BUTTON_PIN4, INPUT_PULLDOWN);
    pinMode(ENTER, INPUT_PULLDOWN);
    
    move.turn = false;
};

bool CodeBreaker::makeGuess() {
    // Consider making this interrupt-based and one function, since this is a lot of identical code
    if (digitalRead(BUTTON_PIN1)){
        // Serial.println("Button 1 Press");
        move.playerGuess[0] = ((move.playerGuess[0] + 1) % 6); 
        pixel.setPixelColor(0, pixelColors[move.playerGuess[0]]);
        printGuess();
        while (digitalRead(BUTTON_PIN1));
    }
    if (digitalRead(BUTTON_PIN2)){
        // Serial.println("Button 2 Press");
        move.playerGuess[1] = ((move.playerGuess[1] + 1) % 6);
        pixel.setPixelColor(0, pixelColors[move.playerGuess[1]]);
        printGuess();
        while (digitalRead(BUTTON_PIN2));
    }
    if (digitalRead(BUTTON_PIN3)){
        // Serial.println("Button 3 Press");
        move.playerGuess[2] = ((move.playerGuess[2] + 1) % 6);
        pixel.setPixelColor(0, pixelColors[move.playerGuess[2]]);
        printGuess();
        while (digitalRead(BUTTON_PIN3));
    }
    if (digitalRead(BUTTON_PIN4)){
        // Serial.println("Button 4 Press");
        move.playerGuess[3] = ((move.playerGuess[3] + 1) % 6);
        pixel.setPixelColor(0, pixelColors[move.playerGuess[3]]);
        printGuess();
        while (digitalRead(BUTTON_PIN4));
    }
    pixel.show();
    delay(50);
    if (digitalRead(ENTER)){    // You can put this in an interrupt to but I'm leaving it here since it has least priority anyway
        Serial.println("Enter");
        while (digitalRead(ENTER));
        return true;
    } else {
        return false;
    };
};

void CodeBreaker::resetGuess(){
    for (uint8_t i = 0; i < 4; i++){
        move.playerGuess[i] = 0;
    }
};

void CodeBreaker::printGuess(){
    for (uint8_t i = 0; i < 4; i++){
        Serial.print(colorChar[move.playerGuess[i]]);
        Serial.print(" ");
    }
    Serial.println();
};

/* --------------------------------------------------------------------------
* SECTION: DUMMY METHODS FOR LAB 3 STARTER CODE
* -------------------------------------------------------------------------- */
bool CodeBreaker::waitForTurn(){
    return move.turn;
};

void CodeBreaker::notify() {};

void CodeBreaker::loadGuess(PlayerBuffer* move, int size){};
