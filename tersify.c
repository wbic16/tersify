#include <stdio.h>
#include "tersify.h"

int main(int argc, char** argv)
{
   if (argc < 2)
   {
      printf("Usage: ./tersify <archive> [file(s)]... [-s <text>]\n");
      return 1;
   }

   if (argc >= 1)
   {
      printf("Arg 0: %s", argv[0]);
   }

   return 0;
}