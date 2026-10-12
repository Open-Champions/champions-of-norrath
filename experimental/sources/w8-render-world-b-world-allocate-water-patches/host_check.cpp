#include <cstring>
extern "C" {
void* waterPatch;
int nmWaterPatches;
void worldAllocateWaterPatches__Fi(int count);
}
static int freed_calls, alloc_calls, init_calls;
static void* freed;
static int alloc_size;
static char alloc_name[32];
static void* init_saw_patch;
static int init_saw_count;
static char backing[4];
extern "C" void memFree__FPv(void* block) { ++freed_calls; freed = block; }
extern "C" void* memAlloc__FiPc(int size, const char* name) {
    ++alloc_calls; alloc_size = size; std::strcpy(alloc_name, name); return backing;
}
extern "C" void waterInit__Fv() { ++init_calls; init_saw_patch = waterPatch; init_saw_count = nmWaterPatches; }
int main() {
    waterPatch = nullptr; nmWaterPatches = 7;
    worldAllocateWaterPatches__Fi(0);
    if (freed_calls || alloc_calls || init_calls || nmWaterPatches != 7) return 1;
    worldAllocateWaterPatches__Fi(3);
    if (freed_calls != 0 || alloc_calls != 1 || init_calls != 1) return 2;
    if (alloc_size != 3 * 4784 || std::strcmp(alloc_name, "water patches") != 0) return 3;
    if (waterPatch != backing || nmWaterPatches != 3) return 4;
    if (init_saw_patch != backing || init_saw_count != 3) return 5;
    worldAllocateWaterPatches__Fi(0);
    if (freed_calls != 1 || freed != backing) return 6;
    if (waterPatch != nullptr || nmWaterPatches != 0) return 7;
    if (alloc_calls != 1 || init_calls != 1) return 8;
    worldAllocateWaterPatches__Fi(2);
    worldAllocateWaterPatches__Fi(5);
    if (freed_calls != 2 || alloc_calls != 3 || init_calls != 3) return 9;
    if (alloc_size != 5 * 4784 || nmWaterPatches != 5) return 10;
    return 0;
}
