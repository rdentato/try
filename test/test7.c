#include <stdio.h>
#include <stdlib.h>

#include "trytest.h"

// This are the additional information you can add to an exception
// the `exception` 
#define exception_info   int  foo; \
                        char *bar;

#include "try.h"

try_t catch = 0;
  int seq = 7100;

int main(int argc,char *argv[])
{

//  try {
//    int value = 71;
//    throw(EX_OUTOFMEM, value, "NONO");
//    tstout("FAIL: Shouldn't be here in try");
//  }
//  catch(EX_OUTOFMEM) {
//    tstout("%s: foo == %d, bar == \"%s\" risen @ %s:%d",tstpass(exception->foo == 71),exception->foo, exception->bar, exception->file_name, exception->line_num);
//  }
//  catch() {
//    tstout("FAIL: Shouldn't be here in catch");
//  }
//
//  try {
//    // You can specify additional information by referring to the info field:
//    throw(EX_OUTOFMEM, .bar = "OPS!");
//    tstout("FAIL: Shouldn't be here in try");
//  }
//  catch(EX_OUTOFMEM) {
//    tstout("%s: foo == %d, bar == \"%s\" risen @ %s:%d",tstpass(exception->foo == 0),exception->foo, exception->bar, exception->file_name, exception->line_num);
//  }
//  catch() {
//    tstout("FAIL: Shouldn't be here in catch");
//  }
//

  try {

    try {
      // Note that if seq was a function local to main(), this won't
      // work because it would be reset by the `longjmp()`
      throw(ENOMEM,.foo = seq++);
      tstout("FAIL: Shouldn't be here in try");
    }
    catch() {
      tstout("%s: Sequnce incremented: %d", 
             tstpass(seq == 7101), seq);
      tstout("%s: Nested handler seq: %d (seq = %d)",
             tstpass(exception->exception_num == ENOMEM),
             exception->foo, seq);
      rethrow(.foo=seq++);
    }
  }
  catch() {
      tstout("%s: Outer handler seq: %d",
             tstpass(exception->foo == 7101),
             exception->foo);
  }


  exit(0);
}
