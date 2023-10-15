
#include <stdio.h>
#include <stdlib.h>

#include "trytest.h"

// These are the additional information you can add to an exception.
// Define `exception_info` as a list of fields definition separated
// by semicolon.
#define exception_info  int foo; char *bar;

// The `exception` object will help you retrieving those information

#include "try.h"

try_t catch=0;

#define EX_FILE                 0x1000
#define EX_FILE_NOT_FOUND       EX_FILE | 0x01
#define EX_FILE_NOT_WRITEABLE   EX_FILE | 0x02
#define EX_FILE_NOT_READABLE    EX_FILE | 0x03
#define EX_FILE_IS_DIRECOTRY    EX_FILE | 0x04

int ex_file(int x) 
{
  return (x & EX_FILE) == EX_FILE;
}

int main(int argc,char *argv[])
{
  try {
    throw(EX_FILE_NOT_FOUND);
    tstout("FAIL: Shouldn't be here in try");
  }
  catch(EX_FILE_NOT_FOUND) {
    tstout("PASS: Got 'EX_FILE_NOT_FOUND'");
  }
  catch(ex_file) {
    tstout("FAIL: Shouldn't be here in ex_file");
  }
  catch() {
    tstout("FAIL: Shouldn't be here in catch");
  }

  try {
    throw(EX_FILE_NOT_READABLE);
    tstout("FAIL: Shouldn't be here in try");
  }
  catch(EX_FILE_NOT_FOUND) {
    tstout("FAIL: Shouldn't have caught 'EX_FILE_NOT_FOUND'");
  }
  catch(ex_file) {
    tstout("PASS: Got some EX_FILE exception");
  }
  catch() {
    tstout("FAIL: Shouldn't be here in catch");
  }

try {
    throw(EX_OUTOFMEM);
    tstout("FAIL: Shouldn't be here in try");
  }
  catch(EX_FILE_NOT_FOUND) {
    tstout("FAIL: Shouldn't have caught 'EX_FILE_NOT_FOUND'");
  }
  catch(ex_file) {
    tstout("FAIL: Shouldn't be here in ex_file");
  }
  catch() {
    tstout("PASS: Got some non-EX_FILE exception");
  }


  #if 0
  #endif
  exit(0);
}
