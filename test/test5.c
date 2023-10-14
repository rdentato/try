#include <stdio.h>
#include <stdlib.h>

#include "trytest.h"

#define exception_info int err;
#include "try.h"

int function_1(int exc)
{
   if (exc) {
     try {
       throw(exc,1);
       tstout("FAIL: No Exception!");
     } 
     catch() {
       tstout("PASS: Exception caught");
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
       tstout("PASS: Exception caught");
       rethrow();
       tstout("FAIL: Should be re-rised");
     }
     tstout("FAIL: Should be re-rised");
   }
   return exc;
}

try_t catch = 0;

int main(int argc,char *argv[])
{
  try {
    try {
      tstout("PASS: Trying something else");
      function_1(EX_OUTOFMEM);  
      tstout("PASS: Should have handled the exception!");
    }
    catch() {
      tstout("FAIL: GOT EXCEPTION %d. (nested)", exception.exception_num);
    }
    tstout("PASS: Try completed");
  }
  catch() {
    tstout("FAIL: GOT EXCEPTION %d. (main)", exception.exception_num);
  }

  try {
    try {
      tstout("PASS: Trying something else");
      function_1(EX_OUTOFMEM);  
      tstout("PASS: handled the exception!");
    }
    catch() {
      tstout("FAIL: GOT EXCEPTION %d. (nested)", exception.exception_num);
      rethrow();
      tstout("FAIL: Should have rised an exception!");
    }
  }
  catch() {
    tstout("PASS: GOT EXCEPTION %d. (main)", exception.exception_num);
  }

  try {
    try {
      tstout("PASS: Trying something else");
      function_2(EX_OUTOFMEM);  
      tstout("FAIL: Should have raised the exception!");
    }
    catch() {
      tstout("PASS: GOT EXCEPTION %d. (nested)", exception.exception_num);
    }
    tstout("PASS: Try completed");
  }
  catch() {
    tstout("FAIL: GOT EXCEPTION %d. (main)", exception.exception_num);
  }

  try {
    try {
      tstout("PASS: Trying something else");
      function_2(EX_OUTOFMEM);  
      tstout("FAIL: Should have handled the exception!");
    }
    catch() {
      tstout("PASS: GOT EXCEPTION %d. (nested)", exception.exception_num);
      rethrow();
      tstout("FAIL: Should have rised an exception!");
    }
  }
  catch() {
    tstout("PASS: GOT EXCEPTION %d. (main)", exception.exception_num);
  }

  exit(0);
}
