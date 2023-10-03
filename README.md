# try 

[Discord Channel](https://discord.gg/QFzP9vaR8j)

Simple and clean exception handling in C

## example

```

  #include "try.h"

  #define OUTOFMEM    1
  #define WRONGINPUT  2
  #define INTERNALERR 3

  // Define a variable of type `try_t` for the use of the library
  try_t catch = 0;

  void some_other_func() 
  {
    ... code ...
     throw(WRONGINPUT);
    ... code ...
  }

  void a_function()
  {
    try {
       ... code ...
       if (something_failed) 
         throw(execption_num)  // exception_num must be between 1 and 65535 
   
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
  }

```

## Use try/catch in your program

 - Set your INCLUDE PATH so that `try.h` is reachable and include it
   from youre source files. 

 - In one (and only one) of your source files define a global variable
   of type `try_t` and set it to 0 (I suggest using `try_t catch = 0;`)
   The library needs this definition to keep track of `try` blocks nesting
   it is not supposed to be used or changed by the programmer.

 - Alternatively, you can compile `try.c` and link against `try.o`
  
See the tests in the `test` directory for some example.

## Reference

### Try/Catch

``` C
  try {
     // Block of code that could potentially throw an exception
     throw(an_exception); // Directly or
     a_function();        // possibly generated
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
If you really need to exit in an unstructured way, use `leave()` that
*must* be outside any loop or switch block.
It will move you to the end of the try/catch block.

### Throwing exceptions.

```
  throw(exc [, err])  Throws an exception (a positive integer between 1 and 65535).
                      The value specified by `err`, if greater than 0, will be
                      assigned to `errno`. This can be useful to specify further
                      information about the exception.

  rethrow()           Only usable in a catch block to throw the same exception.

  thrownexception()   Only usable in a catch block to retrieve the exception
  or thrown()         that has been caught. Only useful in a catch() block).

  thrownid()          Only usable in a catch block to retrieve the exception
                      identifier (the second parameter of the throw() function)

  thrownfile()        Only usable in a catch block to retrieve the filename
  thrownline()        and the line of the try/catch block that genereated the
                      exception. Useful to print a more meaningful error message
```


