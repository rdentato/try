# try    [![Build Status](https://travis-ci.com/rdentato/try.svg?branch=master)](https://travis-ci.com/rdentato/try) 

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
   
       some_other_func();      // you can throw exceptions
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
    catch() {                 // catch any other exception
       ... code ...            // otherwise the progam would abort.
    }


```

## Use try/catch in your program

 - Set your INCLUDE PATH so that `try.h` is reachable and include it
   from youre source files. 
 - In one (and only one) of your source files define a global variable
   of type `try_t` and set it to 0 (I suggest using `try_t catch = 0;`)
   The library needs this definition to keep track of `try` blocks nesting.

See the test `test\test1.c` for an example.

## Reference

### Try/Catch

``` C
  try {
     // Block of code that could potentially
     throw(an_exception) //. Possibly generated
     // in a function called by this block
  }
  catch(an_exception) { 
     // Block of code that is executed if the specified
     // exception is thrown within the try block
  }
  catch() {
     // Block of code that is executed if no other handler
     // catches the exception. It is highly reccomended to 
     // ALWAYS have a catch() block; unhandled exceptions
     // will make the program abort().
  }
```
Remember to never leave a try/catch block with return, goto or break!

### Throwing exceptions.

```
  throw(exc [, id])   Throws an exception (a positive integer).
                      An optional "identifier" (an integer between -16384 and
                      16383) can be specified to provide additional info.

  rethrow()           Only usable in a catch block to throw the same exception.

  thrown()            Only usable in a catch block to retrieve the exception
                      that has been caught. Only useful in a catch() block).

  thrownid()          Only usable in a catch block to retrieve the exception
                      identifier (the second parameter of the throw() function)

  thrownfile()        Only usable in a catch block to retrieve the filename
  thrownline()        and the line of the try/catch block that genereated the
                      exception. Useful to print a more meaningful error message
```


