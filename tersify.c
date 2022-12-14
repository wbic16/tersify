#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "tersify.h"

#define BUFFER_SIZE 1024

int verbose = 0;
int main(int argc, char** argv)
{
   if (argc < 2)
   {
      printf("Usage: ./tersify <archive> -x [file(s)]... [-s <text>]\n");
      printf("  By default trsi slurps the given files into the named Terse Text file.\n");
      printf("  You can supply the -x option to switch to extract mode.\n");
      printf("  Annotation scrolls can be provided inline with the -s option and a quoted string.\n");
      return 1;
   }

   printf("trsi 0.0.3\n(c) 2022 Will Bickford\n");
   char* terse = argv[1];
   FILE* fpa = fopen(terse, "w+");
   // FILE* fs = fopen(summary, "w+");
   printf(": %s\n", argv[1]);
   char summary[1000000];
   char buffer[BUFFER_SIZE];
   memset(buffer, 0, BUFFER_SIZE);

   int counter = 1;
   int mode = 0;
   char* next;
   char* text;
   long file_size;
   int bytes_read;
   long page_number = 0;
   long chunk_size = 10;
   int chunks = 0;
   FILE* fd;
   while (++counter < argc)
   {
      next = argv[counter];
      if (next[0] == '-' && next[1] == 'v')
      {
         verbose = 1;
         continue;
      }
      if (next[0] == '-' && next[1] == 'x')
      {
         mode = 1;
         break;
      }
      if (next[0] == '-' && next[1] == 's')
      {
         ++counter;
         text = argv[counter];
         fwrite(text, 1, strlen(text), fpa);
         fwrite(LINE_BREAK, 1, 1, fpa);
         fwrite(SECTION_BREAK, 1, 1, fpa);
         continue;
      }
      if (next == NULL) { break; }
      fd = fopen(next, "r");
      file_size = 0;
      fwrite(next, 1, strlen(next), fpa);
      fwrite("\n", 1, 1, fpa);
      if (fd)
      {
         bytes_read = -1;
         do
         {
            bytes_read = fread(buffer, 1, BUFFER_SIZE, fd);
            file_size += bytes_read;
            ++page_number;
            if (++chunks > chunk_size)
            {
               chunks = 0;
               int block_written = 0;
               for (int i = 0; i < bytes_read - 1; ++i)
               {
                  if (buffer[i] == LINE_BREAK[0])
                  {
                     // i = 0: 1024 - 1
                     // i = 5: 
                     fwrite(buffer, 1, i + 1, fpa);
                     fwrite(SCROLL_BREAK, 1, 1, fpa);
                     fwrite(buffer + i + 1, 1, bytes_read - (i+1), fpa);
                     block_written = 1;
                     break;
                  }
               }
               if (block_written == 0)
               {
                  fwrite(buffer, 1, bytes_read, fpa);
                  fwrite(SCROLL_BREAK, 1, 1, fpa);
               }
            }
            else
            {
               fwrite(buffer, 1, bytes_read, fpa);
            }
         } while(bytes_read > 0);
         fwrite(SECTION_BREAK, 1, 1, fpa);
         page_number = 0;
         fclose(fd);
      }
      if (verbose > 0) {
         printf(" * %s: %ld\n", next, file_size);
      }
   }

   fclose(fpa);
   // fclose(fs);

   if (mode == 1)
   {
      long file_number = 1;
      char output[1000];
      sprintf(output, "%s-scroll-%ld.txt", terse, file_number);
      printf("Extracting %s...\n", terse);
      fpa = fopen(terse, "r");
      if (verbose) { printf(" * %s", output); }
      fd = fopen(output, "w+");
      int split_file = 0;
      do
      {
         bytes_read = fread(buffer, 1, BUFFER_SIZE, fd);
         for (int i = 0; i < bytes_read; ++i)
         {
            if (buffer[i] == SCROLL_BREAK[0])
            {
               fwrite(buffer, 1, i+1, fd);
               fclose(fd);
               sprintf(output, "%s-scroll-%ld.txt", terse, file_number);
               if (verbose) { printf(" * %s", output); }
               fd = fopen(output, "w+");
               split_file = i+1;
            }
         }
         if (split_file == 0)
         {
            fwrite(buffer, 1, bytes_read, fd);
         }
         else
         {
            char* offset = buffer + split_file + 1;
            int length = bytes_read - split_file - 1;
            if (length > 0)
            {
               fwrite(offset, 1, length, fd);
            }
         }
      } while(bytes_read > 0);
      fclose(fd);
      fclose(fpa);
   }
   return 0;
}
