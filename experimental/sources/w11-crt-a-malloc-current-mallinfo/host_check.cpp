// Host contract for __malloc_current_mallinfo. Not an EE plan input.
#include <cstdio>
#include "malloc_current_mallinfo.cpp"
int main() {
    if (sizeof(MallinfoBlock) != 40 || sizeof(__malloc_current_mallinfo) != 40) return 1;
    for (int i = 0; i < 10; ++i) if (__malloc_current_mallinfo.word[i] != 0) return 2;
    __malloc_current_mallinfo.word[9] = 5;                   // the block is writable
    if (__malloc_current_mallinfo.word[9] != 5) return 3;
    std::puts("mallinfo block ok");
    return 0;
}
