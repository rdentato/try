/*
**  (C) by Remo Dentato (rdentato@gmail.com)
** 
** This software is distributed under the terms of the MIT license:
**  https://opensource.org/licenses/MIT
**     

[[[
# Exceptions

 Simple implementation of try/catch. try blocks can be nested.
 Exceptions are positive integers
 
  #define OUTOFMEM    1
  #define WRONGINPUT  2
  #define INTERNALERR 3

  void some_other_function()
  {
     ... code ...
     throw(WRONGINPUT); // will be handled in the parent function
     ... code ...
  }
  
  try {
     ... code ...
     if (something_failed) throw(OUTOFMEM);  // must be > 0 
     some_other_func(); // you can trhow exceptions from other functions too 
     ... code ...
  }  
  catch(OUTOFMEM) {
     ... code ...
  }
  catch(WRONGINPUT) {
     ... code ...
  }
  catch() {  // if not handled ...
     ... code ...
  }
                 
]]] */

#ifndef TRY_VERSION
#define TRY_VERSION 0x0003001C

#include <stdio.h>
#include <setjmp.h>
#include <string.h>

#ifndef exception_info
#define exception_info 
#endif

typedef struct exception_s {
   int   exception_num;
   int   line_num;
   char *file_name;
   exception_info;  // single semicolon (when the macro is empty) allowed by ISO/IEC 9899:1999 (C99) §6.7.2.1
} exception_t;

typedef struct try_jb_s {
  jmp_buf          jmp_buffer;     // Jump buffer for setjmp/longjmp
  struct try_jb_s *prev_jmpbuf;    // Link to parent for nested try
  int              exception_num;  // Exception number
  int              count;          // Counter
} try_jb_t;

// If you compiler doesn't support thread local variables, just compile with TRY_THREAD defined as empty.
#ifndef TRY_THREAD
#ifdef _MSC_VER
  #define TRY_THREAD __declspec( thread )
#else
  #define TRY_THREAD __thread
#endif
#endif

extern TRY_THREAD try_jb_t *try_jmp_list;
extern TRY_THREAD exception_t exception;

// Declare only ONCE a variable of type `try_t` as if it was an int.
// For example: try_t catch = 0;
#define try_t TRY_THREAD try_jb_t *try_jmp_list=NULL; exception_t TRY_THREAD exception; int

#ifndef tryabort
#define tryabort() (fprintf(stderr,"ERROR: Unhandled exception %d. %s:%d\n",\
                            exception.exception_num,exception.file_name,exception.line_num))
#endif

#define try          for ( try_jb_t try_jb = {.prev_jmpbuf = try_jmp_list, .count = 0}; \
                          (try_jb.count-- <= 0) && (try_jmp_list = &try_jb); \
                           try_jmp_list = try_jb.prev_jmpbuf, try_jb.count = (try_jb.exception_num == 0? 2 : try_jb.count)) \
                            if (try_jb.count < -1) { tryabort(); abort(); }\
                       else if (((try_jb.exception_num = setjmp(try_jb.jmp_buffer)) == 0)) 

#define catch__1(x)    else if ((try_jb.exception_num == (x)) && (try_jmp_list=try_jb.prev_jmpbuf, try_jb.count=2)) 
#define catch__0( )    else for (try_jmp_list=try_jb.prev_jmpbuf; try_jb.count < 0; try_jb.count=2) 

#define catch__cnt(x,y,z,N, ...) N
#define catch__argn(...)       catch__cnt(__VA_ARGS__, 2, 0, 1)
#define catch__comma(...)      ,
#define catch__cat2(x, y,...)  x ## y
#define catch__join(x, y)      catch__cat2(x, y)

#define catch(...) catch__join(catch__ , catch__argn(catch__comma __VA_ARGS__ ()))(__VA_ARGS__)

#define throw(exception_num, ...) \
  do { \
    memset(&exception,0,sizeof(exception_t)); \
    exception = ((exception_t){exception_num, __LINE__, __FILE__, __VA_ARGS__});\
    if (try_jmp_list == NULL) { tryabort(); abort(); } \
    if (exception.exception_num > 0) longjmp(try_jmp_list->jmp_buffer, exception_num); \
  } while(0)

#define rethrow(...) throw(exception.exception_num, __VA_ARGS__)

#define leave(e)  if (!(try_jb.count = 2)); else continue;

#endif  // TRY_VERSION
