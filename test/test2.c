#include <stdio.h>
#include <stdlib.h>

#include "trytest.h"

#define TRY_MAIN
#include "try.h"


int function_1(int exc)
{
   if (exc) throw(exc,1);
   return exc;
}

int main(int argc,char *argv[])
{

  try {
    function_1(EX_NONE);
    tstout("PASS: No Exception");
  }
  catch(EX_OUTOFMEM) {
    tstout("FAIL: GOT EXCEPTION %d.", thrown());
  }
  catchall {
    tstout("FAIL: GOT EXCEPTION %d. (all)", thrown());
  }

  try {
    function_1(EX_OUTOFMEM);
    tstout("FAIL: No Exception!");
  }
  catch(EX_OUTOFMEM) {
    tstout("PASS: GOT EXCEPTION %d.", thrown());
  }
  catchall {
    tstout("FAIL: GOT EXCEPTION %d. (all)", thrown());
  }

  try {
    function_1(EX_NOFILE);
    tstout("FAIL: No Exception!");
  }
  catch(EX_OUTOFMEM) {
    tstout("FAIL: GOT EXCEPTION %d.", thrown());
  }
  catchall {
    tstout("PASS: GOT EXCEPTION %d. (all)", thrown());
  }

  exit(0);
}