// Build the source separately with -Dmemset=norrath_test_memset so the test
// and sanitizers continue to use the host's own standard runtime.
#include <cstddef>
#include <cstdio>
#include <climits>
extern "C" void* norrath_test_memset(void*, int, std::size_t);
int main() {
    alignas(16) unsigned char actual[640];
    const int values[] = {0, 1, 127, 128, 255, 256, 257, -1, -128, -256, INT_MIN, INT_MAX};
    unsigned cases = 0;
    for (unsigned alignment = 0; alignment < 32; ++alignment) {
        for (unsigned count = 0; count <= 512; ++count) {
            for (int value : values) {
                for (auto& byte : actual) byte = 0xa5;
                auto* destination = actual + 32 + alignment;
                if (norrath_test_memset(destination, value, count) != destination) return 1;
                for (unsigned i = 0; i < sizeof actual; ++i) {
                    unsigned char expected = i >= 32 + alignment && i < 32 + alignment + count
                        ? static_cast<unsigned char>(value) : 0xa5;
                    if (actual[i] != expected) return 2;
                }
                ++cases;
            }
        }
    }
    std::printf("%u guarded byte-fill cases passed\n", cases);
}
