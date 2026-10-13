// Host check only. Not an EE build input.
#include <cstdio>
#include <cstring>

#include "msg_load.cpp"

int main() {
    alignas(8) unsigned char object[160];
    char stream[16];
    std::memset(object, 0x5a, sizeof object);
    std::memset(stream, 0x11, sizeof stream);
    unsigned char before[160];
    std::memcpy(before, object, sizeof object);
    int failures = 0;
    if (msg_load__10AutoObjectPc(object, stream) != 0) { std::fprintf(stderr, "fail: nonzero\n"); ++failures; }
    if (std::memcmp(before, object, sizeof object) != 0) { std::fprintf(stderr, "fail: object written\n"); ++failures; }
    if (msg_load__10AutoObjectPc(nullptr, nullptr) != 0) { std::fprintf(stderr, "fail: null args\n"); ++failures; }
    return failures ? 1 : 0;
}
