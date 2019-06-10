#include <stdio.h>
#include <stdlib.h>

#include "try.h"

try_main;

int main(int argc,char *argv[])
{

  try {
    printf("Try something else\n");
    fflush(stdout);
    try {
       throw(EXCEPTION_00);  
    }
    catchall {
      printf("EXCEPTION %08X (nested)\n",thrown());
      fflush(stdout);
    }
    printf("Tried\n");
    fflush(stdout);
  }
  catchall {
    printf("EXCEPTION %08X (main)\n",thrown());
    fflush(stdout);
  }

  try {
    printf("Try something else\n");
    fflush(stdout);
    try {
       throw(EXCEPTION_00);  
    }
    catchall {
      printf("EXCEPTION %08X (nested)\n",thrown());
      fflush(stdout);
      rethrow();
    }
    printf("Tried\n");
    fflush(stdout);
  }
  catchall {
    printf("EXCEPTION %08X (main)\n",thrown());
    fflush(stdout);
  }


  exit(0);
}