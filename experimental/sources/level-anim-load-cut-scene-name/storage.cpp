#include "storage.hpp"
// Observed cutSceneName at 0x00790f10 in .bss: cutStartScene copies the cut name here with strcpy.
// The next symbol, anim, starts at 0x00790f60, which bounds the object at 80 bytes.
// Inferred: a NUL-terminated name; the whole 80 bytes are the extent.
// Unknown: other readers.
extern "C" {
char cutSceneName[80] = {};
}
