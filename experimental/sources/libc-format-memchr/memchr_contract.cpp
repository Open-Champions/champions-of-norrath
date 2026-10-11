#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" const unsigned char* norrath_factory_memchr(const unsigned char* input, int byte, unsigned count);

int main() {
    unsigned cases = 0;
    alignas(64) unsigned char buffer[160];
    const int needles[] = {0, 1, 32, 65, 97, 127, 128, 255, 256, 257, -1, -97, -128};
    for (unsigned alignment = 0; alignment < 32; ++alignment) {
        for (unsigned count = 0; count <= 64; ++count) {
            for (int needle : needles) {
                for (int pattern = 0; pattern < 4; ++pattern) {
                    std::memset(buffer, 0x5a, sizeof buffer);
                    unsigned char* start = buffer + 32 + alignment;
                    if (count != 0) {
                        if (pattern == 1) start[0] = static_cast<unsigned char>(needle);
                        if (pattern == 2) start[count - 1] = static_cast<unsigned char>(needle);
                        if (pattern == 3 && count > 2) start[count / 2] = static_cast<unsigned char>(needle);
                    }
                    const void* expected = std::memchr(start, needle, count);
                    const unsigned char* actual = norrath_factory_memchr(start, needle, count);
                    if (actual != expected) {
                        std::printf("mismatch align %u count %u needle %d pattern %d\n", alignment, count, needle, pattern);
                        return 1;
                    }
                    ++cases;
                }
            }
        }
    }
    if (norrath_factory_memchr(nullptr, 0, 0) != nullptr) return 2;
    ++cases;
    std::printf("%u memchr cases passed\n", cases);
    return 0;
}
