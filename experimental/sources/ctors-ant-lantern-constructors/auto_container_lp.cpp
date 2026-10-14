#include "detail.hpp"
// __15AutoContainerLPPcG6Point3sPPc at 0x225518, 144 bytes.
// Same control as AutoChestLP: point copy, sign-extended short, then
// __13AutoContainer. _vt$15AutoContainerLP (0x44e940) is stored at object+72
// before blockAlloc(388). A null block skips __11AddParticle and leaves V0 at
// 0. A live block is constructed with the same point copy and argument list,
// and V0 is that result. The allocation word is stored at object+328.
extern "C" unsigned norrath_construct_AutoContainerLP(void* object, char* name, const void* point,
                                                     short type, char** arguments) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    (void)norrath_construct_AutoContainer(object, name, &local, type, arguments);
    const Address a = address(object);
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44e940));
    unsigned char* particle = norrath_block_alloc(388);
    unsigned result = 0u;
    if (particle != nullptr)
        result = __11AddParticleG6Point3PPc(particle, &local, arguments);
    write_word(a + 328u, address_bits(particle));
    return result;
}
