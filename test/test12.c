#include <stdio.h>
#include <stdlib.h>

#include "trytest.h"

#include "try.h"

try_t trymain = 1;

int main(int argc,char *argv[])
{
  // 1. Catch ordering: first matching specific catch wins,
  //    later catches (even for same exception) are skipped.
  try {
    throw(42);
    tstout("FAIL: Should not reach");
  }
  catch(42) {
    tstout("PASS: First matching catch wins");
  }
  catch(42) {
    tstout("FAIL: Second catch for same exception should not fire");
  }
  catch() {
    tstout("FAIL: Catch-all should not fire when specific matched");
  }

  // 2. Specific catch after a non-matching catch works correctly
  try {
    throw(99);
    tstout("FAIL: Should not reach");
  }
  catch(10) {
    tstout("FAIL: Wrong catch matched");
  }
  catch(20) {
    tstout("FAIL: Wrong catch matched");
  }
  catch(99) {
    tstout("PASS: Specific catch after non-matching catches works");
  }
  catch() {
    tstout("FAIL: Catch-all should not fire");
  }

  // 3. Catch-all fires only when no specific catch matches
  try {
    throw(77);
    tstout("FAIL: Should not reach");
  }
  catch(10) {
    tstout("FAIL: Wrong catch matched");
  }
  catch(20) {
    tstout("FAIL: Wrong catch matched");
  }
  catch(30) {
    tstout("FAIL: Wrong catch matched");
  }
  catch() {
    tstout("PASS: Catch-all fires when no specific match");
  }

  // 4. New throw() from inside a catch block (not rethrow).
  //    The new exception should propagate to the outer try.
  try {
    try {
      throw(10);
      tstout("FAIL: Should not reach");
    }
    catch() {
      tstout("PASS: Inner catch caught exception %d", catch.exception);
      throw(20);   // brand new throw, not rethrow
      tstout("FAIL: Should not reach after throw in catch");
    }
    tstout("FAIL: Should not reach after inner catch threw");
  }
  catch() {
    tstout("%s: Outer catch got exception %d (expected 20)",
           tstpass(catch.exception == 20), catch.exception);
  }

  // 5. rethrow() with no arguments preserves the original exception code
  try {
    try {
      throw(55);
      tstout("FAIL: Should not reach");
    }
    catch() {
      tstout("PASS: Inner catch caught %d", catch.exception);
      rethrow();
      tstout("FAIL: Should not reach after rethrow");
    }
    tstout("FAIL: Should not reach after inner rethrow");
  }
  catch() {
    tstout("%s: rethrow() preserved exception %d (expected 55)",
           tstpass(catch.exception == 55), catch.exception);
  }

  // 6. Multiple levels of new throw from catch (catch → throw → catch → throw)
  try {
    try {
      try {
        throw(1);
        tstout("FAIL: level 1 should not reach");
      }
      catch() {
        tstout("PASS: Level 1 catch got %d", catch.exception);
        throw(2);
        tstout("FAIL: level 1 after throw");
      }
      tstout("FAIL: level 2 should not reach");
    }
    catch() {
      tstout("PASS: Level 2 catch got %d", catch.exception);
      throw(3);
      tstout("FAIL: level 2 after throw");
    }
    tstout("FAIL: level 3 should not reach");
  }
  catch() {
    tstout("%s: Outer catch got %d (expected 3)",
           tstpass(catch.exception == 3), catch.exception);
  }

  exit(0);
}
