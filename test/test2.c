#include <stdio.h>
#include <stdlib.h>

#include "trytest.h"

#define exception_info int err;

#include "try.h"


int function_1(int exc)
{
   if (exc) throw(exc,1);
   return exc;
}

try_t catch = 0;

int main(int argc,char *argv[])
{

  try {
    function_1(EX_NONE);
    tstout("PASS: No Exception");
  }
  catch(EX_OUTOFMEM) {
    tstout("FAIL: GOT EXCEPTION %d.", exception.exception_num);
  }
  catch() {
    tstout("FAIL: GOT EXCEPTION %d. (all)", exception.exception_num);
  }

  try {
    function_1(EX_OUTOFMEM);
    tstout("FAIL: No Exception!");
  }
  catch(EX_OUTOFMEM) {
    tstout("%s: GOT EXCEPTION %d.", tstpass(exception.exception_num == EX_OUTOFMEM), exception.exception_num);
  }
  catch() {
    tstout("FAIL: GOT EXCEPTION %d. (all)", exception.exception_num);
  }

  try {
    function_1(EX_NOFILE);
    tstout("FAIL: No Exception!");
  }
  catch(EX_OUTOFMEM) {
    tstout("FAIL: GOT EXCEPTION %d.", exception.exception_num);
  }
  catch() {
    tstout("PASS: GOT EXCEPTION %d. (all)", exception.exception_num);
  }

  exit(0);
}