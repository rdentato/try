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
  
  try {
     ... code ...
     if (something_failed) throw(execption_num)  // must be > 0 
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
#define TRY_VERSION 0x0002000C

#include <stdio.h>
#include <setjmp.h>
#include <assert.h>
#include <stdint.h>
#include <errno.h>

typedef struct try_jb_s {
  jmp_buf          jb;  // Jump buffer for setjmp/longjmp
  struct try_jb_s *pv;  // Link to parent for nested try
  const char      *fn;  // Filename
  int              ln;  // Line number
  unsigned short   ex;  // Exception number
  short            nn;  // Counter
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
    unsigned short ex = x; int ey = y;\
    if (try_jmp_list == NULL) try_abort(); \
    if (ex > 0) {\
      try_jmp_list->fn  = fname; \
      try_jmp_list->ln  = line; \
      if (ey>0) errno = ey; \
      longjmp(try_jmp_list->jb, ex); \
    }\
  } while(0)


#define try_INIT     {.pv = try_jmp_list, .nn = 0, .fn = NULL, .ln = 0}

#define try          for ( try_jb_t try_jb = try_INIT; \
                          (try_jb.nn-- <= 0) && (try_jmp_list = &try_jb); \
                           try_jmp_list = try_jb.pv, try_jb.nn = (try_jb.ex == 0? 2 : try_jb.nn)) \
                            if (try_jb.nn < -1) try_abort(); \
                       else if (((try_jb.ex = setjmp(try_jb.jb)) == 0)) 

#define catch__1(x)     else if ((try_jb.ex == (x)) && (try_jmp_list=try_jb.pv, try_jb.nn=2)) 
#define catch__0( )     else for (try_jmp_list=try_jb.pv; try_jb.nn < 0; try_jb.nn=2) 

#define catch__cnt(x,y,z,a ...) a
#define catch__argn(...)       catch__cnt(__VA_ARGS__, 2, 0, 1)
#define catch__comma(...)      ,
#define catch__cat2(x, y,...)  x ## y
#define catch__join(x, y)      catch__cat2(x, y)

#define catch(...) catch__join(catch__ , catch__argn(catch__comma __VA_ARGS__ ()))(__VA_ARGS__)

#define throw(x,...)  try_throw(x, __VA_ARGS__ -0, __FILE__, __LINE__)

#define rethrow()    try_throw(try_jb.ex, errno, __FILE__, __LINE__)
#define thrown()     try_jb.ex
#define thrownfile() try_jb.fn
#define thrownline() try_jb.ln

// #define leave(e)    for(int e_=!!(e); e_; e_=-1) if (e_<0) goto leave; else

#endif
