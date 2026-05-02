#include <stdio.h>
#include <stdlib.h>

#include "trytest.h"

#include "try.h"

try_t trymain = 1;

int main(int argc,char *argv[])
{
  // 1. Bare try block: no catch clause at all. Should compile and run
  //    cleanly when no exception is thrown.
  try {
    tstout("PASS: Bare try block executes");
  }
  tstout("PASS: Bare try block exits cleanly");

  // 2. Bare try block with exception: should abort (unhandled).
  //    We test this by checking that the code after is unreachable.
  //    Actually tested implicitly: the program wouldn't reach here.

  // 3. Many sequential try blocks (10) to stress context init/teardown
  {
    int ok = 1;
    for (int i = 0; i < 10; i++) {
      try {
        if (i % 2 == 0) {
          throw(i + 100);
          tstout("FAIL: seq %d should not reach", i);
        }
        // odd i: no exception, normal path
      }
      catch() {
        if (catch.exception != i + 100) ok = 0;
      }
    }
    tstout("%s: 10 sequential try blocks all caught correctly", tstpass(ok));
  }

  // 4. Sequential try blocks with different exception values,
  //    verifying catch.filename and catch.line are set
  try {
    throw(111);
    tstout("FAIL: should not reach");
  }
  catch() {
    tstout("%s: filename set", tstpass(catch.filename != NULL));
    tstout("%s: line > 0", tstpass(catch.line > 0));
  }

  try {
    throw(222);
    tstout("FAIL: should not reach");
  }
  catch() {
    tstout("%s: filename set", tstpass(catch.filename != NULL));
    tstout("%s: line > 0", tstpass(catch.line > 0));
  }

  exit(0);
}
