#include <cstdint>
#include <cstdio>
#include "packet_pool.cpp"
int main() {
    if (sizeof norrath_sif_m_rpc_packet_pool_view != 64) return 1;
    if (reinterpret_cast<std::uintptr_t>(norrath_sif_m_rpc_packet_pool_view) % 64 != 0) return 2;
    for (unsigned char b : norrath_sif_m_rpc_packet_pool_view) if (b) return 3;
    std::puts("sif m packet pool check passed");
    return 0;
}
