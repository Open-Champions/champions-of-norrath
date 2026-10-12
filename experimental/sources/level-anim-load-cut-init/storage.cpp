#include "storage.hpp"
// Observed cutInit at 0x0048431a in .sdata: one byte cleared by cutStartScene.
// Inferred: the stored width is the whole extent; the unsigned char type follows that width only.
// Unknown: other readers and writers outside cutStartScene.
extern "C" {
unsigned char cutInit = 0;
}
