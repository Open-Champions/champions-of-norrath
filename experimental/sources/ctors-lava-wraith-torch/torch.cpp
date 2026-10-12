#include "../ctors-lava-wraith-detail/detail.hpp"
// __5TorchG6Point3PPci at 0x22f590, 368 bytes.
// GameObject receives a point snapshot, a null resource map, and the argument
// list. View 0x44ab70 goes to +72. A light record is filled in place at +76:
// 240.0f, the float word at 0x481970 and 1.0f in two position/direction
// triples (+104..+119), halves 200/150/50 at +86 and +94, zero halves at +92
// and +100, and zero bytes at +84 and +85 before lightAdd receives +76. The
// point snapshot is also copied to +124. The int is stored as a byte at +138;
// its low byte selects flag 0x1 (zero) or 0x200 (nonzero). Byte +85 is set to
// one, +40 to 0xf0, halves +94/+96/+98 are overwritten with 51/39/18, bytes
// +136/+137 are cleared, and flags gain 0x00010008. The machine V0 left at
// return is the constant 39 from the last half store, not a callee result.
extern "C" unsigned norrath_construct_Torch(void* object, const void* point, char** arguments, int kind) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_game_object_construct(object, &local, nullptr, arguments);
    const Address a = address(object);
    const unsigned third = read_word(address(norrath_ctors_lava_wraith_view_481970));
    const unsigned bright = float_bits(240.0f);
    const unsigned one = float_bits(1.0f);
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44ab70));
    write_byte(a + 85u, 0u);
    write_point(a + 124u, local);
    write_word(a + 112u, bright);
    write_word(a + 116u, third);
    write_word(a + 120u, one);
    write_half(a + 94u, 200u);
    write_half(a + 96u, 150u);
    write_half(a + 98u, 50u);
    write_word(a + 104u, bright);
    write_word(a + 108u, third);
    write_half(a + 86u, 200u);
    write_half(a + 88u, 150u);
    write_half(a + 90u, 50u);
    write_half(a + 92u, 0u);
    write_half(a + 100u, 0u);
    write_byte(a + 84u, 0u);
    norrath_light_add(pointer(a + 76u));
    write_byte(a + 138u, static_cast<unsigned>(kind));
    write_byte(a + 85u, 1u);
    if ((static_cast<unsigned>(kind) << 24) == 0u) {
        write_word(a + 12u, read_word(a + 12u) | 0x1u);
    } else {
        write_word(a + 12u, read_word(a + 12u) | 0x200u);
    }
    const unsigned flags = read_word(a + 12u);
    write_byte(a + 40u, 0xf0u);
    write_half(a + 94u, 51u);
    write_half(a + 96u, 39u);
    write_half(a + 98u, 18u);
    write_half(a + 100u, 0u);
    write_byte(a + 136u, 0u);
    write_byte(a + 137u, 0u);
    write_word(a + 12u, flags | 0x10008u);
    return 39u;
}
