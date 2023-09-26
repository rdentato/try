#include <stdio.h>
#include <stdlib.h>

#include "trytest.h"

#include "try.h"

try_t catch = 0;

int main(int argc,char *argv[])
{

  try {
    try {
      tstout("PASS: Trying something else");
      throw(EX_OUTOFMEM);  
      tstout("FAIL: Should have rised an exception!");
    }
    catch() {
      tstout("PASS: GOT EXCEPTION %d. (nested)", thrown());
    }
    tstout("PASS: Try completed");
  }
  catch() {
    tstout("FAIL: GOT EXCEPTION %d. (main)", thrown());
  }

  try {
    try {
      tstout("PASS: Trying something else");
      throw(EX_OUTOFMEM);  
      tstout("FAIL: Should have rised an exception!");
    }
    catch() {
      tstout("PASS: GOT EXCEPTION %d. (nested)", thrown());
      rethrow();
      tstout("FAIL: Should have rised an exception!");
    }
    tstout("FAIL: Try completed?");
  }
  catch() {
    tstout("PASS: GOT EXCEPTION %d. (main)", thrown());
  }


  exit(0);
}