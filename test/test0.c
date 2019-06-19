#include <stdio.h>
#include <stdlib.h>

#include "trytest.h"
#include "try.h"

try_t maintry;

int main(int argc,char *argv[])
{
  try {
    tstout("PASS: Throwing an unhandled exception will abort.");
    throw(EX_OUTOFMEM);
    tstout("FAIL: SHOULD NOT BE HERE.");
  }
  catch(EX_NODB) {
    tstout("FAIL: GOT EXCEPTION %d.", thrown());
  }
  exit(1);
}