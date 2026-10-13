// Host contract for VIBufferSwap__FiPUc. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
#include <cstdio>
#include "vi_buffer_swap.cpp"
int main() {
    int bad = 0;
    for (int size = -2; size <= 17; ++size) {
        unsigned char buffer[24];
        for (int i = 0; i < 24; ++i) buffer[i] = static_cast<unsigned char>(0x40 + i);
        VIBufferSwap__FiPUc(size, buffer + 3);
        for (int i = 0; i < 24; ++i) {
            int want = 0x40 + i;
            const int rel = i - 3;
            if (size >= 2 && rel >= 0 && rel < size) want = 0x40 + 3 + (size - 1 - rel);
            if (buffer[i] != want) { std::printf("size %d index %d: %02x want %02x\n", size, i, buffer[i], want); ++bad; }
        }
    }
    return bad;
}
