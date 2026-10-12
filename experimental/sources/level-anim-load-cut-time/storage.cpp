#include "storage.hpp"
// Observed cutTime at 0x00486a20 in .sbss: one word cleared by cutStartScene; the symbol size is 0 and the next symbol is 8 bytes on, so only the stored word is claimed.
// Inferred: the stored width is the whole extent; the int type follows that width only.
// Unknown: other readers and writers outside cutStartScene.
extern "C" {
int cutTime = 0;
}
