#include <cstdio>
extern "C" unsigned descListMark[32];
unsigned descListMark[32];
#include "check_can_alloc_descriptor.cpp"
int main() {
    int failures = 0;
    for (int i = 0; i < 32; ++i) descListMark[i] = 7u + i;
    if (checkCanAllocDescriptor__Fv() != 0) { std::fprintf(stderr, "fail: all used\n"); ++failures; }
    for (int free_slot = 0; free_slot < 32; ++free_slot) {
        for (int i = 0; i < 32; ++i) descListMark[i] = 7u + i;
        descListMark[free_slot] = 0xffffffffu;
        if (checkCanAllocDescriptor__Fv() != 1) { std::fprintf(stderr, "fail: slot %d free\n", free_slot); ++failures; }
    }
    for (int i = 0; i < 32; ++i) descListMark[i] = 0u;
    if (checkCanAllocDescriptor__Fv() != 0) { std::fprintf(stderr, "fail: zero is not free\n"); ++failures; }
    return failures;
}
