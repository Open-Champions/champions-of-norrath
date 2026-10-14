// Observed __9DustDevilG6Point3 at 0x23c060, 128 bytes. Arguments: the object and a three-float
// position (by value). Steps, in order:
//  1. The GameObject constructor runs on the object with the position, a null resource map and
//     a null fourth argument.
//  2. The word at +12 gets bits 0x9 set (stored before the next call) and the word at +72 gets the
//     class table _vt$9DustDevil.
//  3. P_CreateParticleSystem__Fv is called with no arguments; its result is stored at +84.
//  4. The word at +76 is cleared and the word at +80 is set to frameNm + 60.
//  5. objectAddToRunList__FP10GameObject runs on the object.
// The routine returns nothing useful. Inferred: +84 holds a particle system handle because of
// the callee's name. Unknown: what +76 and +80 mean, and why 60 is added. Nothing here ran on
// the EE.
#include "../w11-ctor-a-common/ctor.hpp"

extern "C" {
extern unsigned char vt_DustDevil[] __asm__("_vt$9DustDevil");
void __9DustDevilG6Point3(void* self, Point3 position);
}

extern "C" void __9DustDevilG6Point3(void* self, Point3 position) {
    using namespace w11;
    game_object_construct(self, position, nullptr, nullptr);
    u32(self, 72) = word(vt_DustDevil);
    u32(self, 12) |= 0x9u;
    void* system = P_CreateParticleSystem__Fv();
    u32(self, 84) = word(system);
    u32(self, 76) = 0;
    i32(self, 80) = frameNm + 60;
    objectAddToRunList__FP10GameObject(self);
}
