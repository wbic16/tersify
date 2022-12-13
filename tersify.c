#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "tersify.h"

#define BUFFER_SIZE 1048576

int verbose = 0;
int main(int argc, char** argv)
{
   if (argc < 2)
   {
      printf("Usage: ./tersify <archive> [file(s)]... [-s <text>]\n");
      return 1;
   }

   printf("Tersify 0.0.2\n");
   char* archive = argv[1];
   FILE* fpa = fopen(archive, "w+");
   // FILE* fs = fopen(summary, "w+");
   printf("Archive: %s\n", argv[1]);
   char summary[1000000];
   char buffer[BUFFER_SIZE];
   memset(buffer, 0, BUFFER_SIZE);

   int counter = 1;
   char* next;
   char* text;
   long file_size;
   FILE* fd;
   while (++counter < argc)
   {
      next = argv[counter];
      if (next[0] == '-' && next[1] == 'v')
      {
         verbose = 1;
         continue;
      }
      if (next[0] == '-' && next[1] == 's')
      {
         ++counter;
         text = argv[counter];
         fwrite(text, 1, strlen(text), fpa);
         fwrite(LINE_BREAK, 1, 1, fpa);
         fwrite(SCROLL_BREAK, 1, 1, fpa);
         continue;
      }
      if (next == NULL) { break; }
      fd = fopen(next, "r");
      file_size = 0;
      fwrite(next, 1, strlen(next), fpa);
      fwrite("\n", 1, 1, fpa);
      if (fd)
      {
         int bytes_read = -1;
         do
         {
            bytes_read = fread(buffer, 1, BUFFER_SIZE, fd);
            file_size += bytes_read;
            fwrite(buffer, 1, bytes_read, fpa);
            fwrite("\n", 1, 1, fpa);
         } while(bytes_read > 0);
         fwrite(SCROLL_BREAK, 1, 1, fpa);
         fclose(fd);
      }
      if (verbose > 0) {
         printf(" * %s: %ld\n", next, file_size);
      }
   }

   fclose(fpa);
   // fclose(fs);
   return 0;
}
