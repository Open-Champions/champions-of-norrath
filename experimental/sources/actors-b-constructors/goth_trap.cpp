#include "detail.hpp"
// __8GothTrapPcG6Point3sPPc at 0x226218, 116 bytes, plus one padding word before
// the following symbol. After AutoProp: _vt$8GothTrap (0x44f000) at object+72,
// 1 ORed into the word at object+12, objectAddToRunList, then
// setPassableState__8GothTrapb(object, 1). Low V0 is that callee's low V0.
extern "C" unsigned norrath_construct_GothTrap(void* object, char* name, const void* point,
                                              short type, char** arguments) {
    using namespace norrath_actors_b;
    const PointBytes local = snapshot_point(point);
    (void)norrath_construct_AutoProp(object, name, &local, type, arguments);
    const Address a = address(object);
    write_word(a + 72u, address_bits(norrath_actors_b_view_44f000));
    write_word(a + 12u, read_word(a + 12u) | 1u);
    (void)norrath_actor_add_run_list(object);
    return norrath_goth_trap_set_passable_state(object, 1);
}
