#include "../ctors-lava-wraith-detail/detail.hpp"
// __4SoulG6Point3sPPcP6Minion at 0x1f2f08, 520 bytes.
// Creature receives a point snapshot, the sign-extended type, the resource map
// at 0x42b3d8, scale 10.0f and the argument list. +960 is cleared, byte +40
// becomes 30, the doubleword at +160 gains 0x0000000204880000 (low word
// 0x04880000, high word 2), view 0x44def0 goes to +72, the word at 0x4844a4 to
// +248, the literal float 0x3f999999 to +168, 15.0f to +28, -1 as a half to
// +792, 1.0f to +88/+92/+96/+100/+104, and +964, +176, +192, +236, +256 and
// +572 (half) are cleared. setParameters receives 3.4f (0x40599999), the +168
// float, 1.0f twice more and 1. The animation state at +288 is initialised.
// With a minion pointer, flags gain 9, the minion is linked into the list head
// at +960 and the header word at model+108 is added. Without one, the text at
// levelName+81 is compared case-blind with the text at 0x485240: on a match the
// header word at model+52 is added and setState(24) runs, otherwise the header
// word at model+60 is added. +236 becomes 1, the run list is called and
// setAnimEventFlags receives 11, 0.7f (0x3f333333), 1 and zeros, then
// finishConstruction runs. Low V0 is finishConstruction's machine result.
extern "C" unsigned norrath_construct_Soul(void* object, const void* point, short type, char** arguments, void* minion) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_lava_wraith_view_42b3d8, 10.0f, arguments);
    const Address a = address(object);
    write_word(a + 960u, 0u);
    write_byte(a + 40u, 30u);
    const unsigned low_word = read_word(a + 160u) | 0x00080000u | 0x04800000u;
    const unsigned high_word = read_word(a + 164u) | 0x2u;
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 28u, float_bits(15.0f));
    write_half(a + 792u, 0xffffu);
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44def0));
    write_word(a + 160u, low_word);
    write_word(a + 164u, high_word);
    write_word(a + 964u, 0u);
    write_word(a + 176u, 0u);
    write_word(a + 168u, 0x3f999999u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 256u, 0u);
    write_word(a + 88u, float_bits(1.0f));
    write_word(a + 92u, float_bits(1.0f));
    write_word(a + 104u, float_bits(1.0f));
    write_word(a + 96u, float_bits(1.0f));
    write_word(a + 100u, float_bits(1.0f));
    norrath_creature_set_parameters(object, bits_float(0x40599999u), bits_float(0x3f999999u), 1.0f, 1.0f, 1.0f, 1);
    norrath_actor_animation_initialize(bytes(a + 288u));
    if (minion != nullptr) {
        write_word(a + 12u, read_word(a + 12u) | 0x9u);
        (void)norrath_list_oneway_link(bytes(a + 960u), minion, minion);
        norrath_actor_animation_add(bytes(a + 288u), load_pointer(address(load_pointer(a + 36u)) + 108u), 1);
    } else if (norrath_factory_strcasecmp(levelName + 81, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_485240)) == 0) {
        norrath_actor_animation_add(bytes(a + 288u), load_pointer(address(load_pointer(a + 36u)) + 52u), 1);
        norrath_soul_set_state(object, 24);
    } else {
        norrath_actor_animation_add(bytes(a + 288u), load_pointer(address(load_pointer(a + 36u)) + 60u), 1);
    }
    write_word(a + 236u, 1u);
    (void)norrath_actor_add_run_list(object);
    norrath_creature_set_anim_event_flags(object, 11, bits_float(0x3f333333u), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    return norrath_creature_finish_construction(object);
}
