// MemoryFree library based on code posted here:
// http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1213583720/15
// Extended by Matthew Murdoch to include walking of the free list.

#include <stdlib.h>

#ifndef MEMDBG_H_
#define MEMDBG_H_

#ifdef __cplusplus
extern "C" {
#endif

size_t
mem_used();

size_t
mem_avail();

#ifdef __cplusplus
}
#endif

#endif // !MEMDBG_H_
