#include "anttrap.hpp"
namespace {
using namespace norrath_actors_a;
struct PointBytes { unsigned x, y, z; };
static_assert(sizeof(PointBytes) == 12 && alignof(PointBytes) == 4, "raw point storage");
PointBytes snapshot(const void* point) {
    const Address a = address(point);
    return PointBytes{read_word(a), read_word(a + 4u), read_word(a + 8u)};
}
}
// __7AntTrapPcG6Point3sPPc. A null resource leaves V0 at 0. A header is stored
// at offset 408, initialized on the state at object+152, and V0 is the half at
// 158 after OR 0x80. The header then receives 1.0 at +20 and byte 1 at +24.
extern "C" unsigned norrath_construct_AntTrap(void* object, char* name, const void* point, short type,
                                              char** arguments) {
    PointBytes local = snapshot(point);
    (void)norrath_construct_AutoProp(object, name, &local, type, arguments);
    const Address a = address(object);
    write_word(a + 72u, address(norrath_actors_a_view_44f1f8));
    write_word(a + 12u, read_word(a + 12u) | 1u);
    void* header = norrath_lump_find_resource(reinterpret_cast<char*>(pointer(a + 109u)),
                                              reinterpret_cast<char*>(norrath_actors_a_view_471310));
    write_word(a + 408u, address(header));
    if (header == nullptr) return 0u;
    void* state = pointer(a + 152u);
    norrath_actor_animation_initialize(state);
    norrath_actor_animation_add(state, header, 4);
    norrath_animation_set_to_end(state);
    const unsigned marked = read_half(a + 158u) | 0x80u;
    write_half(a + 158u, marked);
    write_word(address(header) + 20u, 0x3f800000u);
    bytes(address(header))[24] = 1u;
    return marked;
}
