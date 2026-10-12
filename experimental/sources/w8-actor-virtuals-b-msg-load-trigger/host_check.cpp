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
    alignas(8) unsigned char object[128];
    alignas(8) char stream[16];
    std::memset(object, 0x77, sizeof object);
    std::memset(stream, 0x3c, sizeof stream);
    stream[0] = 0x44; stream[1] = 0x33; stream[2] = 0x22; stream[3] = 0x11;
    stream[4] = static_cast<char>(0xfe);
    int used = msg_load__7TriggerPc(object, stream);
    expect(used == 5, "consumes five bytes");
    expect(object[100] == 0x44 && object[101] == 0x33 && object[102] == 0x22 && object[103] == 0x11, "word at +100");
    expect(object[96] == 0xfe, "byte at +96");
    bool rest = true;
    for (unsigned i = 0; i < sizeof object; ++i) {
        if ((i >= 96 && i < 97) || (i >= 100 && i < 104)) continue;
        if (object[i] != 0x77) rest = false;
    }
    expect(rest, "nothing else is written");
    expect(stream[5] == 0x3c, "stream is only read");
    return failures ? 1 : 0;
}
