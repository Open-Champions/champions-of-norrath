#include "firebomb.hpp"
namespace {
using namespace norrath_actors_a;
struct PointBytes { unsigned x, y, z; };
static_assert(sizeof(PointBytes) == 12 && alignof(PointBytes) == 4, "raw point storage");
PointBytes snapshot(const void* point) {
    const Address a = address(point);
    return PointBytes{read_word(a), read_word(a + 4u), read_word(a + 8u)};
}
unsigned tag_word(void* object, char* text, unsigned missing, bool scale) {
    if (norrath_actor_find_tag_string(object, text, 0) == nullptr) return missing;
    const unsigned value = static_cast<unsigned>(norrath_game_object_find_tag_int(object, text, 0));
    return scale ? value * 60u : value;
}
}
// __8FireBombG6Point3sPPc. The base call keeps the resource map at 0x00438e98.
// Absent tags store 240, byte 0, and byte 1. A present first tag stores 60 times
// its integer. The word at 0x00483e5c is the linear congruential state; V0 is
// the updated state shifted right 16, and offset 84 stores that value modulo
// the word at offset 80.
extern "C" unsigned norrath_construct_FireBomb(void* object, const void* point, short type,
                                               char** arguments) {
    PointBytes local = snapshot(point);
    (void)norrath_game_object_construct(object, &local, norrath_actors_a_view_438e98, arguments);
    const Address a = address(object);
    write_word(a + 72u, address(norrath_actors_a_view_449430));
    write_word(a + 28u, 0x42700000u);
    write_half(a + 56u, static_cast<unsigned>(type));
    write_word(a + 12u, 0x4001u);
    bytes(a)[77] = 0u;
    (void)norrath_actor_add_run_list(object);
    write_word(a + 80u, tag_word(object, reinterpret_cast<char*>(norrath_actors_a_view_477758), 240u, true));
    bytes(a)[78] = static_cast<unsigned char>(
        tag_word(object, reinterpret_cast<char*>(norrath_actors_a_view_4860b8), 0u, false));
    bytes(a)[79] = static_cast<unsigned char>(
        tag_word(object, reinterpret_cast<char*>(norrath_actors_a_view_4860c0), 1u, false));
    const Address state = address(norrath_actors_a_view_483e5c);
    const unsigned next = read_word(state) * 0x00019660du + 0x3c6ef35fu;
    write_word(state, next);
    const unsigned mixed = next >> 16;
    const int divisor = static_cast<int>(read_word(a + 80u));
    write_word(a + 84u, static_cast<unsigned>(static_cast<int>(mixed) % divisor));
    return mixed;
}
