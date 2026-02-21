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

#include <Arduino.h>

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

#define BUTTON_PIN1 4
#define BUTTON_PIN2 5
#define BUTTON_PIN3 6
#define BUTTON_PIN4 7
#define ENTER 1


#define CODEMAKER
#define CODEBREAKER
#include <MasterMind.h>

CodeMaker host;
CodeBreaker player;

//============================================//
// TODO: Define globals like buttonStatus     //
// or timestamps for debouncing               //
//============================================//

void setup() {
  Serial.begin(9600);
  Serial.println("Starting stuff");

  player.setup();
  host.setup();

  // Dealer generates secret code and lets player know when to start
  host.generateCode();    // host sets up the code
  host.throwResultsFlag();   // Let Player know that they can do something 
  
  //============================================//
  // TODO: Set-up your interrupts               //
  //============================================//
}

void loop() {
  Serial.println("Player can go now");

  //============================================//
  // TODO: Comment out and replace this line    //
  // with ISR-based button handler              //
  while (!player.makeGuess());
  //============================================//
  
  Serial.println("Submitted");
  player.printGuess();

  // Player lets the host know that they have made a guess
  player.notify();

  // After notify, move_callBack is called on the host side
  host.move_callBack();
  
}

//============================================//
// TODO: Define and handle your interrupts    //
//============================================//


// Implement the BLE implementation of throwing the results flag
bool CodeMaker::throwResultsFlag(){
  player.move.turn = true;
  return true;
}

// Read Player's move and do something
void CodeMaker::move_callBack(){
  // Make a local copy (which is the same when you use BLE)
  uint8_t playersGuess[4];
  for (size_t i = 0; i < 4; ++i) {
    playersGuess[i] = player.move.playerGuess[i];  // Make a non-volatile copy
  }

  // Fill buffer with results from players guess
  uint8_t results[2];
  host.checkGuess(results, playersGuess);
  host.printCode(); // Print actual code for reference
  Serial.printf("Results: %d | %d \n", results[0], results[1]);
  
  if (results[0] == 4) {
    host.endGame(); // Don't worry about this for the main BLE parts, you'll implement this for extra credit
  } else {
    host.throwResultsFlag();
  }
}