// Observed: Init__t7VIArray1Z20VICSpritePartEmitteri at 0x012082c8, 92 bytes. Arguments (array, count). It calls
// Clear__t7VIArray1Z20VICSpritePartEmitter(array); a negative result returns -1. Otherwise it stores count at array+0, calls
// Allocate__t7VIArray1Z20VICSpritePartEmitter(array) (the store sits in the call's delay slot, so it happens before the call) and returns
// -1 when that is negative, zero otherwise.
// Inferred: the first word is the element count, as for the other VIArray instantiations.
extern "C" {
int Clear__t7VIArray1Z20VICSpritePartEmitter(void* array);
int Allocate__t7VIArray1Z20VICSpritePartEmitter(void* array);

int Init__t7VIArray1Z20VICSpritePartEmitteri(void* array, int count) {
    if (Clear__t7VIArray1Z20VICSpritePartEmitter(array) < 0) return -1;
    *static_cast<int*>(array) = count;
    return Allocate__t7VIArray1Z20VICSpritePartEmitter(array) < 0 ? -1 : 0;
}
}
