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
#define TRY_VERSION 0x0003000C

#include <stdio.h>
#include <setjmp.h>
#include <assert.h>
#include <errno.h>

#ifndef exception_info
#define exception_info 
#endif

typedef struct exception_s {
   int   exception_num;
   int   line_num;
   char *file_name;
   exception_info;  // single semicolon allowed by ISO/IEC 9899:1999 (C99) §6.7.2.1
} exception_t;


typedef struct try_jb_s {
  jmp_buf          jmp_buffer;     // Jump buffer for setjmp/longjmp
  struct try_jb_s *prev_jmpbuf;    // Link to parent for nested try
  exception_t     *info;
  int              exception_num;  // Exception number
  int              count;          // Counter
} try_jb_t;

#ifdef _MSC_VER
  #define TRY_THREAD __declspec( thread )
#else
  #define TRY_THREAD __thread
#endif

extern TRY_THREAD try_jb_t *try_jmp_list;

#define try_t TRY_THREAD try_jb_t *try_jmp_list=NULL; int

#define CATCH_HANDLER 0

#define try_abort(e) assert(CATCH_HANDLER)

#ifndef tryabort
#define tryabort try_abort
#endif

#define try_ENV     {.prev_jmpbuf = try_jmp_list, .count = 0}

#define try          for ( try_jb_t try_jb = try_ENV; \
                          (try_jb.count-- <= 0) && (try_jmp_list = &try_jb); \
                           try_jmp_list = try_jb.prev_jmpbuf, try_jb.count = (try_jb.exception_num == 0? 2 : try_jb.count)) \
                            if (try_jb.count < -1) { tryabort(try_jb.info); abort(); }\
                       else if (((try_jb.exception_num = setjmp(try_jb.jmp_buffer)) == 0)) 

#define catch__1(x)    else if ((try_jb.exception_num == (x)) && (try_jmp_list=try_jb.prev_jmpbuf, try_jb.count=2)) 
#define catch__0( )    else for (try_jmp_list=try_jb.prev_jmpbuf; try_jb.count < 0; try_jb.count=2) 

#define catch__cnt(x,y,z,N, ...) N
#define catch__argn(...)       catch__cnt(__VA_ARGS__, 2, 0, 1)
#define catch__comma(...)      ,
#define catch__cat2(x, y,...)  x ## y
#define catch__join(x, y)      catch__cat2(x, y)

#define catch(...) catch__join(catch__ , catch__argn(catch__comma __VA_ARGS__ ()))(__VA_ARGS__)

#define throw(exception_num_,...)  try_throw(exception_num_, __LINE__, __FILE__, __VA_ARGS__)

// The only mandatory argument for throw() is the exception number. 
// If `exception_errno` is greater than 0, it will be assigned as value to `errno`.
// You can use it to provide further information about the exception.

#define try_throw(exception_num_, line_num_ , file_name_, ...) \
  do { \
    unsigned short exception_num = exception_num_;\
    static exception_t exc = {0}; \
    exc = ((exception_t){.exception_num = exception_num, .line_num=line_num_ , .file_name=file_name_, __VA_ARGS__});\
    if (try_jmp_list == NULL) try_abort(); \
    if (exception_num > 0) {\
      try_jmp_list->info = &exc;\
      longjmp(try_jmp_list->jmp_buffer, exception_num); \
    }\
  } while(0)

#define rethrow(...)       try_throw( thrown(), __LINE__, __FILE__, __VA_ARGS__)
#define thrown()           (exception->exception_num)
#define thrownexception()  thrown() 
#define thrownfile()       (exception->file_num)
#define thrownline()       (exception->file_num)

#define leave(e)  if (!(try_jb.count = 2)); else continue;

#define exception  (try_jb.info)

#endif
