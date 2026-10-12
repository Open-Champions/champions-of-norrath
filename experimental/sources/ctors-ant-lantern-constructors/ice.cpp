#include "detail.hpp"
// __3IceG6Point3sPcN43 at 0x1e61f0, 316 bytes. The five trailing arguments are
// all char pointers (mangled as one pointer plus four repeats).
// Observed: GameObject receives the point snapshot and two nulls. Byte 40 is
// 120 and _vt$3Ice (0x44cc28) goes to object+72. scriptGetGameState is called
// with the text at 0x46d070. A nonzero result (the whole 64-bit value) makes the
// constructor delete the object through virtual slot 3 of that table: the table
// entry at vtable+8 holds a short this-adjustment and the function pointer at
// vtable+12 is called with object+adjustment and the integer 3; V0 is its V0.
// A zero result stores the type half at 56, ORs 9 into object+12, stores the
// five arguments at 76, 80, 84, 88, 92, looks up lumpFindResource(arg0, arg1)
// into object+96 and lumpFindResource(object[76], object[84]) into object+100,
// writes the single 200.0 to object+28 and +32 and returns the run-list V0.
extern "C" unsigned norrath_construct_Ice(void* object, const void* point, short type, char* first_lump,
                                         char* first_name, char* second_name, char* third_name,
                                         char* fourth_name) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    (void)norrath_game_object_construct(object, &local, nullptr, nullptr);
    const Address a = address(object);
    bytes(a)[40] = 120u;
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44cc28));
    if (norrath_script_get_game_state(reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_46d070)) != 0) {
        if (object == nullptr) return 0u;
        const unsigned char* table = static_cast<const unsigned char*>(pointer(read_word(a + 72u))) + 8;
        const int adjust = static_cast<short>(table[0] | (table[1] << 8));
        const unsigned function = read_word(address(table) + 4u);
        typedef unsigned (*Destroy)(void*, int);
        return reinterpret_cast<Destroy>(pointer(function))(pointer(a + static_cast<Address>(adjust)), 3);
    }
    write_half(a + 56u, static_cast<unsigned>(type));
    write_word(a + 12u, read_word(a + 12u) | 9u);
    write_word(a + 84u, address_bits(second_name));
    write_word(a + 88u, address_bits(third_name));
    write_word(a + 92u, address_bits(fourth_name));
    write_word(a + 76u, address_bits(first_lump));
    write_word(a + 80u, address_bits(first_name));
    write_word(a + 96u, address_bits(norrath_lump_find_resource(first_lump, first_name)));
    write_word(a + 100u, address_bits(norrath_lump_find_resource(
        reinterpret_cast<char*>(pointer(read_word(a + 76u))), reinterpret_cast<char*>(pointer(read_word(a + 84u))))));
    write_word(a + 32u, 0x43480000u);
    write_word(a + 28u, 0x43480000u);
    return norrath_actor_add_run_list(object);
}
