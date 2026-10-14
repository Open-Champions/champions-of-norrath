#include "storage.hpp"
// Observed displayActionPos at 0x00484344 in .sdata: one word cleared by cutStartScene.
// Inferred: the stored width is the whole extent; the int type follows that width only.
// Unknown: other readers and writers outside cutStartScene.
extern "C" {
int displayActionPos = 0;
}
