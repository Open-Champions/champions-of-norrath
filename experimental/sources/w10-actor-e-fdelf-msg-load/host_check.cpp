// Host contract for msg_load__13FemaleDarkElfPc. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#include "msg_load.cpp"
#include <cstdio>
#include <initializer_list>
static int calls = 0, init_calls = 0, result = 0, init_order = 0; static void *s_self, *s_init; static bool s_flag = false;
static unsigned seen[4];
extern "C" int msg_load__8CreaturePc(void* self, char* data) { ++calls; s_self = self; (void)data; return result; }
extern "C" void initType__13FemaleDarkElfb(void* self, bool flag) {
    using namespace vslots_b;
    s_init = self; s_flag = flag; ++init_calls; init_order = calls;
    seen[0] = read_u8(address(self) + 960u); seen[1] = read_word(address(self) + 964u);
    seen[2] = read_word(address(self) + 1104u); seen[3] = read_word(address(self) + 208u);
}
int main() {
    using namespace vslots_b;
    for (int count : {0, 4, 20, 64}) {
        unsigned char object[1200]; unsigned char data[160];
        for (unsigned i = 0; i < sizeof object; ++i) object[i] = 0x41;
        for (unsigned i = 0; i < sizeof data; ++i) data[i] = 0x77;
        write_word(address(data) + unsigned(count), 0xfedcba98u); write_word(address(data) + count + 4u, 0x11223344u);
        write_word(address(data) + count + 8u, 0x55667788u); write_word(address(data) + count + 12u, 0x99aabbccu);
        calls = init_calls = 0; result = count;
        if (msg_load__13FemaleDarkElfPc(object, reinterpret_cast<char*>(data)) != count + 16 || calls != 1 || init_calls != 1) return 1;
        if (s_self != object || s_init != object || !s_flag || init_order != 1) return 2;
        if (seen[0] != 0x98u || seen[1] != 0x11223344u || seen[2] != 0x55667788u || seen[3] != 0x99aabbccu) return 3;
        if (object[961] != 0x41 || object[962] != 0x41 || object[963] != 0x41) return 4;
        for (unsigned i = 0; i < sizeof object; ++i) {
            const bool known = i == 960 || (i >= 964 && i < 968) || (i >= 1104 && i < 1108) || (i >= 208 && i < 212);
            if (!known && object[i] != 0x41) return 5;
        }
    }
    return 0;
}
