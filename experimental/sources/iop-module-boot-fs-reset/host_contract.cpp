#include "fs_reset.cpp"

#include <cstdio>

static void* seen_destination;
static int seen_value;
static unsigned seen_count;

extern "C" void* memset(void* destination, int value, unsigned count) {
    seen_destination = destination;
    seen_value = value;
    seen_count = count;
    auto* bytes = static_cast<unsigned char*>(destination);
    for (unsigned index = 0; index < count; ++index)
        bytes[index] = static_cast<unsigned char>(value);
    return destination;
}

int main() {
    _fs_init = 7;
    _fsversion[0] = 0xff;
    _fsversion[3] = 0xee;
    if (sceFsReset() != 0) return 1;
    if (_fs_init != 0 || seen_destination != _fsversion || seen_value != 0 || seen_count != 4) return 2;
    if (_fsversion[0] != 0 || _fsversion[3] != 0) return 3;
    std::puts("sceFsReset passed");
    return 0;
}
