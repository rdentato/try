#include <stdio.h>
#include <stdlib.h>

#include "trytest.h"

// These are the additional information you can add to an exception.
// Define `exception_info` as a list of fields definition separated
// by semicolon.
#define exception_info   int  foo; char *bar;

// The `exception` object will help you retrieving those information


#include "try.h"

try_t catch = 0;
  int seq = 7100;

int main(int argc,char *argv[])
{

  try {
    int value = 71;
    // The additiona information can be specified in the same order
    // they have been defined.
    throw(EX_OUTOFMEM, value, "NONO");
    tstout("FAIL: Shouldn't be here in try");
  }
  catch(EX_OUTOFMEM) {
    tstout("%s: foo == %d, bar == \"%s\" risen @ %s:%d",tstpass(exception.foo == 71),exception.foo, exception.bar, exception.file_name, exception.line_num);
  }
  catch() {
    tstout("FAIL: Shouldn't be here in catch");
  }

  try {
    // You can specify additional information by referring to the info field:
    throw(EX_OUTOFMEM, .bar = "OPS!");
    tstout("FAIL: Shouldn't be here in try");
  }
  catch(EX_OUTOFMEM) {
    tstout("%s: foo == %d, bar == \"%s\" risen @ %s:%d",tstpass(exception.foo == 0),exception.foo, exception.bar, exception.file_name, exception.line_num);
  }
  catch() {
    tstout("FAIL: Shouldn't be here in catch");
  }


  try {

    try {
      // Note that if seq was a variable local to main(), this wouldn't
      // work because it would be reset by the `longjmp()`
      throw(EX_NOFILE,.foo = seq++);
      tstout("FAIL: Shouldn't be here in try");
    }
    catch() {
      tstout("%s: Sequnce incremented: %d", 
             tstpass(seq == 7101), seq);
      tstout("%s: Nested handler seq: %d (seq = %d)",
             tstpass(exception.exception_num == EX_NOFILE),
             exception.foo, seq);
      rethrow(.foo=seq++);
    }
  }
  catch() {
      tstout("%s: Outer handler seq: %d",
             tstpass(exception.foo == 7101),
             exception.foo);
  }


  exit(0);
}
