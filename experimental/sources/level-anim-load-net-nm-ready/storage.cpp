#include "storage.hpp"
// Observed netNmReady at 0x004843a4 in .sdata: the count of cutscene-ready messages seen; cutMsgHandlerFunction adds one and cutStartScene clears it.
// Inferred: the stored width is the whole extent; the int type follows that width only.
// Unknown: other readers and writers outside cutStartScene.
extern "C" {
int netNmReady = 0;
}
