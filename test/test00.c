#include <stdio.h>
#include <stdlib.h>

#include "trytest.h"

#define tryabort(e) my_handler(e)

#include "try.h"

void my_handler(exception_t *e)
{
  fprintf(stderr,"PASS: Unhandled exception %d @ %s:%d\n",e->exception_num, e->file_name, e->line_num);
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