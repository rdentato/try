# try    [![Build Status](https://travis-ci.org/rdentato/try.svg?branch=master)](https://travis-ci.org/rdentato/try) 

Simple and clean exception handling in C

## example

```
  // TRY_MAIN must be defined before including try.h
  // only ONCE (usually where your main() function is)
  #define TRY_MAIN
  #include "try.h"

  #define OUTOFMEM    1
  #define WRONGINPUT  2
  #define INTERNALERR 3
  
  ... code ...

    try {
       ... code ...
       if (something_failed) 
         throw(execption_num)  // must be > 0 
   
       some_other_func();      // you can trhow exceptions
                               // from other functions 
   
       ... code ...            // You MUST NEVER jump out of a try/catch block
                               // (via return, goto or break). Use throw() to
                               // ensure the right behaviour.
    }  
    catch(OUTOFMEM) {
       ... code ...
    }
    catch(WRONGINPUT) {
       ... code ...
    }
    catchall {                 // catch any other exception
       ... code ...            // otherwise the progam would abort.
    }


```

## Use try/catch in your program

### Header only

 - Set your INCLUDE PATH so that `try.h` is reachable and include it
   from youre source files. 
 - In one of your source files (usually the source file where main() is
   defined) define `TRY_MAIN` *before* including `try.h`

See the test `test\test1.c` for an example.

### Header and source file

 - Set your INCLUDE PATH so that `try.h` is reachable and include it
   from youre source files. 
 - Compile `try.c` and link the object file against your program

See the test `test\ltest1.c` for an example.
 

