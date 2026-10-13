// Observed msg_hurt__6PlayerfP10GameObjecti at 0x001a46a8, 996 bytes. damage is the float argument,
// attacker the object argument (may be null) and flags the integer argument. R is the player record
// player + word(+960) * 23984. In order:
//   1. Return at once when any of these holds: recallOn is non-zero; the halfword at +1188 is non-zero;
//      the word at +1328 is non-zero; compare_text(levelName + 81, "actintro") is zero; jumpGateJumping__Fv()
//      is non-zero; gameControlPaused is non-zero.
//   2. When interfaceOn is non-zero and netPlayerIsLocal__Fi(word +960) is non-zero:
//      engineSetScreenColor__Fiiii(64, 0, 0, 0).
//   3. When attacker is non-null, the word at +1400 is 8 or 11 and bit 0x40 of flags is clear:
//      one step of the shared generator (syncRandState = syncRandState * 1664525 + 1013904223; r = its
//      upper 16 bits mod 65536). The signed byte R[23582] is k. animAddOneShot__FP14AnimationStateP15
//      AnimationHeaderffi(object + 288, header, rate, rate, 2066) with the header taken from the table
//      whose address is word(+36), at byte offset k * 104 + 148 when r <= 44999 (rate 0.2, the single
//      0x3e4ccccc) and k * 104 + 140 otherwise (rate 0.1, 0x3dcccccc). Then with c = icos(half +56),
//      s = isin(half +56), dx = attacker x - object x (floats +16) and dy = attacker y - object y
//      (floats +20): when c * dx + s * dy > 0, the animation named "hit_mtlplate_bludg_med.anm" is looked
//      up in the lump "fx.lmp" (the name goes through sprintf into a stack buffer first) and played with
//      soundPlaySequence at the object's position (floats +16..+24) at rate 1.0, playerBlockSparks__6Playeri
//      (object, 0) is called, and damage becomes 0 unless bit 0x20000 of flags is set, in which case it
//      is halved.
//   4. When checkIfSpellIsRunning__8CreatureRC9type_info(object, __tf15SpellHolyShield()) returns a spell
//      and the float R[196] is positive: a = manaAbsorbOnHit__15SpellHolyShield(spell) * damage;
//      damage -= a; R[196] -= a (stored before the clamps); damage is raised to 0 if negative and R[196]
//      set to 0 if it went negative.
//   5. The float +192 gets damage added; the word +744 gets flags or-ed in.
//   6. When attacker is non-null: unless bit 0x800000 of the new +744 is set, halfword +748 =
//      16384 - iatan2__Fff(object x - attacker x, object y - attacker y); then listDestroy__FP8ListHead
//      on the head at +752; the link target is the attacker, replaced by the word at +12 of the word at
//      +76 when bit 0x80 of the attacker's word +12 is set and that +76 word is non-zero; when it is
//      non-null listOnewayLink__FP8ListHeadT0Pv(head, target, target) is called.
//   7. When the float +192 is positive, bit 0 of gameFlags[word +960] is set.
// Inferred from names only: this is damage handling for a player; +192 holds health taken; +744/+748
// remember the last hit's flags and direction; R[196] is a resource pool the shield draws from.
// Unknown: the roles of +1188, +1328, +1400, R[23582] and the table at word(+36). Nothing here ran on the EE.
#include "../w11-actor-b-common/access.hpp"

extern "C" {
extern signed char recallOn;
extern signed char interfaceOn;
extern int gameControlPaused;
extern int syncRandState;
extern int gameFlags[];
extern char levelName[];
extern unsigned char player[];
int compare_text(const char* a, const char* b) __asm__("strcmp");
int format_text(char* destination, const char* format, ...) __asm__("sprintf");
int jumpGateJumping__Fv();
int netPlayerIsLocal__Fi(int index);
void engineSetScreenColor__Fiiii(int r, int g, int b, int a);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
float icos__Fi(int angle);
float isin__Fi(int angle);
char* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence(void* header, Point3 position, float rate) __asm__("soundPlaySequence__FP15AnimationHeaderG6Point3f");
void playerBlockSparks__6Playeri(void* self, int arg);
void* __tf15SpellHolyShield();
void* checkIfSpellIsRunning__8CreatureRC9type_info(void* self, void* type);
float manaAbsorbOnHit__15SpellHolyShield(void* spell);
int iatan2__Fff(float y, float x);
void listDestroy__FP8ListHead(void* head);
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* other, void* object);

