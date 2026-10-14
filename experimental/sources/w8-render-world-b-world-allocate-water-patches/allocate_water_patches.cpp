// Observed worldAllocateWaterPatches__Fi at 0x0017da40, 100 bytes.
// If the pointer at 0x00484138 (waterPatch) is nonnull, it is passed to
// memFree__FPv and both it and the count at 0x00484130 (nmWaterPatches) are
// cleared. Then, if the argument is nonzero, memAlloc__FiPc is called with
// size argument * 4784 and the 14-byte string "water patches" (read from
// 0x004636a0), the result is stored in waterPatch, the argument in
// nmWaterPatches, and waterInit__Fv is called last. Nothing is returned.
// The 4784-byte patch layout is not established here. The stores of the new
// pointer and count happen before waterInit runs. The globals and callees are
// outside this package and stay unresolved imports.
extern "C" {
extern void* waterPatch;
extern int nmWaterPatches;
void memFree__FPv(void* block);
void* memAlloc__FiPc(int size, const char* name);
void waterInit__Fv();
void worldAllocateWaterPatches__Fi(int count) {
    if (waterPatch != nullptr) {
        memFree__FPv(waterPatch);
        waterPatch = nullptr;
        nmWaterPatches = 0;
    }
    if (count != 0) {
        waterPatch = memAlloc__FiPc(count * 4784, "water patches");
        nmWaterPatches = count;
        waterInit__Fv();
    }
}
}
