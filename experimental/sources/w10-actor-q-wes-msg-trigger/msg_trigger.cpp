#include "../w10-actor-q-common/q.hpp"
// Observed msg_trigger__14WoodElfSoldieri at 0x002484d0, 444 bytes.
// When findTagString__10GameObjectPci(object, <tag text at 0x004745e0>, 0) finds a tag, the trigger is
// forwarded unchanged to msg_trigger__8Creaturei(object, trigger) and nothing else happens.
// Otherwise: bit 0x400 of the word at +244 is cleared and written back, then
// modelGetActiveChunks__FP12_modelHeaderi(model, that word) is stored as a doubleword at +552, where
// model is the word at (word at +36) + 12. A position is built from the Point3 at +16 with 58.0f added
// to z, and a velocity (icos(h) * 400, isin(h) * 400, 0) with h the signed halfword at +56. A name is
// formatted from the format at 0x00474580 with (eRand & 3) + 1, looked up in the lump "fx.lmp" and
// played with soundPlaySequence at gain 1.0 on the object. A 452-byte block is then
// allocated; if that fails the function ends, otherwise the MissileWeapon constructor runs on it with
// (0, the words at +8 and +12 of item::globalItem, position, velocity, h - 16384, the object, no
// sounds, signed halfword at +200, signed halfword at +202 divided by two, 0, 0, signed halfword at
// +204, 4, no description). Nothing is returned.
// Inferred: this fires an arrow at the heading when triggered. Unknown: what the three halfwords at
// +200, +202 and +204 mean. Nothing here ran on the EE.
extern "C" {
void* findTagString__10GameObjectPci(void* self, const char* tag, int occurrence);
void msg_trigger__8Creaturei(void* self, int trigger);
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int requested);
float icos__Fi(int angle);
float isin__Fi(int angle);
int eRand__10GameObject(void* self);
int sprintf(char* destination, const char* format, ...);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* animation, void* object, float gain);
void* blockAlloc__Fi(int size);
void __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
    void* self, int zero, void* model, void* texture, const float* position, const float* velocity, int angle,
    void* creature, void* sound_a, void* sound_b, int a, int b, int c, int d, int e, int f, void* description);
extern unsigned char globalItem[] __asm__("_4item$globalItem");
}

extern "C" void msg_trigger__14WoodElfSoldieri(void* self, int trigger) {
    using namespace w10q;
    static char dialogTag[] = {'d', 'i', 'a', 'l', 'o', 'g', 'S', 'c', 'r', 'i', 'p', 't', '\0'};
    if (findTagString__10GameObjectPci(self, dialogTag, 0) != nullptr) {
        msg_trigger__8Creaturei(self, trigger);
        return;
    }
    const unsigned flags = word(self, 244) & ~0x400u;
    word(self, 244) = flags;
    unsigned char* resource = link(self, 36);
    *reinterpret_cast<Double*>(bytes(self) + 552) =
        modelGetActiveChunks__FP12_modelHeaderi(link(resource, 12), static_cast<int>(flags));
    float position[3];
    position[0] = real(self, 16);
    position[1] = real(self, 20);
    position[2] = real(self, 24) + 58.0f;
    const int heading = shalf(self, 56);
    float velocity[3];
    velocity[0] = icos__Fi(heading) * 400.0f;
    velocity[1] = isin__Fi(heading) * 400.0f;
    velocity[2] = 0.0f;
    char name[64];
    sprintf(name, "swsh_arrow%d.anm", static_cast<int>(static_cast<unsigned>(eRand__10GameObject(self)) & 3u) + 1);
    static char lump[] = {'f', 'x', '.', 'l', 'm', 'p', '\0'};
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(lump, name), self, 1.0f);
    void* block = blockAlloc__Fi(452);
    if (block == nullptr) return;
    __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
        block, 0, from_word(word(globalItem, 8)), from_word(word(globalItem, 12)), position, velocity, heading - 16384, self,
        nullptr, nullptr, shalf(self, 200), shalf(self, 202) / 2, 0, 0, shalf(self, 204), 4, nullptr);
}
