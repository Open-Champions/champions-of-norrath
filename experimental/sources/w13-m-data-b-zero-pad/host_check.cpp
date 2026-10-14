// Host check: compiled together with zero_pad.cpp so it tests the authored definition itself.
#include "zero_pad.cpp"

#include <cstdint>

static_assert(sizeof zeroPad == 88, "zeroPad must be an 88-byte pad record");

int main() {
    for (unsigned char byte : zeroPad) {
        if (byte != 0) return 1;
    }
    return reinterpret_cast<std::uintptr_t>(zeroPad) % 8 == 0 ? 0 : 1;
}
