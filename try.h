/*  (C) by:         Remo Dentato (rdentato@gmail.com)
**  License:        https://opensource.org/licenses/MIT
**  Documentation:  https://github.com/rdentato/try
**  Discord server: https://discord.gg/QFzP9vaR8j
*/

#ifndef TRY_VERSION // 0.4.1-rc
#define TRY_VERSION    0x0004001C

#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef catch_info  // Additional fields for the exception
#define catch_info 
#endif

typedef struct exception_s {
   int   exception;
   int   line;
   char *filename;
   catch_info
} catch_t;

typedef struct try_ctx_s {                    // Context variables for a try block
                    jmp_buf  jmp_buffer;     
  volatile struct try_ctx_s *prev_ctx;        // Link to the parent context for nested try blocks
  volatile              int  exception;   // Non-zero if an exception has been thrown in this context
  volatile              int  caught;          // Non-zero if an exception has been caught in this context
} try_ctx_t;

// If your application uses threads, define `TRY_USE_THREADS` before including `try.h`. 
// If your compiler uses a different syntax to indicate thread-local variables, define `TRY_THREAD` accordingly.
#ifndef TRY_USE_THREADS
  #define TRY_THREAD
#else
  #ifndef TRY_THREAD
    #ifdef _MSC_VER
      #define TRY_THREAD __declspec( thread )
    #else
      #define TRY_THREAD __thread
    #endif
  #endif
#endif

extern TRY_THREAD try_ctx_t  *try_ctx_list;
extern TRY_THREAD catch_t catch;

// Declare only ONCE a variable of type `try_t` as if it was an int. For example: try_t trymain = 1;
#define try_t try_abort_default TRY_THREAD try_ctx_t *try_ctx_list=NULL; catch_t TRY_THREAD catch = {0}; int 
//#define trymain TRY_THREAD try_ctx_t *try_ctx_list=NULL; catch_t TRY_THREAD catch = {0};  

int try_abort();

#define try_abort_default \
    int try_abort() { \
      fprintf(stderr,"ERROR: Unhandled exception %d. %s:%d\n",\
      catch.exception,catch.filename,catch.line); \
      return 1; \
    }

#ifndef tryabort
#define tryabort try_abort 
#endif

#define try  for ( try_ctx_t try_ctx = {.exception = 0, .prev_ctx = try_ctx_list, .caught = -1 }; \
                  (try_ctx.exception && !try_ctx.caught)   ? \
                                     (tryabort() && catch_abort()) : \
                                     ((try_ctx.caught++ < 0) && (try_ctx_list = &try_ctx)); \
                   try_ctx_list = (try_ctx_t *)(try_ctx.prev_ctx)) \
               if (setjmp(try_ctx.jmp_buffer) == 0) 

#define catch(...)   else if (catch__check(__VA_ARGS__ +0, try_ctx.exception) && catch__caught()) 

static inline int catch_abort() {abort(); return 1;}

static inline int catch__check(int x, int e) {return (x == 0) || (x == e);}

#define catch__caught() (try_ctx_list=(try_ctx_t *)(try_ctx.prev_ctx),try_ctx.caught=1)


// To be consistent with setjmp/longjmp behaviour, if `exc` is 0, it is set to 1.
#define throw(exc, ...) \
  do { \
    memset(&catch,0,sizeof(catch_t)); \
    catch = ((catch_t){exc, __LINE__, __FILE__, __VA_ARGS__});\
    if (catch.exception == 0) catch.exception = 1; \
    if (try_ctx_list == NULL) { if (tryabort(&catch)) abort(); } \
    try_ctx_list->exception = catch.exception; \
    longjmp(try_ctx_list->jmp_buffer, catch.exception); \
  } while(0)

// Pass the same exception to parent try/catch block
#define rethrow(...) throw(try_ctx.exception, __VA_ARGS__)

// Quit a try/block in a clean way
#define leave() continue

#endif  // TRY_VERSION
