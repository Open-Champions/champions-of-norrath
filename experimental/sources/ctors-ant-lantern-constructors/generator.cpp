#include "detail.hpp"
// __9GeneratorG6Point3sPPc at 0x1f19c0, 400 bytes.
// Observed: Creature receives a point snapshot, the sign-extended short, a null
// map, the single 8.0 and the argument list. Byte 40 becomes 20, words 984 and
// 192 and 236 and half 572 are cleared, bits 0x9 are set in object+12 and bit
// 0x04000000 in the low word at object+160, the single 20.0 goes to object+28,
// the word at global 0x4844a4 goes to object+248 and _vt$9Generator (0x44bbd0)
// to object+72. setParameters then receives five 1.0 values and the integer 1.
// A tag string (address 0x46ded0) is stored at object+960; two tag integers
// (0x46dee0, 0x46def0) set bits 1 and 2 of object+980 when nonzero. The words
// 964 (600), 968 (copy of object+808), 972 (4), 976 (0) and 984 (0) are stored
// and the object joins the slow run list. Finally the stored string is compared
// with the text at 0x485230; an equal result sets bit 2 of object+980 again.
// V0 is the nonzero comparison, otherwise the updated flag word.
extern "C" unsigned norrath_construct_Generator(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, nullptr, 8.0f, arguments);
    const Address a = address(object);
    bytes(a)[40] = 20u;
    write_word(a + 984u, 0u);
    write_word(a + 12u, read_word(a + 12u) | 9u);
    write_word(a + 160u, read_word(a + 160u) | 0x04000000u);
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44bbd0));
    write_word(a + 28u, 0x41a00000u);
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    (void)norrath_actor_set_parameters(object, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1);
    char* name = norrath_actor_find_tag_string(object, reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_46ded0), 0);
    write_word(a + 960u, address_bits(name));
    if (norrath_game_object_find_tag_int(object, reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_46dee0), 0) != 0)
        write_word(a + 980u, read_word(a + 980u) | 1u);
    if (norrath_game_object_find_tag_int(object, reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_46def0), 0) != 0)
        write_word(a + 980u, read_word(a + 980u) | 2u);
    write_word(a + 964u, 600u);
    write_word(a + 968u, read_word(a + 808u));
    write_word(a + 972u, 4u);
    write_word(a + 984u, 0u);
    write_word(a + 976u, 0u);
    (void)norrath_actor_add_slow_run_list(object);
    const int different = norrath_factory_strcasecmp(reinterpret_cast<char*>(pointer(read_word(a + 960u))),
                                                    reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_485230));
    if (different != 0) return static_cast<unsigned>(different);
    const unsigned flags = read_word(a + 980u) | 2u;
    write_word(a + 980u, flags);
    return flags;
}
