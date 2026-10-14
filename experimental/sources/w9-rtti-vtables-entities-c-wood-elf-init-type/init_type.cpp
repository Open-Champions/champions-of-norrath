#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed initType__14WoodElfSoldier at 0x249c28, 96 bytes, emitted here as
// norrath_wood_elf_soldier_init_type (the name its constructor imports). Reads the word at
// object+964 and stores a word at object+244:
//   0: 128;
//   1: calls addCommander__14WoodElfSoldier(object) first, then stores 256;
//   3: 16384;
//   anything else (2 and above 3 included): 512.
// Unknown: what the numbers at +244 are, and what addCommander does with the
// object.
extern "C" void addCommander__14WoodElfSoldier(void* object);

extern "C" void norrath_wood_elf_soldier_init_type(void* self) {
    using namespace rtti_c;
    const Address object = address(self);
    unsigned value;
    switch (read_word(object + 964u)) {
    case 0u:
        value = 128u;
        break;
    case 1u:
        addCommander__14WoodElfSoldier(self);
        value = 256u;
        break;
    case 3u:
        value = 16384u;
        break;
    default:
        value = 512u;
        break;
    }
    write_word(object + 244u, value);
}
