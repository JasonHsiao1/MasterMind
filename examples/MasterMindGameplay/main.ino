#include <Arduino.h>

#include "CodeMaker.h"
#include "CodeBreaker.h"


static constexpr uint8_t DEALER_SIGNAL = 22;

CodeMaker host;
CodeBreaker player;


void setup() {
  Serial.begin(115200);
  Serial.println("Starting stuff");
  pinMode(DEALER_SIGNAL, INPUT_PULLDOWN);

  player.setup();
  host.setup();

  // Bluetooth connection stuff

  // Don't know how necessary this one is
  // // Player notifies dealer to start
  // player.notify();

  // Dealer generates secret code and lets player know when to start
  host.generateCode();    // host sets up the code
  host.throwResultsFlag();   // Let Player know that they can do something 

}

void loop() {
  // Serial.println("Generating New Code");
  // host.generateCode();
  
  // player.resetGuess();
  // player.printGuess();
  
  // When player sees that their buffer has changed, make a move
  while (!player.waitForTurn()){
    player.move.turn = digitalRead(DEALER_SIGNAL);  // Code Maker says they're ready
  };

  // while (!digitalRead(DEALER_SIGNAL));
  Serial.println("Player can go now");
  // host.printCode();

  // Make a move
  while (!player.makeGuess());
  // player.makeGuess();
  Serial.println("Submitted");
  player.printGuess();


  // player.notify();

  // // After notify, move_callBack should be called
  host.move_callBack();

  
}

// Implement the BLE implementation of throwing the results flag
bool CodeMaker::throwResultsFlag(){
  player.move.turn = true;
  return true;
}

// Read Player's move and do something
void CodeMaker::move_callBack(){
  // Pull players guess from buffer
  // uint8_t* playersGuess = player.move.playerGuess;   // No longer works once you make this volatile

  // Make a local copy (which is the same when you use BLE){
  uint8_t playersGuess[4];

  for (size_t i = 0; i < 4; ++i) {
    playersGuess[i] = player.move.playerGuess[i];  // volatile → non-volatile copy
  }

  // Fill buffer with results from players guess
  uint8_t results[2];
  host.checkGuess(results, playersGuess);
  // player.move.results[0] = host.checkCorrect(playersGuess);
  // player.move.results[1] = host.checkCorrectColor(playersGuess);
  host.printCode();
  Serial.printf("Results: %d | %d \n", results[0], results[1]);
  
  if (results[0] == 4) {
    host.endGame();
  } else {
    host.throwResultsFlag();
  }
}