void msg_hurt__6PlayerfP10GameObjecti(void* self, float damage, void* attacker, int flags) {
    using namespace w11b;
    unsigned char* o = static_cast<unsigned char*>(self);
    unsigned char* a = static_cast<unsigned char*>(attacker);
    unsigned char* record = player + sword(o, 960) * 23984;
    if (recallOn != 0) return;
    if (half(o, 1188) != 0) return;
    if (word(o, 1328) != 0) return;
    if (compare_text(levelName + 81, "actintro") == 0) return;
    if (jumpGateJumping__Fv() != 0) return;
    if (gameControlPaused != 0) return;
    if (interfaceOn != 0 && netPlayerIsLocal__Fi(sword(o, 960)) != 0) engineSetScreenColor__Fiiii(64, 0, 0, 0);
    if (a != nullptr && (sword(o, 1400) == 8 || sword(o, 1400) == 11) && (flags & 0x40) == 0) {
        const unsigned state = static_cast<unsigned>(syncRandState) * 0x19660du + 0x3c6ef35fu;
        syncRandState = static_cast<int>(state);
        const int r = static_cast<int>(state >> 16) % 65536;
        const int k = *reinterpret_cast<signed char*>(record + 23582);
        unsigned char* table = from_word(word(o, 36));
        if (r <= 44999)
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(o + 288, from_word(word(table, k * 104 + 148)), bits(0x3e4ccccc), bits(0x3e4ccccc), 2066);
        else
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(o + 288, from_word(word(table, k * 104 + 140)), bits(0x3dcccccc), bits(0x3dcccccc), 2066);
        const float c = icos__Fi(half(o, 56));
        const float s = isin__Fi(half(o, 56));
        const float dx = real(a, 16) - real(o, 16);
        const float dy = real(a, 20) - real(o, 20);
        if (c * dx + s * dy > 0.0f) {
            char name[128];
            format_text(name, "hit_mtlplate_bludg_med.anm");
            soundPlaySequence(lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), name), *reinterpret_cast<Point3*>(o + 16), 1.0f);
            playerBlockSparks__6Playeri(o, 0);
            if ((flags & 0x20000) != 0) damage = damage * 0.5f;
            else damage = 0.0f;
        }
    }
    void* spell = checkIfSpellIsRunning__8CreatureRC9type_info(o, __tf15SpellHolyShield());
    if (spell != nullptr && real(record, 196) > 0.0f) {
        const float absorbed = damage * manaAbsorbOnHit__15SpellHolyShield(spell);
        damage = damage - absorbed;
        const float pool = real(record, 196) - absorbed;
        real(record, 196) = pool;
        if (damage < 0.0f) damage = 0.0f;
        if (pool < 0.0f) real(record, 196) = 0.0f;
    }
    real(o, 192) = real(o, 192) + damage;
    const unsigned merged = word(o, 744) | static_cast<unsigned>(flags);
    word(o, 744) = merged;
    if (a != nullptr) {
        if ((merged & 0x800000u) == 0)
            half(o, 748) = static_cast<short>(16384 - iatan2__Fff(real(o, 16) - real(a, 16), real(o, 20) - real(a, 20)));
        unsigned char* head = o + 752;
        listDestroy__FP8ListHead(head);
        unsigned target = to_word(a);
        if ((word(a, 12) & 0x80u) != 0 && word(a, 76) != 0) target = word(from_word(word(a, 76)), 12);
        if (target != 0) listOnewayLink__FP8ListHeadT0Pv(head, from_word(target), from_word(target));
    }
    if (real(o, 192) > 0.0f) gameFlags[sword(o, 960)] |= 1;
}
}
