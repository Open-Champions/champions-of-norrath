#include "../w10-actor-l-common/access.hpp"
// Observed msg_poke__8RollDoori at 0x0021c4e0, 116 bytes. The incoming integer is unused. First it
// clears bit 20 (mask 0xffefffff) of the flag word at object+12, always. Then, only if the
// halfword at +76 is not negative: adds the object to the run list, looks up the animation
// "door_stn_open.anm" in the lump "fx.lmp" (lumpFindResource, lump first), starts that
// sequence on the object at rate 1.0 and stores 1 in the halfword at +90. Nothing is returned.
// Unknown: what bit 20 and the halfwords mean. Nothing here ran on the EE.
extern "C" {
void objectAddToRunList__FP10GameObject(void* object);
char* lumpFindResource__FPcT0(char* lump, char* name);
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float rate);

void msg_poke__8RollDoori(void* self, int) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    w10l::word(o, 12) &= 0xffefffffu;
    if (w10l::half(o, 76) < 0) return;
    objectAddToRunList__FP10GameObject(self);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
        lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("door_stn_open.anm")), self, 1.0f);
    w10l::half(o, 90) = 1;
}
}
