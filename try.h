/*
**  (C) by Remo Dentato (rdentato@gmail.com)
** 
** This software is distributed under the terms of the MIT license:
**  https://opensource.org/licenses/MIT
**     

[[[
# Exceptions

 Simple implementation of try/catch. try blocks can be nested.
 Exceptions are one of the the 30 possible exception numbered from 0 to 30
 
  #define OUTOFMEM    EXCEPTION_00
  #define WRONGINPUT  EXCEPTION_01
  #define INTERNALERR EXCEPTION_02
  
  try {
     ... code ...
     if (something_failed) throw(execption_num)  // must be > 0 
     some_other_func(); // you can trhow exceptions from other functions too 
     ... code ...
  }  
  catch(OUTOFMEM) {
     ... code ...
  }
  catch(WRONGINPUT | INTERNALERR) {
     ... code ...
  }
  catchall {  // if not handled ...
     ... code ...
  }
                 
]]] */


#ifndef __TRY_H__
#define __TRY_H__
#include <setjmp.h>
#include <assert.h>

typedef struct try_jb_s {
  jmp_buf           jmp;
  struct try_jb_s  *prv;
  const char       *fn;
  int               ln;
  int               ex;
  int16_t           id;
  int16_t           nn;
} try_jb_t;


#ifdef _MSC_VER
  #define TRY_THREAD __declspec( thread )
#else
  #define TRY_THREAD __thread
#endif

extern TRY_THREAD try_jb_t *try_jmp_list;
extern char *try_emptystring;

#define try_main   TRY_THREAD try_jb_t *try_jmp_list=NULL; \
                   char *try_emptystring = ""

#ifdef TRY_MAIN
  try_main;
#endif

#define CATCH_HANDLER 0

#define try_INIT     {.prv = try_jmp_list, .nn = 0, .fn = try_emptystring, .ln = 0, .id = 0}

#define try          for ( try_jb_t try_jb = try_INIT; \
                          (try_jb.nn-- <= 0) && (try_jmp_list = &try_jb); \
                           try_jmp_list = try_jb.prv, try_jb.nn = (try_jb.ex == 0? 2 : try_jb.nn)) \
                            if (try_jb.nn < -1) assert(CATCH_HANDLER); \
                       else if (((try_jb.ex = setjmp(try_jb.jmp)) == 0)) 

#define catch(x)       else if ((try_jb.ex & (x)) && (try_jmp_list=try_jb.prv, try_jb.nn=2)) 

#define catchall       else for ( try_jmp_list=try_jb.prv; try_jb.nn < 0; try_jb.nn=2) 

#define throw2(x,y) do { int ex_ = x; \
                         if (ex_ > 0 && try_jmp_list) {\
                           try_jmp_list->fn  = __FILE__; \
                           try_jmp_list->ln  = __LINE__;\
                           try_jmp_list->id  = (y);\
                           longjmp(try_jmp_list->jmp, ex_); \
                         } \
                    } while (0)

#define try_0(x,...)   x
#define try_1(x,y,...) y
#define throw(...)   throw2(try_0(__VA_ARGS__,1), try_1(__VA_ARGS__,0,0))

#define rethrow()    throw2(try_jb.ex,try_jb.id)
#define thrown()     try_jb.ex
#define thrownid()   try_jb.id
#define thrownfile() try_jb.fn
#define thrownline() try_jb.ln

#define EXCEPTION(n)  ((1<<(n &0x1F)) & 0x7FFFFFFF)

#endif
