#include "storage.hpp"
// Observed dialogActor at 0x00486a14 in .sbss: one word cleared by cutStartScene; the symbol size is 0, so only the stored word is claimed.
// Inferred: the stored width is the whole extent; the int type follows that width only.
// Unknown: other readers and writers outside cutStartScene.
extern "C" {
int dialogActor = 0;
}
