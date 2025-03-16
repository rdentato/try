#include <stdio.h>
#include <stdlib.h>

#include "trytest.h"

#define catch_info int err;
#include "try.h"


int function_1(int exc)
{
   if (exc) {
     try {
       throw(exc,1);
       tstout("FAIL: No Exception!");
     } 
     catch() {
       tstout("PASS: Exception %d caught (err=%d)", catch.exception, catch.err);
     }
     tstout("PASS: Should not be re-rised");
   }
   return exc;
}

int function_2(int exc)
{
   if (exc) {
     try {
       throw(exc,2);
       tstout("FAIL: No Exception!");
     } 
     catch() {
       tstout("PASS: Exception %d caught (err=%d)", catch.exception, catch.err);
       rethrow(3);
       tstout("FAIL: Should be re-rised");
     }
     tstout("FAIL: Should be re-rised");
   }
   return exc;
}

try_t trymain = 1;

int main(int argc,char *argv[])
{
  int ret = 0;

  try {
    ret = function_1(EX_OUTOFMEM);
    tstout("PASS: No Exception (function return: %d)",ret);
  }
  catch() {
    tstout("FAIL: GOT EXCEPTION %d,%d. (all)", catch.exception, catch.err);
  }

  try {
    ret = function_2(EX_NOFILE);
    tstout("FAIL: No Exception (function return: %d)",ret);
  }
  catch() {
    tstout("PASS: GOT EXCEPTION %d,%d. (all)", catch.exception, catch.err);
  }

  exit(0);
}