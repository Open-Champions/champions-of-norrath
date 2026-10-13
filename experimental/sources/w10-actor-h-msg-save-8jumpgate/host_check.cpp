// Host contract for JumpGate msg_save; not an EE input.
#include "msg_save.cpp"
#include <cstdio>
#include <cstring>
int main() {
    unsigned char object[128]; char out[8];
    for (int i = 0; i < 128; ++i) object[i] = static_cast<unsigned char>(i + 1);
    object[77] = 0xc3; object[78] = 0x05;
    std::memset(out, 0x7e, sizeof out);
    int r = msg_save__8JumpGatePc(object, out);
    int bad = 0;
    if (r != 2) { std::puts("fail: returns 2"); bad = 1; }
    if (static_cast<unsigned char>(out[0]) != 0xc3 || out[1] != 0x05) { std::puts("fail: bytes from +77 then +78"); bad = 1; }
    if (out[2] != 0x7e) { std::puts("fail: wrote past two bytes"); bad = 1; }
    if (!bad) std::puts("jumpgate msg_save ok");
    return bad;
}
