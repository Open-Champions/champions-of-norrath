// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "init_type.cpp"
#include <cstdio>
#include <cstring>
static int commanders = 0;
static void* commander_object = nullptr;
extern "C" void addCommander__14WoodElfSoldier(void* object) { ++commanders; commander_object = object; }
int main() {
    int bad = 0;
    const unsigned variants[] = {0, 1, 2, 3, 4, 5, 100, 0xffffffffu};
    for (unsigned v : variants) {
        unsigned char object[1000];
        std::memset(object, 0x5a, sizeof object);
        rtti_c::write_word(rtti_c::address(object) + 964u, v);
        commanders = 0; commander_object = nullptr;
        norrath_wood_elf_soldier_init_type(object);
        unsigned want = 512u;
        if (v == 0) want = 128u; else if (v == 1) want = 256u; else if (v == 3) want = 16384u;
        if (rtti_c::read_word(rtti_c::address(object) + 244u) != want) { std::printf("variant %u\n", v); ++bad; }
        if (commanders != (v == 1 ? 1 : 0) || (v == 1 && commander_object != object)) { std::printf("commander %u\n", v); ++bad; }
        if (object[243] != 0x5a || object[248] != 0x5a) { std::printf("neighbours\n"); ++bad; }
    }
    std::printf(bad ? "failed\n" : "initType WoodElfSoldier passed\n");
    return bad != 0;
}
