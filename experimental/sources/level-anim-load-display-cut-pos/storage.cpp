#include "storage.hpp"
// Observed displayCutPos at 0x00484348 in .sdata: one word cleared by cutStartScene.
// Inferred: the stored width is the whole extent; the int type follows that width only.
// Unknown: other readers and writers outside cutStartScene.
extern "C" {
int displayCutPos = 0;
}
