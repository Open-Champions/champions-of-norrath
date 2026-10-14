#include "srecv_view.cpp"

#include <cstdio>

int main() {
    if (sizeof norrath_overlay_sRecv_11edd00 != 512) return 1;
    if (reinterpret_cast<__UINTPTR_TYPE__>(norrath_overlay_sRecv_11edd00) % 64 != 0) return 2;
    for (int word : norrath_overlay_sRecv_11edd00) if (word != 0) return 3;
    std::puts("overlay sRecv view contract passed");
    return 0;
}
