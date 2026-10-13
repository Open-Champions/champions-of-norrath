// Observed: Init__11VIClipStack at 0x01098140 is 144 bytes. It calls, in order,
// Init__t7VIArray1Z11VIClipEntryi(offset 4, 500), Init__t7VIArray1Z7VIVect3i(16, 2000),
// Init__t7VIArray1Z7VIPlanei(28, 2000), Init__t7VIArray1Z7VIVect3i(36, 32) and
// Init__t7VIArray1Z7VIVect3i(44, 32). A negative result from any call returns -1
// at once without the later calls. After all five succeed it calls Reset__11VIClipStack
// on the object and returns 0. The result of the last array call is also tested
// for negative.
extern "C" {
int Init__t7VIArray1Z11VIClipEntryi(void* array, int count);
int Init__t7VIArray1Z7VIVect3i(void* array, int count);
int Init__t7VIArray1Z7VIPlanei(void* array, int count);
int Reset__11VIClipStack(void* object);

int Init__11VIClipStack(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    if (Init__t7VIArray1Z11VIClipEntryi(bytes + 4, 500) < 0) return -1;
    if (Init__t7VIArray1Z7VIVect3i(bytes + 16, 2000) < 0) return -1;
    if (Init__t7VIArray1Z7VIPlanei(bytes + 28, 2000) < 0) return -1;
    if (Init__t7VIArray1Z7VIVect3i(bytes + 36, 32) < 0) return -1;
    if (Init__t7VIArray1Z7VIVect3i(bytes + 44, 32) < 0) return -1;
    Reset__11VIClipStack(object);
    return 0;
}
}
