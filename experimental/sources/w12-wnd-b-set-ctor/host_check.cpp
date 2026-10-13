// Host check only; not a build input. Exactly the five words at +0..+16 are written.
#include "ctor.cpp"
#include <cstdio>
#include <cstring>

int main() {
    unsigned char object[48];
    std::memset(object, 0x5c, sizeof object);
    __t5VISet1ZP5VIWnd(object);
    int words[5];
    std::memcpy(words, object, sizeof words);
    bool ok = words[0] == 0 && words[1] == 0 && words[2] == 0 && words[3] == 0 && words[4] == -1;
    for (unsigned i = 20; i < sizeof object; ++i) ok = ok && object[i] == 0x5c;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
