#include <stdio.h>
#include <stdlib.h>

#include "trytest.h"

#include "try.h"

try_t trymain = 1;

// Helper: create `depth` nested try blocks via recursion.
// If `do_throw` is non-zero, throws `exc` at the innermost level
// and lets it propagate through rethrow at every level.
static int deep_nest(int depth, int do_throw, int exc)
{
  int result = -1;
  if (depth <= 0) {
    if (do_throw) throw(exc);
    return 0;
  }
  try {
    result = deep_nest(depth - 1, do_throw, exc);
  }
  catch() {
    rethrow();
  }
  return result;
}

int main(int argc,char *argv[])
{
  // 1. Deep nesting (10 levels) with no exception:
  //    all contexts should init and teardown cleanly.
  try {
    int r = deep_nest(10, 0, 0);
    tstout("%s: Deep nest (10) no exception, result=%d",
           tstpass(r == 0), r);
  }
  catch() {
    tstout("FAIL: Unexpected exception %d in deep nest", catch.exception);
  }

  // 2. Deep nesting (10 levels) with exception at innermost level.
  //    Exception must propagate through all rethrow layers.
  try {
    deep_nest(10, 1, 777);
    tstout("FAIL: Should not reach after deep nest throw");
  }
  catch() {
    tstout("%s: Deep nest (10) exception %d propagated to top",
           tstpass(catch.exception == 777), catch.exception);
  }

  // 3. Deeper nesting (20 levels) to stress context chain.
  try {
    int r = deep_nest(20, 0, 0);
    tstout("%s: Deep nest (20) no exception, result=%d",
           tstpass(r == 0), r);
  }
  catch() {
    tstout("FAIL: Unexpected exception %d in deep nest (20)", catch.exception);
  }

  // 4. Deeper nesting (20) with exception.
  try {
    deep_nest(20, 1, 999);
    tstout("FAIL: Should not reach");
  }
  catch() {
    tstout("%s: Deep nest (20) exception %d propagated",
           tstpass(catch.exception == 999), catch.exception);
  }

  // 5. Mixed sequential + nested: 5 outer try blocks each containing
  //    a 3-level nested try that rethrows.
  {
    int ok = 1;
    for (int i = 0; i < 5; i++) {
      try {
        deep_nest(3, 1, 100 + i);
        tstout("FAIL: seq-nest %d should not reach", i);
      }
      catch() {
        if (catch.exception != 100 + i) ok = 0;
      }
    }
    tstout("%s: 5 sequential x 3 nested, all propagated correctly", tstpass(ok));
  }

  // 6. Interleaved: try blocks that don't throw mixed with ones that do
  {
    int no_exc_count = 0;
    int exc_count = 0;
    for (int i = 0; i < 10; i++) {
      try {
        if (i % 3 == 0) {
          throw(i);
        }
        no_exc_count++;
      }
      catch() {
        exc_count++;
      }
    }
    tstout("%s: Interleaved: %d no-exc, %d exc",
           tstpass(no_exc_count == 6 && exc_count == 4),
           no_exc_count, exc_count);
  }

  exit(0);
}
