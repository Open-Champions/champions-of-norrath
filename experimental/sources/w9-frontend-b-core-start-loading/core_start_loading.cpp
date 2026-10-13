// Observed: coreStartLoading__Fv at 0x002d0d98 is 20 bytes. It stores 10 in
// coreLoading and copies drawFrameNm into coreLoadingStart. It makes no call.
// Inferred: nothing.
// Unknown: what the value 10 means to the loading code that reads coreLoading.

extern "C" int drawFrameNm;
extern "C" int coreLoading;
extern "C" int coreLoadingStart;

extern "C" void coreStartLoading__Fv() {
    coreLoading = 10;
    coreLoadingStart = drawFrameNm;
}
