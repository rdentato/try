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
 try {
    printf("Try something else\n");
    fflush(stdout);
    try {
       function_1(EXCEPTION_00);  
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
       function_1(EXCEPTION_00);  
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

 try {
    printf("Try something else\n");
    fflush(stdout);
    try {
       function_2(EXCEPTION_00);  
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
       function_2(EXCEPTION_00);  
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
