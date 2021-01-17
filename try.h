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
  catchall {  // if not handled ...
     ... code ...
  }
                 
]]] */

#ifndef TRY_VERSION
#define TRY_VERSION 0x0001000C
#define TRY_VERSION_STR "try 0.1.0rc"

#include <stdio.h>
#include <setjmp.h>
#include <assert.h>
#include <stdint.h>

typedef struct try_jb_s {
  jmp_buf          jb;  // Jump buffer for setjmp/longjmp
  struct try_jb_s *pv;  // Link to parent for nested try
  const char      *fn;  // Filename 
  int              ln;  // Line number
  int              ex;  // Exception number
  int16_t          id;  // Auxiliary information
  int16_t          nn;  // Counter
} try_jb_t;

#ifdef _MSC_VER
  #define TRY_THREAD __declspec( thread )
#else
  #define TRY_THREAD __thread
#endif

extern TRY_THREAD try_jb_t *try_jmp_list;
extern char const *try_emptystring;

#define try_t  char const *try_emptystring = ""; \
               TRY_THREAD try_jb_t *try_jmp_list=NULL;\
               int

#ifdef TRY_MAIN
  try_t try__;

  #define CATCH_HANDLER 0
  int try_abort() { assert(CATCH_HANDLER);}

  int try_throw(int x, int y, char *fname, int line) 
  {
    if (try_jmp_list == NULL) try_abort(); 
    if (x > 0) {\
      try_jmp_list->fn  = fname; \
      try_jmp_list->ln  = line;
      try_jmp_list->id  = y;
      longjmp(try_jmp_list->jb, x);
    } 
    return x;
  }

#endif


#define try_INIT     {.pv = try_jmp_list, .nn = 0, .fn = try_emptystring, .ln = 0, .id = 0}

#define try          for ( try_jb_t try_jb = try_INIT; \
                          (try_jb.nn-- <= 0) && (try_jmp_list = &try_jb); \
                           try_jmp_list = try_jb.pv, try_jb.nn = (try_jb.ex == 0? 2 : try_jb.nn)) \
                            if (try_jb.nn < -1) try_abort(); \
                       else if (((try_jb.ex = setjmp(try_jb.jb)) == 0)) 

#define catch(x)       else if ((try_jb.ex == (x)) && (try_jmp_list=try_jb.pv, try_jb.nn=2)) 

#define catchall       else for ( try_jmp_list=try_jb.pv; try_jb.nn < 0; try_jb.nn=2) 

int try_throw(int x, int y, char *fname, int line);
int try_abort();
  
#define try_exp(x) x
#define try_0(x,...)   x
#define try_1(x,y,...) y
#define throw(...)  try_throw(try_exp(try_0(__VA_ARGS__,1)), \
                              try_exp(try_1(__VA_ARGS__,0,0)),\
                               __FILE__, __LINE__)

#define rethrow()    try_throw(try_jb.ex, try_jb.id, __FILE__, __LINE__)
#define thrown()     try_jb.ex
#define thrownid()   try_jb.id
#define thrownfile() try_jb.fn
#define thrownline() try_jb.ln

// #define leave(e)    for(int e_=!!(e); e_; e_=-1) if (e_<0) goto leave; else

#endif
