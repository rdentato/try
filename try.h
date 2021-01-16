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
     some_other_func(); // you can throw exceptions from other functions too 
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

#ifndef TRY_H__
#define TRY_H__
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
#endif

#define CATCH_HANDLER 0

#define try_INIT     {.pv = try_jmp_list, .nn = 0, .fn = try_emptystring, .ln = 0, .id = 0}

#define try          for ( try_jb_t try_jb = try_INIT; \
                          (try_jb.nn-- <= 0) && (try_jmp_list = &try_jb); \
                           try_jmp_list = try_jb.pv, try_jb.nn = (try_jb.ex == 0? 2 : try_jb.nn)) \
                            if (try_jb.nn < -1) assert(CATCH_HANDLER); \
                       else if (((try_jb.ex = setjmp(try_jb.jb)) == 0)) 

#define catch(x)       else if ((try_jb.ex == (x)) && (try_jmp_list=try_jb.pv, try_jb.nn=2)) 

#define catchall       else for ( try_jmp_list=try_jb.pv; try_jb.nn < 0; try_jb.nn=2) 

#define throw2(x,y)  do { int ex_ = x; \
                       if (ex_ > 0) { \
                         if (!try_jmp_list) assert(CATCH_HANDLER); \
                         try_jmp_list->fn  = __FILE__; \
                         try_jmp_list->ln  = __LINE__;\
                         try_jmp_list->id  = (y);\
                         longjmp(try_jmp_list->jb, ex_); \
                       } \
                     } while (0)

#define tryexp(x) x
#define try_0(x,...)   x
#define try_1(x,y,...) y
#define throw(...)   throw2(tryexp(try_0(__VA_ARGS__,1)), \
                            tryexp(try_1(__VA_ARGS__,0,0)))

#define rethrow()    throw2(try_jb.ex, try_jb.id)
#define thrown()     try_jb.ex
#define thrownid()   try_jb.id
#define thrownfile() try_jb.fn
#define thrownline() try_jb.ln

#endif
