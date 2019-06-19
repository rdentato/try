#include <stdio.h>
#include <stdlib.h>

#include "trytest.h"
#include "try.h"

try_t maintry;

int main(int argc,char *argv[])
{

  try {
    try {
      tstout("PASS: Trying something else");
      throw(EX_OUTOFMEM);  
      tstout("FAIL: Should have rised an exception!");
    }
    catchall {
      tstout("PASS: GOT EXCEPTION %d. (nested)", thrown());
    }
    tstout("PASS: Try completed");
  }
  catchall {
    tstout("FAIL: GOT EXCEPTION %d. (main)", thrown());
  }

  try {
    try {
      tstout("PASS: Trying something else");
      throw(EX_OUTOFMEM);  
      tstout("FAIL: Should have rised an exception!");
    }
    catchall {
      tstout("PASS: GOT EXCEPTION %d. (nested)", thrown());
      rethrow();
      tstout("FAIL: Should have rised an exception!");
    }
    tstout("FAIL: Try completed?");
  }
  catchall {
    tstout("PASS: GOT EXCEPTION %d. (main)", thrown());
  }


  exit(0);
}