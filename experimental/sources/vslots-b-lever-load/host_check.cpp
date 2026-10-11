#define NORRATH_VSLOTS_B_HOST 1
#include "lever_load.cpp"
#include <cstring>
static int calls = 0;
static void* seen = nullptr;
static unsigned during = 0;
extern "C" unsigned norrath_actor_add_run_list(void* object) {
    ++calls;
    seen = object;
    during = vslots_b::read_u8(vslots_b::address(object) + 81u);
    vslots_b::write_u8(vslots_b::address(object) + 81u, 0x22u);
    return 1u;
}
int main() {
    unsigned char object[96];
    unsigned char buffer[16];
    std::memset(object, 0x5a, sizeof object);
    unsigned char pattern[10] = {0x01, 0x00, 0xc0, 0x7f, 0x11, 0x22, 0x33, 0x44, 0x80, 0x00};
    std::memcpy(buffer, pattern, 10);
    buffer[10] = 0x99;
    if (msg_load__5LeverPc(object, buffer) != 10 || calls != 0) return 1;
    if (vslots_b::read_word(vslots_b::address(object) + 76u) != 0x7fc00001u) return 1;
    if (vslots_b::read_word(vslots_b::address(object) + 12u) != 0x44332211u) return 1;
    if (object[80] != 0x80 || object[81] != 0x00 || object[11] != 0x5a || object[16] != 0x5a) return 1;
    if (object[75] != 0x5a || object[82] != 0x5a || buffer[10] != 0x99) return 1;

    buffer[9] = 0x80;
    if (msg_load__5LeverPc(object, buffer) != 10 || calls != 1 || seen != object || during != 0x80u) return 2;
    if (object[81] != 0x22) return 2;
    return 0;
}
