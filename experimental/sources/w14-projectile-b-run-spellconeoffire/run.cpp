// Observed run__15SpellConeOfFireG6Point3s at 0x002b5ad8, 3248 bytes. Arguments: the spell S, a point ORG (three floats, by address, copied to
// a local) and a signed halfword angle. frameNm is the global frame counter. "caster" is the object that is the word at S+8, "owner" the object
// that is the word at S+12. The signed byte at S+0 is passed on to the caster calls. Pool constants with no symbols (bits): KA = 0x409ffe80
// (4.9998), KB = 0x426fffd7 (59.9998), KC = 0x4270000d (60.00005), KD = 0x40a0000e (5.000007), KE = 0x42200057 (40.0003), KF = 0x3e4ccccc (0.2).
// A "unit-ball vector" is drawn by rejection: each component is eRand__5Spell(S) * 2^-16 doubled minus 1.0, x then y then z, repeated while 1.0 <
// (x*x + y*y) + z*z. Three particle definitions are 80-byte bss objects of the image (file-local symbols coneoffireParticle1 0x008d7b30,
// coneoffireParticle2 0x008d7b80 and coneoffireParticle3 0x008d7bd0), imported through address-named views. Steps in order:
//  1. q = (int)((isin(frameNm * 3500) + isin(frameNm * 1750)) * 10.0) + 80; the light embedded at S+24 gets its singles at S+52 and S+60
//     set to (float)q and those at S+56 and S+64 set to 1.0 / ((float)q * (float)q). (lightUpdatePosition is not called.)
//  2. n is the entry at table offset 104 of the table that is the word at +96 of the caster, called on the caster (adjusted by the entry's signed
//     halfword) with (1, the signed byte at S+0, the owner). lo = angle - 91 * n and spread = 182 * n.
//  3. Three particle loops. Each iteration draws a unit-ball vector V, X = V * K + ORG (K per loop), draws a second unit-ball vector W, then with
//     r1 and r2 two further eRand__5Spell(S) draws ((float)r * 2^-16): W.x += icos((int)((float)lo + r1 * (float)spread)) * L and W.y +=
//     isin((int)((float)lo + r2 * (float)spread)) * L (L per loop); P_AddParticle__FP11ParticleDefP6Point3T1f(definition, X, W, 0.0) runs.
//     Loop 1: 20 times, K = KA, L = KB, definition 1. Loop 2: 5 times, K = 5.0, L = KC, definition 2. Loop 3: 3 times, K = KD, L = KE, definition 3.
//  4. Nothing more unless frameNm % 15 == 0. Then a list head (first word zero) is built on the stack; CP = (icos(angle) * 12.0, isin(angle) * 12.0,
//     0.0); m is the same caster entry-104 call as in step 2; C2 = ORG - CP in x and y and ORG.z in z;
//     objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(C2, angle, 96.0, (float)(m + 10), list, null) fills the list. For each node N
//     in order (a node's next is the word at N+0), with E the word at N+12: the entry at table offset 160 of the owner is called on the owner with
//     E and a zero result skips the node. Otherwise a = (int)((float)p * 0.25) where p is the caster entry-104 call again but with the first argument word 0; an item description with
//     no entries set (see spell.hpp) gets the first free (or kind-21) entry (21, a, b, 0) with b = (int)(((float)a + ((float)a * ((float)(eRand__5Spell(S))
//     * 2^-16)) * KF) * 0.25); gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(E, owner, 0.0, 0.0, 100000, 0, 0, 64, description,
//     false) runs and a hit flag is set. After the list: when the flag is set, soundPlaySequence__FP15AnimationHeaderG6Point3f(
//     lumpFindResource__FPcT0("fx.lmp", "spel_ew_cfire_hit.anm"), the point at owner+16, 1.0) plays. Finally listDestroy__FP8ListHead(list) runs.
// Inferred: a cone of fire in front of a caster that sprays particles and burns creatures found in a sorted cone query every 15 frames. Unknown:
// what the byte at S+0, the entry at 104, the constants and the second item-description argument mean. Nothing here ran on the EE.
#include "../w14-projectile-b-common/spell.hpp"

extern "C" {
extern int frameNm;
extern unsigned char norrath_w13_spell_msg_a_view_8d7b30[80];
extern unsigned char norrath_w13_spell_msg_a_view_8d7b80[80];
extern unsigned char norrath_w13_spell_msg_a_view_8d7bd0[80];
int eRand__5Spell(void* spell);
float icos__Fi(int angle);
float isin__Fi(int angle);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float f);
void objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(Point3 center, short angle, float a, float b, void* list, void* creature);
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* attacker, void* target, float a, float b, int c, int d, int e, int f,
                                                                    void* description, bool g);
