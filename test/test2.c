#include <stdio.h>
#include <stdlib.h>

#define TRY_MAIN
#include "try.h"

int function_1(int exc)
{
   if (exc) throw(exc,1);
   return exc;
}

int main(int argc,char *argv[])
{

  try {
    function_1(0);
    printf("NO ERROR\n");
    fflush(stdout);
  }
  catch(EXCEPTION(0)) {
    printf("EXCEPTION 0\n");
    fflush(stdout);
  }
  catchall {
    printf("EXCEPTION %08X\n",thrown());
    fflush(stdout);
  }

  try {
    function_1(EXCEPTION(0));
    printf("NO ERROR\n");
    fflush(stdout);
  }
  catch(EXCEPTION(0)) {
    printf("EXCEPTION 0\n");
    fflush(stdout);
  }
  catchall {
    printf("EXCEPTION %08X\n",thrown());
    fflush(stdout);
  }

  try {
    function_1(EXCEPTION(1));
    printf("NO ERROR\n");
    fflush(stdout);
  }
  catch(EXCEPTION(0)) {
    printf("EXCEPTION 0\n");
    fflush(stdout);
  }
  catchall {
    printf("EXCEPTION %08X\n",thrown());
    fflush(stdout);
  }

  exit(0);
}