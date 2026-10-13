// Observed P_SetParticleSystemMatrix__FiP8Matrix34 at 0x001803c0, extent 164. For
// a nonzero id whose PS record (0x006f5ad0, stride 312, selected by the low byte
// of the id) holds that id at +304, the 48 bytes at the matrix argument are
// copied to offset 256 of the record. Any other case changes nothing. Returns
// nothing. Matrix34 is twelve floats (see the shared math types).
// Unknown: what the particle code does with the stored matrix.
#include "../w9-render-math-b-common/math_types.hpp"
extern "C" {
extern unsigned char PS[];
void P_SetParticleSystemMatrix__FiP8Matrix34(int id, const Matrix34* matrix) {
    if (id != 0) {
        unsigned char* record = PS + (static_cast<unsigned>(id) & 0xffu) * 312u;
        if (*reinterpret_cast<int*>(record + 304) == id) *reinterpret_cast<Matrix34*>(record + 256) = *matrix;
    }
}
}
