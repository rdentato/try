#include <stdio.h>
#include <stdlib.h>

#include "trytest.h"

#define tryabort my_handler

#include "try.h"

try_t trymain = 1;

int my_handler()
{
  fprintf(stderr,"PASS: Unhandled (managed) exception %d @ %s:%d\n",catch.exception, catch.filename, catch.line);
  return 0; // Don't abort
}


int main(int argc,char *argv[])
{
  try {
    tstout("PASS: Throwing an unhandled exception will NOT abort.");
    throw(EX_OUTOFMEM);
    tstout("FAIL: SHOULD NOT BE HERE.");
  }
  catch(EX_NODB) {
    tstout("FAIL: GOT EXCEPTION %d.", catch.exception);
  }
  tstout("PASS: exited cleanly");
  exit(1);
}