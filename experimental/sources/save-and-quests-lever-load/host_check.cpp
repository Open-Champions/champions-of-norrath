#define NORRATH_SQ_HOST 1
#include <cstdio>
#include <cstring>
static void* added;
static int add_calls;
extern "C" void objectAddToRunList__FP10GameObject(void* o) { added = o; add_calls += 1; }
#include "lever_load.cpp"
int main() {
    unsigned char object[84];
    unsigned char buffer[12];
    std::memset(object, 0x44, sizeof object);
    std::memset(buffer, 0x99, sizeof buffer);
    sq::write_word(buffer, 0x7fc00001u);  // a NaN pattern must survive
    sq::write_word(buffer + 4, 0x11223344u);
    buffer[8] = 0xab;
    buffer[9] = 0x00;
    if (msg_load__5LeverPc(object, (char*)buffer) != 10) return 1;
    if (sq::read_word(object + 76) != 0x7fc00001u || sq::read_word(object + 12) != 0x11223344u) return 2;
    if (object[80] != 0xab || object[81] != 0 || object[82] != 0x44 || object[11] != 0x44) return 3;
    if (add_calls != 0) return 4;
    buffer[9] = 0x80;  // negative byte is nonzero
    if (msg_load__5LeverPc(object, (char*)buffer) != 10) return 5;
    if (object[81] != 0x80 || add_calls != 1 || added != object) return 6;
    buffer[9] = 1;
    msg_load__5LeverPc(object, (char*)buffer);
    if (add_calls != 2) return 7;
    std::puts("ok");
    return 0;
}
