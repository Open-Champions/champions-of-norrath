#include "eggsack.hpp"
namespace {
using namespace norrath_actors_a;
struct PointBytes { unsigned x, y, z; };
static_assert(sizeof(PointBytes) == 12 && alignof(PointBytes) == 4, "raw point storage");
PointBytes snapshot(const void* point) {
    const Address a = address(point);
    return PointBytes{read_word(a), read_word(a + 4u), read_word(a + 8u)};
}
}
// __7EggSackPcG6Point3sPPc. Four words of 2 are stored at 452, 448, 444 and 440.
// The return is the address one word below the last store.
extern "C" unsigned norrath_construct_EggSack(void* object, char* name, const void* point, short type,
                                              char** arguments) {
    PointBytes local = snapshot(point);
    (void)norrath_construct_AutoPropLP(object, name, &local, type, arguments);
    const Address a = address(object);
    write_word(a + 72u, address(norrath_actors_a_view_44f2a0));
    Address cursor = a + 452u;
    int remaining = 3;
    do {
        remaining -= 1;
        write_word(cursor, 2u);
        cursor -= 4u;
    } while (remaining >= 0);
    write_word(a + 316u, read_word(a + 316u) | 2u);
    return cursor;
}
