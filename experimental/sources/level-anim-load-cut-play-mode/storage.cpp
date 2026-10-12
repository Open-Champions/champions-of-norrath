#include "storage.hpp"
// Observed cutPlayMode at 0x00484319 in .sdata: one byte set to 1 by cutStartScene, between cutMode and cutInit.
// Inferred: the stored width is the whole extent; the unsigned char type follows that width only.
// Unknown: other readers and writers outside cutStartScene.
extern "C" {
unsigned char cutPlayMode = 0;
}
