#include "nbio.h"
#include <string.h>
#include <stdio.h>

static void nbio_write_test(void)
{
   size_t size, prog;
   bool looped = false;
   void* ptr = NULL;
   struct nbio_t* write = nbio_open("test.bin", NBIO_WRITE);

   nbio_resize(write, 1024*1024);

   ptr = nbio_get_ptr(write, &size);
   if (size != 1024*1024)
      puts("ERROR: wrong size (1)");

   memset(ptr, 0x42, 1024*1024);
   nbio_begin_write(write);

   while (!nbio_iterate(write, &prog, &size))
   {
      printf("%u/%u\n", (unsigned)prog, (unsigned)size);
      looped=true;
   }

   if (!looped)
      puts("Write finished immediately?");

   nbio_free(write);
}

static void nbio_read_test(void)
{
   size_t size, prog;
   bool looped = false;
   struct nbio_t* read = nbio_open("test.bin", NBIO_READ);
   void *ptr           = nbio_get_ptr(read, &size);

   if (size != 1024*1024)
      puts("ERROR: wrong size (2)");
   if (ptr)
      puts("Read pointer is available before iterating?");

   nbio_begin_read(read);

   while (!nbio_iterate(read, &prog, &size))
   {
      printf("%u/%u\n", (unsigned)prog, (unsigned)size);
      looped=true;
   }
   if (!looped) puts("Read finished immediately?");

   ptr = nbio_get_ptr(read, &size);
   if (size != 1024*1024)
      puts("ERROR: wrong size (3)");
   if (*(char*)ptr != 0x42 || memcmp(ptr, (char*)ptr+1, 1024*1024-1))
      puts("ERROR: wrong data");

   nbio_free(read);
}

int main(void)
{
   nbio_write_test();
   nbio_read_test();
}
