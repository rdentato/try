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
  int16_t           flg;
} try_jb_t;

extern try_jb_t *try_jmp_list;
extern char *try_emptystring;

#define try_main   try_jb_t *try_jmp_list=NULL; \
                   char *try_emptystring = ""

#ifdef TRY_MAIN
  try_main;
#endif
#define CATCH_HANDLER 0

#define try_INIT  {.prv = try_jmp_list, .flg = 0, .ex =0, \
                   .fn = try_emptystring, .ln = 0, .id = 0 }

#define try          for ( try_jb_t try_jb = try_INIT; \
                          (try_jb.flg-- <= 0) && (try_jmp_list = &try_jb); \
                           try_jmp_list = try_jb.prv, try_jb.flg = (try_jb.ex == 0? 2 : try_jb.flg)) \
                            if (try_jb.flg < -1) assert(CATCH_HANDLER); \
                       else if (((try_jb.ex = setjmp(try_jb.jmp)) == 0)) 

#define catch(x)       else if ((try_jb.ex & (x)) && \
                                 (try_jmp_list=try_jb.prv, try_jb.flg=2)) 

#define catchall       else for ( try_jmp_list=try_jb.prv; \
                                  try_jb.flg == -1; try_jb.flg=2) 

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
#define throw(...)   throw2(try_0(__VA_ARGS__,-1),\
                            try_1(__VA_ARGS__,0,0))

#define rethrow()    throw2(try_jb.ex,try_jb.id)
#define thrown()     try_jb.ex
#define thrownid()   try_jb.id
#define thrownfile() try_jb.fn
#define thrownline() try_jb.ln

#define EXCEPTION_00 0x00000001
#define EXCEPTION_01 0x00000002
#define EXCEPTION_02 0x00000004
#define EXCEPTION_03 0x00000008
#define EXCEPTION_04 0x00000010
#define EXCEPTION_05 0x00000020
#define EXCEPTION_06 0x00000040
#define EXCEPTION_07 0x00000080
#define EXCEPTION_08 0x00000100
#define EXCEPTION_09 0x00000200
#define EXCEPTION_10 0x00000400
#define EXCEPTION_11 0x00000800
#define EXCEPTION_12 0x00001000
#define EXCEPTION_13 0x00002000
#define EXCEPTION_14 0x00004000
#define EXCEPTION_15 0x00008000
#define EXCEPTION_16 0x00010000
#define EXCEPTION_17 0x00020000
#define EXCEPTION_18 0x00040000
#define EXCEPTION_19 0x00080000
#define EXCEPTION_20 0x00100000
#define EXCEPTION_21 0x00200000
#define EXCEPTION_22 0x00400000
#define EXCEPTION_23 0x00800000
#define EXCEPTION_24 0x01000000
#define EXCEPTION_25 0x02000000
#define EXCEPTION_26 0x04000000
#define EXCEPTION_27 0x08000000
#define EXCEPTION_28 0x10000000
#define EXCEPTION_29 0x20000000
#define EXCEPTION_30 0x40000000


#endif
