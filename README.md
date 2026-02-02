# Mastermind Helper Library for ECE4180 Labs 3 & 4

See the structure of the following Arduino library `Mastermind` which includes `Codemaker` and `Codebreaker`:

|--src  
|  |  
|  |--Codemaker  
|  |  |--Codemaker.cpp  
|  |  |--Codemaker.h  
|  |  
|  |--Codebreaker  
|  |  |- Codebreaker.c  
|  |  |- Codebreaker.h  
|  |  
|  |--include  
|  |  |- config.h  
|  |  
|  |- Mastermind.cpp  
|  |- Mastermind.h  
|  
|--examples  
|  |--MastermindGameplay  
|  |  |- MastermindGameplay.ino  
|  
|- keywords.txt  
|- library.properties  
|- README.md ---> This file  

How to use the helper functions in `Mastermind.h` for Codemaker and Codebreaker:
```
// If you want to use the helper functions for the CodeMaker
#define CODEMAKER

// If you want to use the helper functions for the CodeBreaker
#define CODEBREAKER

#include <Mastermind.h>

```

Easily include this library in your Arduino by exporting as a .zip file and importing using the include .zip file feature
