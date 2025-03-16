#include <stdio.h>
#include <stdlib.h>

#include "trytest.h"

#include "try.h"

try_t trymain = 1;

int main(int argc,char *argv[])
{

  try {
    tstout("PASS: No Exception");
  }
  catch(24) {
    tstout("FAIL: GOT EXCEPTION %d.", catch.exception);
  }
  catch() {
    tstout("FAIL: GOT EXCEPTION %d. (all)", catch.exception);
  }

  try {
    throw(24);
    tstout("FAIL: Should not be here\n");
  }
  catch(24) {
    tstout("PASS: GOT EXCEPTION %d from: %s:%d. ", catch.exception,catch.filename, catch.line);
  }
  catch() {
    tstout("FAIL: GOT EXCEPTION %d from: %s:%d. ", catch.exception,catch.filename, catch.line);
  }

  try {
    throw(25);
    tstout("FAIL: Should not be here\n");
  }
  catch(24) {
    tstout("FAIL: GOT EXCEPTION %d.", catch.exception);
  }
  catch() {
    tstout("PASS: GOT DEFAULT EXCEPTION %d from: %s:%d.", catch.exception,catch.filename, catch.line);
  }
  exit(0);
}