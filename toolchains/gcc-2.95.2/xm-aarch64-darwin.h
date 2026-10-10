/* Local host-only bridge for a native little-endian Apple Silicon build.
   Independently authored; does not alter target instruction/ABI definitions. */
#define FALSE 0
#define TRUE 1
#define HOST_BITS_PER_CHAR 8
#define HOST_BITS_PER_SHORT 16
#define HOST_BITS_PER_INT 32
#define HOST_BITS_PER_LONG 64
#define HOST_BITS_PER_LONGLONG 64
#define SUCCESS_EXIT_CODE 0
#define FATAL_EXIT_CODE 33
#include "tm.h"
#ifndef alloca
#define alloca __builtin_alloca
#endif
