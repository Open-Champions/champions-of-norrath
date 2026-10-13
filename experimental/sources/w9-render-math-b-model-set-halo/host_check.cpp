// Host contract for modelSetHalo__FP7HaloDefi. Not an EE plan input.
#include <cstdio>
extern "C" {
const void* halos;
int nmHalos;
void modelSetHalo__FP7HaloDefi(const void*, int);
}
int main() {
    int failures = 0;
    int table[4];
    modelSetHalo__FP7HaloDefi(table, 3);
    if (halos != table || nmHalos != 3) { std::fprintf(stderr, "fail: first call\n"); ++failures; }
    modelSetHalo__FP7HaloDefi(nullptr, -1);
    if (halos != nullptr || nmHalos != -1) { std::fprintf(stderr, "fail: second call\n"); ++failures; }
    if (failures) return 1;
    std::printf("modelSetHalo contract passed\n");
    return 0;
}
