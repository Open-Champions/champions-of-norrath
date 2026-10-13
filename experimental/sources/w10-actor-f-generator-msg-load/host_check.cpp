// Host contract for msg_load__9GeneratorPc. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#include "msg_load.cpp"
int main() {
    const unsigned firsts[] = {0u, 1u, 0x12345678u, 0xffff8001u, 0xdeadbeefu};
    for (unsigned first : firsts) {
        unsigned char object[1100];
        unsigned char data[16];
        for (unsigned i = 0; i < sizeof object; ++i) object[i] = 0x41;
        for (unsigned i = 0; i < sizeof data; ++i) data[i] = 0x77;
        vslots_b::write_word(vslots_b::address(data), first);
        vslots_b::write_word(vslots_b::address(data) + 4u, first * 3u + 0x01020304u);
        if (msg_load__9GeneratorPc(object, reinterpret_cast<char*>(data)) != 8) return 1;
        if (vslots_b::read_u16(vslots_b::address(object) + 572u) != (first & 0xffffu)) return 2;
        if (vslots_b::read_word(vslots_b::address(object) + 976u) != first * 3u + 0x01020304u) return 3;
        for (unsigned i = 0; i < sizeof object; ++i) {
            const bool known = (i >= 572 && i < 574) || (i >= 976 && i < 980);
            if (!known && object[i] != 0x41) return 4;
        }
        for (unsigned i = 8; i < sizeof data; ++i) if (data[i] != 0x77) return 5;
    }
    return 0;
}
