// Observed: five 4-byte counters, all zero in the file:
//   nmActors        0x00486a10
//   nmAlwaysPages   0x00486984
//   nmChars         0x00486998
//   nmHalos         0x00484088
//   nmWaterPatches  0x00484130
// Their users load them as plain ints (the water initializer reads nmWaterPatches, the
// advance-frame and cut-scene routines read nmActors).
// Inferred: none beyond the names the image's symbols give. Unknown: the producers of
// nmChars and nmAlwaysPages among the not-yet-authored code.
extern "C" {
int nmActors = 0;
int nmAlwaysPages = 0;
int nmChars = 0;
int nmHalos = 0;
int nmWaterPatches = 0;
}
static_assert(sizeof(nmActors) == 4 && sizeof(nmWaterPatches) == 4, "observed sizes");
