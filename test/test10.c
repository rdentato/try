#include <stdio.h>
#include <stdlib.h>

#include "trytest.h"

#include "try.h"

try_t trymain = 1;

int main(int argc,char *argv[])
{
  // throw(0) must become exception 1 (setjmp/longjmp semantics)
  try {
    throw(0);
    tstout("FAIL: Should not reach here");
  }
  catch() {
    tstout("%s: throw(0) -> exception == 1", tstpass(catch.exception == 1));
  }

  // throw(1) must stay 1, not conflated with the 0→1 promotion
  try {
    throw(1);
    tstout("FAIL: Should not reach here");
  }
  catch() {
    tstout("%s: throw(1) -> exception == 1", tstpass(catch.exception == 1));
  }

  // Non-zero, non-one values are passed through unchanged
  try {
    throw(42);
    tstout("FAIL: Should not reach here");
  }
  catch() {
    tstout("%s: throw(42) -> exception == 42", tstpass(catch.exception == 42));
  }

  // Large value
  try {
    throw(65535);
    tstout("FAIL: Should not reach here");
  }
  catch() {
    tstout("%s: throw(65535) -> exception == 65535", tstpass(catch.exception == 65535));
  }

  exit(0);
}
