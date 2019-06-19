#include <stdio.h>
#include <stdlib.h>

#include "trytest.h"
#include "try.h"

try_t maintry;

int main(int argc,char *argv[])
{

  try {
    tstout("PASS: No Exception");
  }
  catch(24) {
    tstout("FAIL: GOT EXCEPTION %d.", thrown());
  }
  catchall {
    tstout("FAIL: GOT EXCEPTION %d. (all)", thrown());
  }

  try {
    throw(24);
    tstout("FAIL: Should not be here\n");
  }
  catch(24) {
    tstout("PASS: GOT EXCEPTION %d.", thrown());
  }
  catchall {
    tstout("FAIL: GOT EXCEPTION %d. (all)", thrown());
  }

  try {
    throw(25);
    tstout("FAIL: Should not be here\n");
  }
  catch(24) {
    tstout("FAIL: GOT EXCEPTION %d.", thrown());
  }
  catchall {
    tstout("PASS: GOT EXCEPTION %d. (all)", thrown());
  }
  exit(0);
}