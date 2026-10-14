// Observed: Clear__11VIClipStack at 0x010994b0 is 128 bytes. It calls, in order,
// Clear__t7VIArray1Z11VIClipEntry on offset 4, Clear__t7VIArray1Z7VIVect3 on
// offset 16, Clear__t7VIArray1Z7VIPlane on offset 28, then Clear__t7VIArray1Z7VIVect3
// on offsets 36 and 44. A negative result from any call returns -1 at once;
// otherwise it returns 0 (the last result is tested the same way).
// Inferred: the five members are the arrays Init sizes (500, 2000, 2000, 32, 32).
extern "C" {
int Clear__t7VIArray1Z11VIClipEntry(void* array);
int Clear__t7VIArray1Z7VIVect3(void* array);
int Clear__t7VIArray1Z7VIPlane(void* array);

int Clear__11VIClipStack(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    if (Clear__t7VIArray1Z11VIClipEntry(bytes + 4) < 0) return -1;
    if (Clear__t7VIArray1Z7VIVect3(bytes + 16) < 0) return -1;
    if (Clear__t7VIArray1Z7VIPlane(bytes + 28) < 0) return -1;
    if (Clear__t7VIArray1Z7VIVect3(bytes + 36) < 0) return -1;
    if (Clear__t7VIArray1Z7VIVect3(bytes + 44) < 0) return -1;
    return 0;
}
}
