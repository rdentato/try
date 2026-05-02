#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trytest.h"

// Define extra exception fields
#define catch_info int err; char *msg;

#include "try.h"

try_t trymain = 1;

int main(int argc,char *argv[])
{
  // 1. throw() with no extra args when catch_info is defined:
  //    the memset in throw() should zero all extra fields.
  try {
    throw(100);
    tstout("FAIL: Should not reach");
  }
  catch() {
    tstout("%s: err zeroed (got %d)", tstpass(catch.err == 0), catch.err);
    tstout("%s: msg zeroed (got %p)", tstpass(catch.msg == NULL), (void*)catch.msg);
  }

  // 2. throw() with partial extra info (named initializer):
  //    unspecified fields should be zero.
  try {
    throw(200, .msg = "hello");
    tstout("FAIL: Should not reach");
  }
  catch() {
    tstout("%s: err zero (got %d)", tstpass(catch.err == 0), catch.err);
    tstout("%s: msg set to \"hello\"",
           tstpass(catch.msg != NULL && strcmp(catch.msg, "hello") == 0));
  }

  // 3. throw() with all extra fields specified
  try {
    throw(300, .err = 42, .msg = "world");
    tstout("FAIL: Should not reach");
  }
  catch() {
    tstout("%s: err == 42 (got %d)", tstpass(catch.err == 42), catch.err);
    tstout("%s: msg == \"world\"",
           tstpass(catch.msg != NULL && strcmp(catch.msg, "world") == 0));
  }

  // 4. rethrow() with no extra args: memset zeroes the extra fields,
  //    but the exception code is preserved.
  try {
    try {
      throw(400, .err = 99, .msg = "inner");
      tstout("FAIL: Should not reach");
    }
    catch() {
      tstout("%s: inner err=%d msg=%s",
             tstpass(catch.err == 99 && catch.msg != NULL
                     && strcmp(catch.msg, "inner") == 0),
             catch.err, catch.msg ? catch.msg : "NULL");
      rethrow();
      tstout("FAIL: Should not reach after rethrow");
    }
    tstout("FAIL: Should not reach after inner catch");
  }
  catch() {
    tstout("%s: rethrow() cleared err (got %d)", tstpass(catch.err == 0), catch.err);
    tstout("%s: rethrow() cleared msg (got %p)", tstpass(catch.msg == NULL), (void*)catch.msg);
    tstout("%s: exception preserved (got %d)", tstpass(catch.exception == 400), catch.exception);
  }

  // 5. rethrow() with new extra info: should carry the new values.
  try {
    try {
      throw(500, .err = 1, .msg = "original");
      tstout("FAIL: Should not reach");
    }
    catch() {
      rethrow(.err = 2, .msg = "updated");
      tstout("FAIL: Should not reach");
    }
    tstout("FAIL: Should not reach");
  }
  catch() {
    tstout("%s: rethrow with new err=%d (expected 2)",
           tstpass(catch.err == 2), catch.err);
    tstout("%s: rethrow with new msg=\"updated\"",
           tstpass(catch.msg != NULL && strcmp(catch.msg, "updated") == 0));
    tstout("%s: exception preserved (got %d, expected 500)",
           tstpass(catch.exception == 500), catch.exception);
  }

  exit(0);
}
