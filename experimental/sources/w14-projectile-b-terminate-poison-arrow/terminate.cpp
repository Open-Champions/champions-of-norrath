// Observed terminate__21PoisonArrowProjectilei at 0x002a0888, 424 bytes. O is the projectile; the integer argument is not used.
// The word at O+96 is a done flag and nothing happens when it is non-zero. Otherwise, in order:
//  1. For angle a = 0, 309, 618, ... while a <= 65535 (213 turns, the last angle being 65508), V = (icos__Fi(a) * 9.0, isin__Fi(a) * 9.0, 0.0) and
//     P_AddParticle__FP11ParticleDefP6Point3T1f(poisonPoof, O+16, V, 0.0) runs (the position is O+16 itself, not a copy).
//  2. When the word Q at O+76 (an object, the owner) is non-zero and blockAlloc__Fi(112) succeeds, a poisonGasSpell is constructed
//     in that block. S is the object that is the word at O+172 and n the signed word at O+104. The entry at offset 88 of the table
//     whose address is the word at S+96 is called on S (adjusted by its signed halfword) with (2, n), giving a, and then with
//     (3, n), giving b. The constructor gets (block, a copy of the point at O+16, b * 60, a * 12, 0, the word at Q+12, 5).
//  3. soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0("fx.lmp", "spel_we_poisarrow_hit.anm"), O, 1.0)
//     runs, objectAddToDeleteList__FP10GameObject(O) runs and the word at O+96 becomes 1.
// Inferred: the burst and poison cloud left where a poison arrow ends; the table slot at 88 gives per-level numbers. Unknown: what
// the two slot results and the constructor's integers mean, and the use of the discarded argument.
// Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
extern unsigned char poisonPoof[];
float icos__Fi(int angle);
float isin__Fi(int angle);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float age);
void* blockAlloc__Fi(int size);
void poisonGasSpell_construct(void* self, Point3 where, int a, int b, int c, void* owner, int d) __asm__("__14poisonGasSpellG6Point3iiiP10GameObjecti");
void* lumpFindResource__FPcT0(const char* lump, const char* resource);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void objectAddToDeleteList__FP10GameObject(void* object);

void terminate__21PoisonArrowProjectilei(void* self, int) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (word(o, 96) != 0u) return;
    Point3 velocity;
    velocity.z = 0.0f;
    for (int angle = 0; angle <= 0xffff; angle += 309) {
        velocity.x = icos__Fi(angle) * 9.0f;
        velocity.y = isin__Fi(angle) * 9.0f;
        P_AddParticle__FP11ParticleDefP6Point3T1f(poisonPoof, reinterpret_cast<Point3*>(o + 16), &velocity, 0.0f);
    }
    if (word(o, 76) != 0u) {
        void* const block = blockAlloc__Fi(112);
        if (block != nullptr) {
            unsigned char* const s = from_word(word(o, 172));
            const int a = static_cast<int>(call_entry_from(s, 96, 88, 2, word(o, 104)));
            const int b = static_cast<int>(call_entry_from(s, 96, 88, 3, word(o, 104)));
            poisonGasSpell_construct(block, *reinterpret_cast<Point3*>(o + 16), b * 60, a * 12, 0, from_word(word(from_word(word(o, 76)), 12)), 5);
        }
    }
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0("fx.lmp", "spel_we_poisarrow_hit.anm"), o, 1.0f);
    objectAddToDeleteList__FP10GameObject(o);
    word(o, 96) = 1;
}
}
