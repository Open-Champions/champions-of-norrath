#include <cstdio>
#include <cstring>

#include "msg_load.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

int main() {
    alignas(8) unsigned char object[96];
    alignas(8) char stream[8];
    std::memset(object, 0x77, sizeof object);
    std::memset(stream, 0x3c, sizeof stream);
    stream[0] = 0x34; stream[1] = static_cast<char>(0x92);
    int used = msg_load__7ShooterPc(object, stream);
    expect(used == 2, "consumes two bytes");
    expect(object[82] == 0x34 && object[83] == 0x92, "half at +82");
    bool rest = true;
    for (unsigned i = 0; i < sizeof object; ++i) {
        if (i == 82 || i == 83) continue;
        if (object[i] != 0x77) rest = false;
    }
    expect(rest, "nothing else is written");
    return failures ? 1 : 0;
}
