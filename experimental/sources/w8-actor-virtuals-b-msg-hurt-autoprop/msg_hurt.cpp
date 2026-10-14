// Observed msg_hurt__8AutoPropfP10GameObjecti at 0x00224d70, 264 bytes.
// The float, the GameObject pointer and the int are not read.
// Does nothing when the word at object+288 is zero or when none of bits 2 and 3
// (mask 0xc) of the word at object+284 is set. Otherwise:
//   1. calls the table entry at byte offset 136 (advanceState in the AutoProp family
//      tables) through the table at the word at object+72;
//   2. compares the name text at object+76 with a short resource name, ignoring case;
//   3. if it matches, plays one sound sequence for object (volume 1.0): lump
//      "gothtrap.lmp", resource "cs_wallcrumble.anm"; otherwise plays two: lump "fx.lmp"
//      resource "spel_he_repulse.anm", then lump "fx.lmp" resource "hit_chnmail_bludg_hvy.anm".
// The image keeps these text constants in its shared string area at 0x470ec8, 0x470ed8,
// 0x470ee8, 0x470f00, 0x470f18 and 0x485508; this source holds its own copies.
// Unknown: what the two flag bits mean.

#include "../w8-actor-virtuals-b-common/virtual_call.hpp"

extern "C" {
int strcasecmp(const char* left, const char* right);
void* lumpFindResource__FPcT0(const char* lump, const char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume);
}

extern "C" void msg_hurt__8AutoPropfP10GameObjecti(void* self, float amount, void* attacker, int flags) {
    (void)amount;
    (void)attacker;
    (void)flags;
    unsigned char* object = static_cast<unsigned char*>(self);
    if (*reinterpret_cast<const w8avb::UWord*>(object + 288) == 0) return;
    if ((*reinterpret_cast<const w8avb::UWord*>(object + 284) & 0xc) == 0) return;
    w8avb::call_slot(self, 136);
    if (strcasecmp(reinterpret_cast<const char*>(object + 76), "gothtrap") == 0) {
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
            lumpFindResource__FPcT0("gothtrap.lmp", "cs_wallcrumble.anm"), self, 1.0f);
    } else {
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
            lumpFindResource__FPcT0("fx.lmp", "spel_he_repulse.anm"), self, 1.0f);
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
            lumpFindResource__FPcT0("fx.lmp", "hit_chnmail_bludg_hvy.anm"), self, 1.0f);
    }
}
