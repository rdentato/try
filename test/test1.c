#include <stdio.h>
#include <stdlib.h>

#include "trytest.h"

#include "try.h"

try_t catch = 0;

int main(int argc,char *argv[])
{

  try {
    tstout("PASS: No Exception");
  }
  catch(24) {
    tstout("FAIL: GOT EXCEPTION %d.", exception.exception_num);
  }
  catch() {
    tstout("FAIL: GOT EXCEPTION %d. (all)", exception.exception_num);
  }

  try {
    throw(24);
    tstout("FAIL: Should not be here\n");
  }
  catch(24) {
    tstout("PASS: GOT EXCEPTION %d from: %s:%d.", exception.exception_num,exception.file_name, exception.line_num);
  }
  catch() {
    tstout("FAIL: GOT EXCEPTION %d from: %s:%d.", exception.exception_num,exception.file_name, exception.line_num);
  }

  try {
    throw(25);
    tstout("FAIL: Should not be here\n");
  }
  catch(24) {
    tstout("FAIL: GOT EXCEPTION %d.", exception.exception_num);
  }
  catch() {
    tstout("PASS: GOT DEFAULT EXCEPTION %d from: %s:%d.", exception.exception_num,exception.file_name, exception.line_num);
  }
  exit(0);
}