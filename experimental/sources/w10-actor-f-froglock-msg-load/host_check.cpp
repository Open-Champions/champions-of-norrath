// Host contract for msg_load__8FroglockPc. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#include "msg_load.cpp"
#include <initializer_list>
static int base_calls = 0, init_calls = 0, result = 0, order = 0, base_at = -1, init_at = -1;
static void *b_self, *i_self;
static unsigned seen960 = 0, seen208 = 0;
extern "C" int msg_load__8CreaturePc(void* self, char* data) {
    ++base_calls; base_at = order++; b_self = self; (void)data; return result;
}
extern "C" void initAttackVariant__8Froglock(void* self) {
    ++init_calls; init_at = order++; i_self = self;
    seen960 = vslots_b::read_word(vslots_b::address(self) + 960u);
    seen208 = vslots_b::read_word(vslots_b::address(self) + 208u);
}
int main() {
    using namespace vslots_b;
    for (int count : {0, 4, 20, 64}) for (unsigned half : {0u, 9u, 10u, 11u, 0xfffau, 0x800au}) {
        unsigned char object[1200];
        unsigned char data[160];
        for (unsigned i = 0; i < sizeof object; ++i) object[i] = 0x41;
        for (unsigned i = 0; i < sizeof data; ++i) data[i] = static_cast<unsigned char>(0x77 + i);
        write_u16(address(object) + 572u, half);
        base_calls = init_calls = order = 0; result = count;
        if (msg_load__8FroglockPc(object, reinterpret_cast<char*>(data)) != count + 8) return 1;
        if (base_calls != 1 || init_calls != 1 || base_at != 0 || init_at != 1 || b_self != object || i_self != object) return 2;
        const unsigned w0 = read_word(address(data) + unsigned(count));
        const unsigned w1 = read_word(address(data) + unsigned(count) + 4u);
        if (seen960 != w0 || seen208 != w1) return 3;
        if (read_word(address(object) + 208u) != w1) return 4;
        const unsigned want960 = (half == 10u) ? 0u : w0;
        if (read_word(address(object) + 960u) != want960) return 5;
        for (unsigned i = 0; i < sizeof object; ++i) {
            const bool known = (i >= 960 && i < 964) || (i >= 208 && i < 212) || (i >= 572 && i < 574);
            if (!known && object[i] != 0x41) return 6;
        }
    }
    return 0;
}
