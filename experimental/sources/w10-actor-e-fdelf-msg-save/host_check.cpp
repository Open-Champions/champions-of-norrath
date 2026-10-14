// Host contract for msg_save__13FemaleDarkElfPc. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#include "msg_save.cpp"
#include <cstdio>
#include <initializer_list>
static int calls = 0, result = 0; static void *s_self; static char* s_data;
extern "C" int msg_save__8CreaturePc(void* self, char* data) { ++calls; s_self = self; s_data = data; return result; }
int main() {
    using namespace vslots_b;
    for (int count : {0, 4, 20, 64}) for (unsigned char b : {0, 1, 5, 127, 128, 255}) {
        unsigned char object[1200]; unsigned char data[160];
        for (unsigned i = 0; i < sizeof object; ++i) object[i] = 0x41;
        for (unsigned i = 0; i < sizeof data; ++i) data[i] = 0x77;
        object[960] = b; write_word(address(object) + 964u, 0x11223344u); write_word(address(object) + 1104u, 0x55667788u);
        write_word(address(object) + 208u, 0x99aabbccu);
        calls = 0; result = count;
        if (msg_save__13FemaleDarkElfPc(object, reinterpret_cast<char*>(data)) != count + 16 || calls != 1 || s_self != object) return 1;
        const unsigned want0 = b < 0x80 ? b : (unsigned)b | 0xffffff00u;
        if (read_word(address(data) + unsigned(count)) != want0 || read_word(address(data) + count + 4u) != 0x11223344u ||
            read_word(address(data) + count + 8u) != 0x55667788u || read_word(address(data) + count + 12u) != 0x99aabbccu) return 2;
        for (unsigned i = 0; i < sizeof data; ++i)
            if ((i < unsigned(count) || i >= unsigned(count) + 16) && data[i] != 0x77) return 3;
        for (unsigned i = 0; i < sizeof object; ++i) {
            const bool known = i == 960 || (i >= 964 && i < 968) || (i >= 1104 && i < 1108) || (i >= 208 && i < 212);
            if (!known && object[i] != 0x41) return 4;
        }
    }
    return 0;
}
