#include "storage.hpp"
// Observed netWeSentReady at 0x004843a8 in .sdata: cleared by cutStartScene next to netNmReady; no reader is reconstructed.
// Inferred: the stored width is the whole extent; the int type follows that width only.
// Unknown: other readers and writers outside cutStartScene.
extern "C" {
int netWeSentReady = 0;
}
