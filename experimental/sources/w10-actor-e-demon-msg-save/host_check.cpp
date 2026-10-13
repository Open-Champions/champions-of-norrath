// Host contract for msg_save__5DemonPc. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#include "msg_save.cpp"
#include <cstdio>
#include <initializer_list>
static int calls = 0, result = 0; static void *s_self; static char* s_data;
extern "C" int msg_save__8CreaturePc(void* self, char* data) { ++calls; s_self = self; s_data = data; return result; }
int main() {
    for (int count : {0, 4, 17, 64}) {
        unsigned char object[256]; unsigned char data[160];
        for (unsigned i = 0; i < sizeof object; ++i) object[i] = 0x41;
        for (unsigned i = 0; i < sizeof data; ++i) data[i] = 0x77;
        vslots_b::write_word(vslots_b::address(object) + 208u, 0xa1b2c3d4u);
        calls = 0; result = count;
        const int got = msg_save__5DemonPc(object, reinterpret_cast<char*>(data));
        if (got != count + 4 || calls != 1 || s_self != object || s_data != reinterpret_cast<char*>(data)) return 1;
        if (vslots_b::read_word(vslots_b::address(data) + unsigned(count)) != 0xa1b2c3d4u) return 2;
        for (unsigned i = 0; i < sizeof data; ++i)
            if ((i < unsigned(count) || i >= unsigned(count) + 4) && data[i] != 0x77) return 3;
        for (unsigned i = 0; i < sizeof object; ++i)
            if ((i < 208 || i >= 212) && object[i] != 0x41) return 4;
    }
    return 0;
}
