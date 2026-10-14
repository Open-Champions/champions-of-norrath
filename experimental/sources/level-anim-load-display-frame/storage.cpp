#include "storage.hpp"
// Observed displayFrame at 0x00484328 in .sdata: one word set to 1 by cutStartScene.
// Inferred: the stored width is the whole extent; the int type follows that width only.
// Unknown: other readers and writers outside cutStartScene.
extern "C" {
int displayFrame = 0;
}
