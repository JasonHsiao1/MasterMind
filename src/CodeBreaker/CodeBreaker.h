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

// #ifdef HAS_SENSORS

#include <Arduino.h>
#include "include/config.h"
#include <Wire.h>
#include "CodeBreaker/Board.h"

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
         * @note Initializes objects.
         */
        void setup(){
            pixel.begin();
            
            pixel.setBrightness(20); // Make it less bright!

            pinMode(BUTTON_PIN1, INPUT_PULLDOWN);
            // attachInterruptArg(BUTTON_PIN1, onButtonPress, (void*) &(move.playerGuess[0]), RISING);

            pinMode(BUTTON_PIN2, INPUT_PULLDOWN);
            // attachInterruptArg(BUTTON_PIN2, onButtonPress, (void*) &(move.playerGuess[1]), RISING);

            pinMode(BUTTON_PIN3, INPUT_PULLDOWN);
            // attachInterruptArg(BUTTON_PIN3, onButtonPress, (void*) &(move.playerGuess[2]), RISING);

            pinMode(BUTTON_PIN4, INPUT_PULLDOWN);
            // attachInterruptArg(BUTTON_PIN4, onButtonPress, (void*) &(move.playerGuess[3]), RISING);

            pinMode(ENTER, INPUT_PULLDOWN);
            
            move.turn = false;

        };

        /**
         * @brief Read and print the current hand
         * @todo Need to switch here to show each player the card.
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

        bool waitForTurn(){
            return move.turn;
        };

        void notify();


        PlayerBuffer move;

    private:
        // uint8_t deck[13]; // eventually use Card struct, for now just keep track
        // uint8_t dealersHand[10];
        Board board;
};

// #endif