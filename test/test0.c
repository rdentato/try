#include <stdio.h>
#include <stdlib.h>

#include "try.h"

try_main;

int main(int argc,char *argv[])
{

  try {
    throw(EXCEPTION_01);
    printf("NO ERROR\n");
    fflush(stdout);
  }
  catch(EXCEPTION_00) {
    printf("EXCEPTION 0\n");
    fflush(stdout);
  }

  exit(0);
}