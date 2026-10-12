#include "storage.hpp"
// Observed cutSkipMode at 0x00484334 in .sdata: one byte cleared by cutStartScene.
// Inferred: the stored width is the whole extent; the unsigned char type follows that width only.
// Unknown: other readers and writers outside cutStartScene.
extern "C" {
unsigned char cutSkipMode = 0;
}
