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

#ifndef TRY_VERSIONjmp_buffer
#define TRY_VERSION 0x0002000F

#include <stdio.h>
#include <setjmp.h>
#include <assert.h>
#include <stdint.h>
#include <errno.h>

typedef struct try_jb_s {
  jmp_buf          jmp_buffer;     // Jump buffer for setjmp/longjmp
  struct try_jb_s *prev_jmpbuf;    // Link to parent for nested try
  const char      *file_name;      // Filename
  int              line_num;       // Line number
  unsigned short   exception_num;  // Exception number
  short            count;          // Counter
} try_jb_t;

#ifdef _MSC_VER
  #define TRY_THREAD __declspec( thread )
#else
  #define TRY_THREAD __thread
#endif

extern TRY_THREAD try_jb_t *try_jmp_list;

#define try_t TRY_THREAD try_jb_t *try_jmp_list=NULL; int

#define CATCH__HANDLER 0
#define try_abort() assert(CATCH__HANDLER)

#define try_throw(x,y,fname,line) \
  do { \
    unsigned short exception_num = x; int ey = y;\
    if (try_jmp_list == NULL) try_abort(); \
    if (exception_num > 0) {\
      try_jmp_list->file_name  = fname; \
      try_jmp_list->line_num  = line; \
      if (ey>0) errno = ey; \
      longjmp(try_jmp_list->jmp_buffer, exception_num); \
    }\
  } while(0)


#define try_INIT     {.prev_jmpbuf = try_jmp_list, .count = 0, .file_name = NULL, .line_num = 0}

#define try          for ( try_jb_t try_jb = try_INIT; \
                          (try_jb.count-- <= 0) && (try_jmp_list = &try_jb); \
                           try_jmp_list = try_jb.prev_jmpbuf, try_jb.count = (try_jb.exception_num == 0? 2 : try_jb.count)) \
                            if (try_jb.count < -1) try_abort(); \
                       else if (((try_jb.exception_num = setjmp(try_jb.jmp_buffer)) == 0)) 

#define catch__1(x)     else if ((try_jb.exception_num == (x)) && (try_jmp_list=try_jb.prev_jmpbuf, try_jb.count=2)) 
#define catch__0( )     else for (try_jmp_list=try_jb.prev_jmpbuf; try_jb.count < 0; try_jb.count=2) 

#define catch__cnt(x,y,z,a ...) a
#define catch__argn(...)       catch__cnt(__VA_ARGS__, 2, 0, 1)
#define catch__comma(...)      ,
#define catch__cat2(x, y,...)  x ## y
#define catch__join(x, y)      catch__cat2(x, y)

#define catch(...) catch__join(catch__ , catch__argn(catch__comma __VA_ARGS__ ()))(__VA_ARGS__)

#define throw(x,...)  try_throw(x, __VA_ARGS__ -0, __FILE__, __LINE__)

#define rethrow()    try_throw(try_jb.exception_num, errno, __FILE__, __LINE__)
#define thrown()     try_jb.exception_num
#define thrownfile() try_jb.file_name
#define thrownline() try_jb.line_num

#define leave(e)    if (!(try_jb.count = 2)); else continue;

#endif
