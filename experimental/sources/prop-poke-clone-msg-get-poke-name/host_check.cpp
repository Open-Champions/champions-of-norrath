#include "msg_get_poke_name.cpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
int main() {
    unsigned char object[32];
    unsigned short buffer[8];
    std::memset(object, 0x3c, sizeof object);
    std::memset(buffer, 0x5a, sizeof buffer);
    expect(msg_getPokeName__10GameObjectPUsi(object, buffer, 7) == 0, "returns zero");
    for (unsigned char b : object) expect(b == 0x3c, "object unchanged");
    for (unsigned short b : buffer) expect(b == 0x5a5a, "buffer unchanged");
    return failures ? 1 : 0;
}
