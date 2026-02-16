#pragma once
#include <Arduino.h>


enum Colors {
    RED = 1,
    BLUE = 2,
    GREEN = 3,
    YELLOW = 4,
    PURPLE = 5,
    ORANGE = 6
};

inline constexpr char colorChar[6] = {'R', 'B', 'G', 'Y', 'P', 'O'};

struct __attribute__((packed)) PlayerBuffer {
    volatile uint8_t playerGuess[4];
    uint8_t results[2]; // First is number of black pegs, second is number of white pegs
    bool turn;  // Indicate if the game is active (i.e. if the player can make a turn)
};




