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

#define LED_PIN     8     // Data pin connected to onboard RGB LED
#define NUM_LEDS    1     // Only one LED on board

Adafruit_NeoPixel pixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

uint32_t pixelColors[6] = {
  pixel.Color(255, 0, 0),    // Red
  pixel.Color(0, 0, 255),    // Blue
  pixel.Color(0, 255, 0),    // Green
  pixel.Color(255, 255, 0),  // Yellow
  pixel.Color(128, 0, 128),  // Purple
  pixel.Color(255, 165, 0)   // Orange
};

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
        CodeBreaker(){

        };

        /**
         * @brief Set-up the new CodeBreaker object.
         * @note Sets up the button as pull-down pins
         */
        void setup(){
            pixel.begin();
            pixel.setBrightness(20);

            pinMode(BUTTON_PIN1, INPUT_PULLDOWN);
            pinMode(BUTTON_PIN2, INPUT_PULLDOWN);
            pinMode(BUTTON_PIN3, INPUT_PULLDOWN);
            pinMode(BUTTON_PIN4, INPUT_PULLDOWN);
            pinMode(ENTER, INPUT_PULLDOWN);
            
            move.turn = false;
        };

        /**
         * @brief Placeholder Function that toggles peg colors on button presses
         * @note debounced using blocking while loops, don't do this for your ISR or it will crash
         * @todo Replace this function with a version based on ISRs
         */
        bool makeGuess() {
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

        /**
         * @brief Resets the Guess to default
         * @note Extraneous but just in case someone wanted to follow the actual online game style
         */
        void resetGuess(){
            for (uint8_t i = 0; i < 4; i++){
                move.playerGuess[i] = 0;
            }
        };

        /**
         * @brief Print Current Guess.
         */
        void printGuess(){
            for (uint8_t i = 0; i < 4; i++){
                Serial.print(colorChar[move.playerGuess[i]]);
                Serial.print(" ");
            }
            Serial.println();
        };
        
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
        bool waitForTurn(){
            return move.turn;
        };

        /**
         * @brief Dummy function for mimicking BLE callback on Client side, used and implemented in starter code
         * @note use only for Lab 3
         */
        void notify() {};

        /**
         * @brief Copy your guess into buffer to be transmitted
         * @param move the codebreakers guess
         * @param size the size of the codebreakers guess
         * @note Dummy function for mimicking BLE flow, use only for Lab 3
         */
        void loadGuess(PlayerBuffer* move, int size){};


    private:

};

#endif
// #endif