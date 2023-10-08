#include <stdio.h>
#include <stdlib.h>

#include "trytest.h"

#define tryabort() my_handler()

#include "try.h"

void my_handler()
{
  fprintf(stderr,"PASS: Unhandled exception %d @ %s:%d\n",exception.exception_num, exception.file_name, exception.line_num);
}

try_t catch = 0;

int main(int argc,char *argv[])
{
  try {
    tstout("PASS: Throwing an unhandled exception will abort.");
    throw(EX_OUTOFMEM);
    tstout("FAIL: SHOULD NOT BE HERE.");
  }
  catch(EX_NODB) {
    tstout("FAIL: GOT EXCEPTION %d.", thrown());
  }
  exit(1);
}