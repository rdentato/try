#include <stdio.h>
#include <stdlib.h>

#include "trytest.h"

#define catch_info int err;

#include "try.h"


int function_1(int exc)
{
   if (exc) throw(exc,1);
   return exc;
}

try_t trymain = 1;

int main(int argc,char *argv[])
{

  try {
    function_1(EX_NONE);
    tstout("PASS: No Exception");
  }
  catch(EX_OUTOFMEM) {
    tstout("FAIL: GOT EXCEPTION %d.", catch.exception);
  }
  catch() {
    tstout("FAIL: GOT EXCEPTION %d. (all)", catch.exception);
  }

  try {
    function_1(EX_OUTOFMEM);
    tstout("FAIL: No Exception!");
  }
  catch(EX_OUTOFMEM) {
    tstout("%s: GOT EXCEPTION %d.", tstpass(catch.exception == EX_OUTOFMEM), catch.exception);
  }
  catch() {
    tstout("FAIL: GOT EXCEPTION %d. (all)", catch.exception);
  }

  try {
    function_1(EX_NOFILE);
    tstout("FAIL: No Exception!");
  }
  catch(EX_OUTOFMEM) {
    tstout("FAIL: GOT EXCEPTION %d.", catch.exception);
  }
  catch() {
    tstout("PASS: GOT EXCEPTION %d. (all)", catch.exception);
  }

  exit(0);
}