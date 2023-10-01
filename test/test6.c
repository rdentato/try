#include <stdio.h>
#include <stdlib.h>

#include "trytest.h"

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
    tstout("PASS: Leaving from try block");
    leave();
    tstout("FAIL: Shouldn't be here in try");
  }
  catch() {
    tstout("FAIL: Shouldn't be here in catch");
  }
  tstout("PASS: Left from try");

  try {
    throw(12);
  }
  catch() {
    tstout("PASS: Leaving from  catch block");
    leave();
    tstout("FAIL: Shouldn't be here in catch");
  }
  tstout("PASS: Left from catch");

  // NEsted

  try {

    try {
      throw(43);
      tstout("FAIL: Shouldn't be here in try");
    } 
    catch () {
      leave();
      tstout("FAIL: Shouldn't be here in catch");
    }

    tstout("PASS: Back to parent try");
    throw(12);
    tstout("FAIL: Shouldn't be here in try");

  }
  catch () {
    tstout("PASS: Caught exception");
    leave();
    tstout("FAIL: Shouldn't be here in catch");
  } 
  tstout("PASS: Left from catch");

  exit(0);
}
