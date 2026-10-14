#include "detail.hpp"
// __11AutoChestLPPcG6Point3sPPc at 0x225250, 144 bytes.
// Observed: the 12 point bytes are copied, the short in the fourth argument is
// sign-extended, and __9AutoChestPcG6Point3sPPc receives this, the name, that
// copy, the short, and the argument list. _vt$11AutoChestLP (0x44ea78) is stored
// at object+72 before blockAlloc(388). A null allocation skips
// __11AddParticleG6Point3PPc and leaves V0 at 0. A live allocation constructs
// AddParticle on that block with the same point copy and argument list, and V0
// is AddParticle's V0. Either way the allocation word is stored at object+248.
extern "C" unsigned norrath_construct_AutoChestLP(void* object, char* name, const void* point,
                                                 short type, char** arguments) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    (void)norrath_construct_AutoChest(object, name, &local, type, arguments);
    const Address a = address(object);
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44ea78));
    unsigned char* particle = norrath_block_alloc(388);
    unsigned result = 0u;
    if (particle != nullptr)
        result = __11AddParticleG6Point3PPc(particle, &local, arguments);
    write_word(a + 248u, address_bits(particle));
    return result;
}
