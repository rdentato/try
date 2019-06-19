#include <stdio.h>
#include <stdlib.h>

#include "trytest.h"
#include "try.h"

try_t maintry;

int function_1(int exc)
{
   if (exc) {
     try {
       throw(exc,1);
       tstout("FAIL: No Exception!");
     } 
     catchall {
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
     catchall {
       tstout("PASS: Exception caught");
       rethrow();
       tstout("FAIL: Should be re-rised");
     }
     tstout("FAIL: Should be re-rised");
   }
   return exc;
}

int main(int argc,char *argv[])
{
  try {
    try {
      tstout("PASS: Trying something else");
      function_1(EX_OUTOFMEM);  
      tstout("PASS: Should have handled the exception!");
    }
    catchall {
      tstout("FAIL: GOT EXCEPTION %d. (nested)", thrown());
    }
    tstout("PASS: Try completed");
  }
  catchall {
    tstout("FAIL: GOT EXCEPTION %d. (main)", thrown());
  }

  try {
    try {
      tstout("PASS: Trying something else");
      function_1(EX_OUTOFMEM);  
      tstout("PASS: handled the exception!");
    }
    catchall {
      tstout("FAIL: GOT EXCEPTION %d. (nested)", thrown());
      rethrow();
      tstout("FAIL: Should have rised an exception!");
    }
  }
  catchall {
    tstout("PASS: GOT EXCEPTION %d. (main)", thrown());
  }

  try {
    try {
      tstout("PASS: Trying something else");
      function_2(EX_OUTOFMEM);  
      tstout("FAIL: Should have raised the exception!");
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
      function_2(EX_OUTOFMEM);  
      tstout("FAIL: Should have handled the exception!");
    }
    catchall {
      tstout("PASS: GOT EXCEPTION %d. (nested)", thrown());
      rethrow();
      tstout("FAIL: Should have rised an exception!");
    }
  }
  catchall {
    tstout("PASS: GOT EXCEPTION %d. (main)", thrown());
  }

  exit(0);
}
