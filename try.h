/*
**  (C) by Remo Dentato (rdentato@gmail.com)
** 
** This software is distributed under the terms of the MIT license:
**  https://opensource.org/licenses/MIT
**     
**
**  Full documentation on https://github.com/rdentato/try
**  Discord server: https://discord.gg/QFzP9vaR8j
*/

#ifndef TRY_VERSION // 0.3.1-rc
#define TRY_VERSION    0x0003001C

#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <stdlib.h>

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
  jmp_buf          jmp_buffer;     
  volatile struct try_jb_s *prev_jmpbuf;    // Link to parent try block for nested try
  volatile int     exception_num;
  volatile int     caught;
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

static inline int try_abort() {abort(); return 1;}

#ifndef tryabort
#define tryabort() (fprintf(stderr,"ERROR: Unhandled exception %d. %s:%d\n",\
                            exception.exception_num,exception.file_name,exception.line_num))
#endif

#define try          for ( try_jb_t try_jb = {.exception_num = 0, .prev_jmpbuf = try_jmp_list, .caught = -1 }; \
                          (try_jb.exception_num > 0 && !try_jb.caught) ? (tryabort(), try_abort()) \
                                                                       : ((try_jb.caught++ < 0) && (try_jmp_list = &try_jb)); \
                           try_jmp_list = (try_jb_t *)(try_jb.prev_jmpbuf)) \
                       if (!setjmp(try_jb.jmp_buffer)) 

#define catch__1(x)    else if ((try_jb.exception_num == (x)) && (try_jb.caught=1)) 
#define catch__0( )    else if (!(try_jb.caught=1)) ; else

#define catch__cnt(x,y,z,N, ...) N
#define catch__argn(...)       catch__cnt(__VA_ARGS__, 2, 0, 1)
#define catch__comma(...)      ,
#define catch__cat2(x, y,...)  x ## y
#define catch__join(x, y)      catch__cat2(x, y)

#define catch(...) catch__join(catch__ , catch__argn(catch__comma __VA_ARGS__ ()))(__VA_ARGS__)

#define throw(exc, ...) \
  do { \
    memset(&exception,0,sizeof(exception_t)); \
    exception = ((exception_t){exc, __LINE__, __FILE__, __VA_ARGS__});\
    if (try_jmp_list == NULL) { tryabort(); abort(); } \
    try_jmp_list->exception_num = exception.exception_num; \
    if (exception.exception_num > 0) longjmp(try_jmp_list->jmp_buffer, exception.exception_num); \
  } while(0)

// Pass the same exception to parent try/catch block
#define rethrow(...) do {try_jmp_list=(try_jb_t *)(try_jb.prev_jmpbuf); throw(try_jb.exception_num, __VA_ARGS__);} while(0)

// Quit a try/block in a clean way
#define leave()  continue;

#endif  // TRY_VERSION
