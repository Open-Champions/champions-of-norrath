// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp lgcodec_encode.cpp
#include <cstring>
static int tail_calls; static const void *t_codec, *t_src; static int t_ssize; static void* t_dst; static int* t_size;
extern "C" {
unsigned lgCodecGSMEncode(const void* codec, const void* source, int source_size, void* destination, int* size) {
    ++tail_calls; t_codec = codec; t_src = source; t_ssize = source_size; t_dst = destination; t_size = size;
    return 0x1234;
}
unsigned lgCodecEncode(const int* codec, const void* source, int source_size, void* destination, int* size);
}
int main() {
    int codec[2] = {10, 0}; unsigned char src[64], dst[64]; int size = 40;
    for (int i = 0; i < 64; ++i) src[i] = static_cast<unsigned char>(i + 1);
    std::memset(dst, 0xee, sizeof dst);
    // null codec
    if (lgCodecEncode(nullptr, src, 8, dst, &size) != 0x80000004u || dst[0] != 0xee || size != 40) return 1;
    // kind 10: copy the smaller of the source size and the room, report it
    if (lgCodecEncode(codec, src, 16, dst, &size) != 0 || size != 16 || std::memcmp(dst, src, 16) != 0 || dst[16] != 0xee) return 2;
    size = 8; std::memset(dst, 0xee, sizeof dst);
    if (lgCodecEncode(codec, src, 16, dst, &size) != 0 || size != 8 || std::memcmp(dst, src, 8) != 0 || dst[8] != 0xee) return 3;
    size = 8; std::memset(dst, 0xee, sizeof dst);
    if (lgCodecEncode(codec, src, 8, dst, &size) != 0 || size != 8 || dst[7] != 8 || dst[8] != 0xee) return 4;
    size = 30; if (lgCodecEncode(codec, src, 0, dst, &size) != 0 || size != 0) return 5;
    // kind 10 with a missing source or destination
    size = 40; std::memset(dst, 0xee, sizeof dst);
    if (lgCodecEncode(codec, nullptr, 8, dst, &size) != 0x80000004u || size != 40 || dst[0] != 0xee) return 6;
    if (lgCodecEncode(codec, src, 8, nullptr, &size) != 0x80000004u || size != 40) return 7;
    // kind 300 is forwarded unchanged, result included
    codec[0] = 300; size = 40;
    if (lgCodecEncode(codec, src, 24, dst, &size) != 0x1234 || tail_calls != 1) return 8;
    if (t_codec != codec || t_src != src || t_ssize != 24 || t_dst != dst || t_size != &size || size != 40) return 9;
    // forwarded even with null buffers (the checks belong to the other path)
    if (lgCodecEncode(codec, nullptr, 24, nullptr, &size) != 0x1234 || tail_calls != 2) return 10;
    // other kinds
    codec[0] = 11; if (lgCodecEncode(codec, src, 8, dst, &size) != 0x80000000u) return 11;
    codec[0] = 0;  if (lgCodecEncode(codec, src, 8, dst, &size) != 0x80000000u) return 12;
    codec[0] = 299; if (lgCodecEncode(codec, src, 8, dst, &size) != 0x80000000u || tail_calls != 2) return 13;
    return 0;
}
