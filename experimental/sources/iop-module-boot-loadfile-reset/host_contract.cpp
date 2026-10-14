#include "loadfile_reset.cpp"

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
    _bind_check = 4;
    _lfversion[0] = 0x32;
    _lfversion[3] = 0x30;
    if (sceSifLoadFileReset() != 0) return 1;
    if (_bind_check != -1 || seen_destination != _lfversion || seen_value != 0 || seen_count != 4) return 2;
    if (_lfversion[0] != 0 || _lfversion[3] != 0) return 3;
    std::puts("sceSifLoadFileReset passed");
    return 0;
}
