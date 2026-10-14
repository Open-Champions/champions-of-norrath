#include "edge_lists.cpp"

#include <cstdio>

int main() {
    if (sizeof mt_ebx != 1024 || sizeof mt_eby != 1024) return 1;
    for (int i = 0; i < 256; ++i) if (mt_ebx[i] != 0.0f || mt_eby[i] != 0.0f) return 2;
    mt_ebx[255] = -2.0f;
    if (mt_eby[255] != 0.0f || mt_ebx[0] != 0.0f) return 3;
    if (static_cast<void*>(mt_ebx) == static_cast<void*>(mt_eby)) return 4;
    std::puts("megatile edge lists contract passed");
    return 0;
}
