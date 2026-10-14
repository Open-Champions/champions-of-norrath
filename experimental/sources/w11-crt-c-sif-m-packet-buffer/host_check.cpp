// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_check.cpp   (the source is included)
#include "packet_buffer.cpp"
static_assert(sizeof(norrath_sif_m_rpc_packet_buffer_view) == 0x8e8c00 - 0x8e8400, "span between the two recorded addresses");
static_assert(sizeof(norrath_sif_m_rpc_packet_buffer_view) == 32 * 64, "32 packets of 64 bytes");
static_assert(__alignof__(norrath_sif_m_rpc_packet_buffer_view) >= 64, "packet alignment");
int main() {
    for (unsigned char byte : norrath_sif_m_rpc_packet_buffer_view) if (byte != 0) return 1;
    return 0;
}
