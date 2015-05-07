// MemoryFree library based on code posted here:
// http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1213583720/15
// Extended by Matthew Murdoch to include walking of the free list.

#include "memdbg.h"
#include <avr/io.h>

extern unsigned int __heap_start;
extern void* __brkval;

struct __freelist {
        size_t             sz;
        struct __freelist* nx;
};

extern struct __freelist* __flp;


static size_t
_free_list_size()
{
        struct __freelist* cur;
        size_t total = 0;

        for (cur = __flp; cur != NULL; cur = cur->nx)
        {
                total += 2; /* block header size */
                total += cur->sz;
        }

        return total;
}

size_t
mem_used()
{
        size_t used;
        struct __freelist* cur;

        if (__brkval == 0)
        {
                return 0;
        }

        used = (size_t)__brkval - (size_t)__malloc_heap_start - _free_list_size();

        return used;
}

size_t
mem_avail()
{
        size_t avail;
        if ((size_t)__brkval == 0)
        {
                avail = ((size_t) &avail) - ((size_t) &__heap_start);
        }
        else
        {
                avail = ((size_t) &avail) - ((size_t) __brkval);
                avail += _free_list_size();
        }

        return avail;
}
