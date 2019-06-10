#include <stdio.h>
#include <stdlib.h>

#define TRY_MAIN
#include "try.h"

int function_1(int exc)
{
   if (exc) {
     try {
       throw(exc,1);
     } 
     catchall {
      printf("EXCEPTION %08X in function_1\n",thrown());
      fflush(stdout);   
     }
   }
   return exc;
}

int function_2(int exc)
{
   if (exc) {
     try {
       throw(exc,2);
     } 
     catchall {
      printf("EXCEPTION %08X in function_2\n",thrown());
      fflush(stdout);   
      rethrow();
     }
   }
   return exc;
}

int main(int argc,char *argv[])
{
  int ret = 0;
  try {
    ret = function_1(EXCEPTION_00);
    printf("NO ERROR (%d)\n",ret);
    fflush(stdout);
  }
  catchall {
    printf("EXCEPTION %08X in main\n",thrown());
    fflush(stdout);
  }

  try {
    ret = function_2(EXCEPTION_01);
    printf("NO ERROR (%d)\n",ret);
    fflush(stdout);
  }
  catchall {
    printf("EXCEPTION %08X in main\n",thrown());
    fflush(stdout);
  }


  exit(0);
}