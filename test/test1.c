#include <stdio.h>
#include <stdlib.h>

#include "try.h"

try_main;

int main(int argc,char *argv[])
{

  try {
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
    throw(EXCEPTION(0));
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
    throw(EXCEPTION(1));
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