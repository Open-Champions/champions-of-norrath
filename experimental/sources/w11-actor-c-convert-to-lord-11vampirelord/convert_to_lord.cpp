#include "../w11-actor-c-common/c.hpp"
// Observed convertToLord__11VampireLord at 0x00243748, 180 bytes. With v the object, in the order of the image:
//   the three floats at v+16 are copied to a local Point3 and makeExplosion__FG6Point3f(point, 4.0) runs;
//   the "spel_we_explarrow_hit.anm" sequence is looked up in the "fx.lmp" lump and played on v through
//   soundPlaySequence__FP15AnimationHeaderP10GameObjectf with volume 1.0;
//   the word at v+964 becomes 0, the byte at v+292 becomes 0, and the animation at word(+28) of the block
//   the pointer at v+36 refers to is added to the state at v+288 with animAdd (flags 1);
//   finally bit 0 of the word at v+12 is cleared and bit 20 (0x100000) of the 64-bit word at v+160 is
//   cleared. That undoes convertToBat__11VampireLord.
// Inferred: this turns the bat form back into the lord with an explosion effect. The meaning of the v+36
// block is not established (the vampire's model/animation set, by its use here). Nothing here ran on the EE.
struct Point3 {
    float x, y, z;
};
extern "C" {
void makeExplosion__FG6Point3f(Point3 position, float scale);
void* lumpFindResource__FPcT0(const char* lump, const char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flags);
}

extern "C" void convertToLord__11VampireLord(void* self) {
    using namespace w11c;
    Point3 position = {real(self, 16), real(self, 20), real(self, 24)};
    makeExplosion__FG6Point3f(position, 4.0f);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0("fx.lmp", "spel_we_explarrow_hit.anm"), self, 1.0f);
    word(self, 964) = 0u;
    unsigned char* block = link(self, 36);
    ubyte(self, 292) = 0;
    animAdd__FP14AnimationStateP15AnimationHeaderi(bytes(self) + 288, from_word(word(block, 28)), 1);
    word(self, 12) &= ~1u;
    *reinterpret_cast<Double*>(bytes(self) + 160) &= ~0x100000ull;
}
