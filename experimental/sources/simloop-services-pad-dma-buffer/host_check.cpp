// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_check.cpp   (the source is included)
// The extent is the distance between two recorded addresses of the original (pad_dma_buf at
// 0x006e1240 and the next symbol, stateData, at 0x006e1c40). The alignment must be able to hold the
// original's 64-byte aligned address, and the object starts zeroed.
namespace {
constexpr unsigned long kStart = 0x006e1240ul;
constexpr unsigned long kNext = 0x006e1c40ul;
static_assert(kStart % 64 == 0, "the original object is 64-byte aligned");
static_assert(kNext - kStart == 2560, "recorded span");
}
#include "pad_dma_buffer.cpp"
static_assert(sizeof(pad_dma_buf) == kNext - kStart, "object size equals the recorded span");
static_assert(__alignof__(pad_dma_buf) >= 64, "alignment holds the original 64-byte aligned address");
int main() {
    for (unsigned char byte : pad_dma_buf) if (byte != 0) return 2;
    return 0;
}
