// Host contract for objectFindObject__FPc. Not an EE plan input.
#include <cstdio>

extern "C" void* objectFindObject__FPc(char* name);

static int failures = 0;
static char* seen_name = nullptr;
static int seen_capacity = 0;
static void** fill = nullptr;
static int fill_count = 0;

extern "C" void objectFindObject__FPcPP10GameObjecti(char* name, void** output, int capacity) {
    seen_name = name;
    seen_capacity = capacity;
    for (int i = 0; i < fill_count; ++i) output[i] = fill[i];
    if (fill_count < 2) output[fill_count] = nullptr;
}

int main() {
    char name[] = "door";
    void* two[2] = {reinterpret_cast<void*>(0x44), reinterpret_cast<void*>(0x55)};
    fill = two;
    fill_count = 2;
    void* result = objectFindObject__FPc(name);
    if (seen_name != name) { std::fprintf(stderr, "name not forwarded\n"); ++failures; }
    if (seen_capacity != 2) { std::fprintf(stderr, "capacity is not 2\n"); ++failures; }
    if (result != two[0]) { std::fprintf(stderr, "first match not returned\n"); ++failures; }
    fill_count = 0;
    if (objectFindObject__FPc(name) != nullptr) { std::fprintf(stderr, "empty result is not null\n"); ++failures; }
    if (failures != 0) return 1;
    std::printf("find-first contract passed\n");
    return 0;
}