void* lumpFindResource__FPcT0(char* lump, char* name);
int soundPlaySequence__FP15AnimationHeaderG6Point3f(void* header, Point3 p, float volume);
void listDestroy__FP8ListHead(void* head);
}

static int roll(unsigned char* s) { return eRand__5Spell(s); }

static int caster_amount(unsigned char* s, int mode) {
    using namespace w12c;
    return w13a::call_entry_104_mode(from_word(word(s, 8)), mode, *reinterpret_cast<signed char*>(s), from_word(word(s, 12)));
}

static void spray(unsigned char* s, const Point3& origin, void* definition, float scale, float launch, int lo, int spread, int count) {
    using namespace w12c;
    for (int i = 0; i < count; ++i) {
        Point3 v;
        unit_ball(s, v, roll);
        Point3 x;
        x.x = v.x * scale;
        x.y = v.y * scale;
        x.z = v.z * scale;
        x.x = x.x + origin.x;
        x.y = x.y + origin.y;
        x.z = x.z + origin.z;
        Point3 w;
        unit_ball(s, w, roll);
        const float spreadF = static_cast<float>(spread);
        const float loF = static_cast<float>(lo);
        const float first = (static_cast<float>(eRand__5Spell(s)) * bits(0x37800000u)) * spreadF;
        w.x = w.x + icos__Fi(static_cast<int>(loF + first)) * launch;
        const float second = (static_cast<float>(eRand__5Spell(s)) * bits(0x37800000u)) * spreadF;
        w.y = w.y + isin__Fi(static_cast<int>(loF + second)) * launch;
        P_AddParticle__FP11ParticleDefP6Point3T1f(definition, &x, &w, 0.0f);
    }
}

extern "C" void run__15SpellConeOfFireG6Point3s(void* self, Point3* originPointer, short angle) {
    using namespace w12c;
    unsigned char* const s = static_cast<unsigned char*>(self);
    const Point3 origin = *originPointer;
    const float wobble = (isin__Fi(frameNm * 3500) + isin__Fi(frameNm * 1750)) * 10.0f;
    unsigned char* const light = s + 24;
    const float q = static_cast<float>(static_cast<int>(wobble) + 80);
    real(light, 28) = q;
    real(light, 36) = q;
    const float inverse = 1.0f / (q * q);
    real(light, 40) = inverse;
    real(light, 32) = inverse;
    const int n = caster_amount(s, 1);
    const int lo = angle - 91 * n;
    const int spread = 182 * n;
    spray(s, origin, norrath_w13_spell_msg_a_view_8d7b30, bits(0x409ffe80u), bits(0x426fffd7u), lo, spread, 20);
    spray(s, origin, norrath_w13_spell_msg_a_view_8d7b80, 5.0f, bits(0x4270000du), lo, spread, 5);
    spray(s, origin, norrath_w13_spell_msg_a_view_8d7bd0, bits(0x40a0000eu), bits(0x42200057u), lo, spread, 3);
    if (frameNm % 15 != 0) return;
    unsigned char list[16];
    word(list, 0) = 0;
    Point3 cp;
    cp.x = icos__Fi(angle) * 12.0f;
    cp.y = isin__Fi(angle) * 12.0f;
    cp.z = 0.0f;
    Point3 center;
    center.x = origin.x - cp.x;
    center.y = origin.y - cp.y;
    center.z = origin.z;
    const int m = caster_amount(s, 1);
    objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(center, angle, 96.0f, static_cast<float>(m + 10), list, nullptr);
    bool hit = false;
    for (unsigned char* node = from_word(word(list, 0)); node != nullptr; node = from_word(word(node, 0))) {
        unsigned char* const e = from_word(word(node, 12));
        if (call_entry(from_word(word(s, 12)), 160, to_word(e)) == 0) continue;
        const int a = static_cast<int>(static_cast<float>(caster_amount(s, 0)) * 0.25f);
        unsigned char description[80];
        w13a::init_description(description);
        const float af = static_cast<float>(a);
        const float r = static_cast<float>(eRand__5Spell(s)) * bits(0x37800000u);
        const float grown = af + (af * r) * bits(0x3e4ccccc);
        const int b = static_cast<int>(grown * 0.25f);
        w13a::fill_description(description, 21, static_cast<short>(a), static_cast<short>(b), 0);
        gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(e, from_word(word(s, 12)), 0.0f, 0.0f, 100000, 0, 0, 64, description, false);
        hit = true;
    }
    if (hit) {
        void* const header = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_ew_cfire_hit.anm"));
        Point3 at = *reinterpret_cast<Point3*>(from_word(word(s, 12)) + 16);
        soundPlaySequence__FP15AnimationHeaderG6Point3f(header, at, 1.0f);
    }
    listDestroy__FP8ListHead(list);
}
