#include "../w10-actor-l-common/access.hpp"
// Observed msg_run__5Rondo at 0x00232b78, 720 bytes. Per-frame step. Sequence:
//  1. newDamageHandler(object, model) where model is the word at +12 of the group at object+36;
//     then everyFrame(object, model); a nonzero result ends the call.
//  2. objectActivationCheck(object, 600); advance = creatureAdvanceAnimation(object, 1.0);
//     slideBack(object, 0.3 (the constant stored in the image)).
//  3. The table at object+72 has entries of {signed halfword adjustment, function word at +4}.
//     Entry at table offset 168 is called with the adjusted object only.
//  4. The signed halfword at +572 selects a case (11 or more is the default). "Call184(a)" is the
//     entry at table offset 184 called with the adjusted object and the integer a.
//     0: bFarFromHome nonzero -> Call184(1); else enemy = alternateEnemyInZone(object, short at
//        +56, 32767, 0.0, 20.0): nonzero -> Call184(6); else Call184(1) when word +256 < word
//        +808 (signed), else nothing.
//     1: enemy as in case 0: nonzero -> Call184(6); else copy of the Point3 at +272 goes to
//        steerAndRouteToPosition: nonzero -> Call184(0); else Call184(0) when word +256 < +808.
//     5: bExitStatePersueIdle nonzero -> Call184(0).
//     6: when word +576 is nonzero: if bit 0 of `advance` is set, produceMeleeDamage(object,
//        36.0, halfwords +200, +202, +204, 1, 0); then turnToFaceEnemy. Then, unless bit 8 of the
//        halfword at +294 is set, Call184(0).
//     7: Call184(0) when word +256 < +808; else when the word at +960 is nonzero flee2(object,
//        word at +12 of what +960 points to); otherwise Call184(0).
//     8: Call184(0) unless bit 16 of the halfword at +294 is set.
//     9: Call184(0) when word +804 < word +808 (signed).
//    10: objectRemoveFromRunList(object) when bit 128 of the halfword at +294 is set.
//     2, 3, 4 and 11 or more: Call184(0).
// Unknown: state names, the meaning of the words at +256, +576, +804, +808, +960 and of the
// halfwords at +200..+204 and +294. Nothing here ran on the EE.
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
void objectActivationCheck__8Creaturei(void* self, int range);
int creatureAdvanceAnimation__8Creaturef(void* self, float step);
void slideBack__8Creaturef(void* self, float amount);
int bFarFromHome__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void* alternateEnemyInZone__8Creatureffss(void* self, float a, float b, short c, short d);
int steerAndRouteToPosition__8CreatureG6Point3(void* self, const void* position);
void flee2__8CreatureP10GameObject(void* self, void* other);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float a, int b, int c, int d, int e, void* f);
void turnToFaceEnemy__8Creature(void* self);
void objectRemoveFromRunList__FP10GameObject(void* object);

void msg_run__5Rondo(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    void* const model = w10l::from_word(w10l::word(w10l::from_word(w10l::word(o, 36)), 12));
    newDamageHandler__8CreatureP12_modelHeader(self, model);
    if (everyFrame__8CreatureP12_modelHeader(self, model) != 0) return;
    objectActivationCheck__8Creaturei(self, 600);
    const int advance = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, 0.3f);
    w10l::call_entry(o, 168);
    const auto call184 = [&](unsigned long argument) { w10l::call_entry(o, 184, argument); };
    switch (static_cast<unsigned>(static_cast<int>(w10l::half(o, 572)))) {
    case 0:
        if (bFarFromHome__8Creature(self) != 0) { call184(1); break; }
        if (alternateEnemyInZone__8Creatureffss(self, 0.0f, 20.0f, w10l::half(o, 56), 32767) != nullptr) { call184(6); break; }
        if (w10l::sword(o, 256) < w10l::sword(o, 808)) call184(1);
        break;
    case 1: {
        if (alternateEnemyInZone__8Creatureffss(self, 0.0f, 20.0f, w10l::half(o, 56), 32767) != nullptr) { call184(6); break; }
        Point3 target = *reinterpret_cast<Point3*>(o + 272);
        if (steerAndRouteToPosition__8CreatureG6Point3(self, &target) != 0) { call184(0); break; }
        if (w10l::sword(o, 256) < w10l::sword(o, 808)) call184(0);
        break;
    }
    case 5:
        if (bExitStatePersueIdle__8Creature(self) != 0) call184(0);
        break;
    case 6:
        if (w10l::word(o, 576) != 0) {
            if ((advance & 1) != 0)
                produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, 36.0f, w10l::half(o, 200), w10l::half(o, 202), w10l::half(o, 204), 1, nullptr);
            turnToFaceEnemy__8Creature(self);
        }
        if ((static_cast<unsigned short>(w10l::half(o, 294)) & 8u) == 0) call184(0);
        break;
    case 7:
        if (w10l::sword(o, 256) < w10l::sword(o, 808)) { call184(0); break; }
        if (w10l::word(o, 960) != 0)
            flee2__8CreatureP10GameObject(self, w10l::from_word(w10l::word(w10l::from_word(w10l::word(o, 960)), 12)));
        else
            call184(0);
        break;
    case 8:
        if ((static_cast<unsigned short>(w10l::half(o, 294)) & 0x10u) == 0) call184(0);
        break;
    case 9:
        if (w10l::sword(o, 804) < w10l::sword(o, 808)) call184(0);
        break;
    case 10:
        if ((static_cast<unsigned short>(w10l::half(o, 294)) & 0x80u) != 0) objectRemoveFromRunList__FP10GameObject(self);
        break;
    default:
        call184(0);
        break;
    }
}
}
