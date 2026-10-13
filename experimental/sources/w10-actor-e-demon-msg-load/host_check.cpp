// Host contract for msg_load__5DemonPc. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#include "msg_load.cpp"
#include <cstdio>
#include <initializer_list>
static int calls = 0, init_calls = 0, result = 0; static void *s_self, *s_init; static unsigned word_at_init = 0;
static int init_after_load = 0;
extern "C" int msg_load__8CreaturePc(void* self, char* data) { ++calls; s_self = self; (void)data; return result; }
extern "C" void initAttackVariant__5Demon(void* self) {
    s_init = self; ++init_calls; init_after_load = calls; word_at_init = vslots_b::read_word(vslots_b::address(self) + 208u);
}
int main() {
    for (int count : {0, 4, 17, 64}) {
        unsigned char object[256]; unsigned char data[160];
        for (unsigned i = 0; i < sizeof object; ++i) object[i] = 0x41;
        for (unsigned i = 0; i < sizeof data; ++i) data[i] = 0x77;
        vslots_b::write_word(vslots_b::address(data) + unsigned(count), 0x01020304u);
        calls = init_calls = 0; result = count;
        const int got = msg_load__5DemonPc(object, reinterpret_cast<char*>(data));
        if (got != count + 4 || calls != 1 || init_calls != 1 || s_self != object || s_init != object) return 1;
        if (word_at_init != 0x01020304u || init_after_load != 1) return 2;
        for (unsigned i = 0; i < sizeof object; ++i)
            if ((i < 208 || i >= 212) && object[i] != 0x41) return 4;
    }
    return 0;
}
