#include "../ctors-lava-wraith-detail/detail.hpp"
// __5RondoG6Point3sPPc at 0x232818, 388 bytes.
// Creature receives a point snapshot, the sign-extended type, the resource map
// at 0x42d048, scale 10.0f and the argument list. Flags gain 0x01000000 and
// the high word of the doubleword at +160 gains 0x00040004. View 0x44f3d8 goes
// to +72, the literal-pool float 0x3f999999 to +168, 20.0f to +28 and the word
// at 0x4844a4 to +248. +792 gets -1 as a half, +572 a zero half, and +176,
// +192, +236, +256, +960 and +964 are zeroed. setParameters receives five
// 1.5f values and 1. The animation state at +288 is initialised, then the
// header word at (model+44), where model is the word at +36, is added with
// flag 1. +244 and +236 become 1, and the 64-bit result of modelGetActiveChunks
// (the word at model+12, 1) is stored at +552. The run list is called and
// setAnimEventFlags receives 3, the literal-pool float 0x3e999999, 1 and
// zeros. Low V0 is finishConstruction's machine result, which is not known to
// carry meaning.
extern "C" unsigned norrath_construct_Rondo(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_lava_wraith_view_42d048, 10.0f, arguments);
    const Address a = address(object);
    write_word(a + 12u, read_word(a + 12u) | 0x01000000u);
    const unsigned high = read_word(a + 164u) | 0x00040004u;
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44f3d8));
    write_word(a + 168u, 0x3f999999u);
    write_word(a + 28u, float_bits(20.0f));
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 164u, high);
    write_half(a + 792u, 0xffffu);
    write_word(a + 960u, 0u);
    write_word(a + 176u, 0u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 256u, 0u);
    write_word(a + 964u, 0u);
    norrath_creature_set_parameters(object, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1);
    norrath_actor_animation_initialize(bytes(a + 288u));
    norrath_actor_animation_add(bytes(a + 288u), load_pointer(address(load_pointer(a + 36u)) + 44u), 1);
    write_word(a + 244u, 1u);
    const unsigned long long chunks = norrath_actor_model_active_chunks(load_pointer(address(load_pointer(a + 36u)) + 12u), 1);
    write_word(a + 236u, 1u);
    write_word(a + 552u, static_cast<unsigned>(chunks));
    write_word(a + 556u, static_cast<unsigned>(chunks >> 32));
    (void)norrath_actor_add_run_list(object);
    norrath_creature_set_anim_event_flags(object, 3, bits_float(0x3e999999u), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    return norrath_creature_finish_construction(object);
}
