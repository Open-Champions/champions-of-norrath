// Host check only; not an EE input. The image holds 64 zero bytes at the free-list table; the authored object must be
// 16 pointer-sized heads that start empty. Build together with free_list.cpp.
#include <cstdio>
extern "C" void* w15_small_block_free_list[16] __asm__("_t24__default_alloc_template2b0i0$_S_free_list");
int main() {
    for (int i = 0; i < 16; ++i)
        if (w15_small_block_free_list[i] != nullptr) { std::puts("free list not empty"); return 1; }
    std::puts("free_list ok");
    return 0;
}
