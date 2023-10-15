/*  (C) by:         Remo Dentato (rdentato@gmail.com)
**  License:        https://opensource.org/licenses/MIT
**  Documentation:  https://github.com/rdentato/try
**  Discord server: https://discord.gg/QFzP9vaR8j
*/

#ifndef TRY_VERSION // 0.3.3-rc
#define TRY_VERSION    0x0003003C

#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef exception_info  // Additional fileds for the exception
#define exception_info 
#endif

typedef struct exception_s {
   int   exception_num;
   int   line_num;
   char *file_name;
   exception_info;  // single semicolon (when the macro is empty) allowed by ISO/IEC 9899:1999 (C99) §6.7.2.1
} exception_t;

typedef struct try_ctx_s {                    // Context variables for a try block
                    jmp_buf  jmp_buffer;     
  volatile struct try_ctx_s *prev_ctx;        // Link to the parent context for nested try blocks
  volatile              int  exception_num;   // Non-zero if an exception has been thrown in this context
  volatile              int  caught;          // Non-zero if an exception has been caught in this context
} try_ctx_t;

// If you compiler has a different keyword for thread local variables, define TRY_THREAD 
// before including `try.h`. Define as empty if there is no support at all.
#ifndef TRY_THREAD
#ifdef _MSC_VER
  #define TRY_THREAD __declspec( thread )
#else
  #define TRY_THREAD __thread
#endif
#endif

extern TRY_THREAD try_ctx_t  *try_ctx_list;
extern TRY_THREAD exception_t exception;

// Declare only ONCE a variable of type `try_t` as if it was an int. For example: try_t catch = 0;
#define try_t TRY_THREAD try_ctx_t *try_ctx_list=NULL; exception_t TRY_THREAD exception; int

#ifndef tryabort
#define tryabort() (fprintf(stderr,"ERROR: Unhandled exception %d. %s:%d\n",\
                            exception.exception_num,exception.file_name,exception.line_num))
#endif

static inline int try_abort() {abort(); return 1;}

#define try        for ( try_ctx_t try_ctx = {.exception_num = 0, .prev_ctx = try_ctx_list, .caught = -1 }; \
                        (try_ctx.exception_num && !try_ctx.caught) ? (tryabort(), try_abort()) \
                                                                   : ((try_ctx.caught++ < 0) && (try_ctx_list = &try_ctx)); \
                         try_ctx_list = (try_ctx_t *)(try_ctx.prev_ctx)) \
                     if (setjmp(try_ctx.jmp_buffer) == 0) 
#define catch__1(x)  else if (catch__check(x) && catch__caught()) 
#define catch__0( )  else if (!catch__caught()) ; else

#define catch__check(x) _Generic((x),  int(*)(int):((int(*)(int))(x))(try_ctx.exception_num), \
                                       default:catch__eq((int)((uintptr_t)(x)),try_ctx.exception_num))

static inline int catch__eq(int x, int e) {return x == e;}

#define catch__caught() (try_ctx_list=(try_ctx_t *)(try_ctx.prev_ctx),try_ctx.caught=1)

#define catch__cnt(x,y,z,N, ...) N
#define catch__argn(...)       catch__cnt(__VA_ARGS__, 2, 0, 1)
#define catch__comma(...)      ,
#define catch__cat2(x, y,...)  x ## y
#define catch__join(x, y)      catch__cat2(x, y)

#define catch(...) catch__join(catch__, catch__argn(catch__comma __VA_ARGS__ ()))(__VA_ARGS__)

// To be consistent with setjmp/longjmp behaviour, if `exc` is 0, it is set to 1.
#define throw(exc, ...) \
  do { \
    memset(&exception,0,sizeof(exception_t)); \
    exception = ((exception_t){exc, __LINE__, __FILE__, __VA_ARGS__});\
    if (exception.exception_num == 0) exception.exception_num = 1; \
    if (try_ctx_list == NULL) { tryabort(); abort(); } \
    try_ctx_list->exception_num = exception.exception_num; \
    longjmp(try_ctx_list->jmp_buffer, exception.exception_num); \
  } while(0)

// Pass the same exception to parent try/catch block
#define rethrow(...) throw(try_ctx.exception_num, __VA_ARGS__);

// Quit a try/block in a clean way
#define leave() continue;

#endif  // TRY_VERSION
