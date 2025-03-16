#include <stdio.h>
#include <stdlib.h>

#include "trytest.h"

// These are the additional information you can add to an exception.
// Define `catch_info` as a list of fields definition separated
// by semicolon.
#define catch_info  int foo; char *bar;

// The `exception` object will help you retrieving those information

#include "try.h"

try_t trymain = 1;
  int seq = 7100;

int main(int argc,char *argv[])
{

  try {
    int value = 71;
    // The additional information can be specified in the same order
    // they have been defined.
    throw(EX_OUTOFMEM, value, "NONO");
    tstout("FAIL: Shouldn't be here in try");
  }
  catch(EX_OUTOFMEM) {
    tstout("%s: foo == %d, bar == \"%s\" risen @ %s:%d",tstpass(catch.foo == 71),catch.foo, catch.bar, catch.filename, catch.line);
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
    tstout("%s: foo == %d, bar == \"%s\" risen @ %s:%d",tstpass(catch.foo == 0),catch.foo, catch.bar, catch.filename, catch.line);
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
             tstpass(catch.exception == EX_NOFILE),
             catch.foo, seq);
      rethrow(.foo=seq++);
    }
  }
  catch() {
      tstout("%s: Outer handler seq: %d",
             tstpass(catch.foo == 7101),
             catch.foo);
  }


  exit(0);
}
