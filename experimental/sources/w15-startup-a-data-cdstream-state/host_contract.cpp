#include "cdstream_state.cpp"

#include <cstdio>

int main() {
    if (sizeof norrath_cdstream_client != 64 || reinterpret_cast<__UINTPTR_TYPE__>(norrath_cdstream_client) % 64 != 0) return 8;
    for (int i = 0; i < 16; ++i) if (norrath_cdstream_client[i] != 0) return 9;
    if (sizeof norrath_cdstream_send != 64 || sizeof norrath_cdstream_recv != 64) return 1;
    if (sizeof norrath_cdstream_pending != 4 || sizeof norrath_cdstream_status != 4) return 2;
    if (reinterpret_cast<__UINTPTR_TYPE__>(norrath_cdstream_send) % 64 != 0) return 3;
    if (reinterpret_cast<__UINTPTR_TYPE__>(norrath_cdstream_recv) % 64 != 0) return 4;
    for (int i = 0; i < 16; ++i) if (norrath_cdstream_send[i] != 0 || norrath_cdstream_recv[i] != 0) return 5;
    if (norrath_cdstream_pending != 0 || norrath_cdstream_status != 0 || last_buffer != nullptr) return 6;
    if (static_cast<void*>(norrath_cdstream_send) == static_cast<void*>(norrath_cdstream_recv)) return 7;
    std::puts("cdstream state contract passed");
    return 0;
}
