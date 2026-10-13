// Observed msg_run__6Minion at 0x00233bd0, 4700 bytes: the per-frame routine of Minion.
// "object" is the argument; offsets are bytes. t is the resource block pointed to by the word at +36
// (its word +12 is a model header). R is the result of creatureAdvanceAnimation. W(x) is the word at
// +x; list heads are at +960 (a soul, a "node" whose word +12 is the soul object), +968 (the chosen
// enemy) and +964/+976/+980/+984 are plain words. h294 is the halfword at +294, h56 the halfword at
// +56. "SS(n)" is virtual slot 184 (table entry at word(+72) + 184: a signed halfword adjustment,
// then a function word at +4) called with the adjusted object and n; "V176(p)" is slot 176 called
// with (adjusted object, 0, p). The state is the halfword at +572. cutMode is a global byte.
//  1. newDamageHandler(object, t[12]); a nonzero everyFrame(object, t[12]) returns at once;
//     objectActivationCheck(object, 600) is called and its result is not used;
//     R = creatureAdvanceAnimation(object, 1.0f).
//  2. When cutMode is nonzero (re-read after each call):
//     - R bit 4: the attachment position is taken with modelGetCharAttachmentPos(t[12], copy of the
//       point at +16, h56, animation at +288, 0, &position, &other, single at +224); a 984-byte block
//       from blockAlloc, when not null, is built by the Soul constructor (position, h56, no names,
//       this object), linked as the object's soul with listOnewayLink(object+960, soul, soul), its
//       signed byte at +570 is saved and cleared, SS(22) runs on the soul, the byte is restored and bit
//       2 of the soul's word +12 is set.
//     - R bit 5 with a soul: clears bit 1 of the soul's word +12 and calls objectAlwaysDraw(soul).
//     - R bit 6 with a soul: clears its byte +570 around SS(18) on it.
//     - R bit 7 with a soul: clears its byte +570 around SS(17), then restores it, destroys the list
//       at +960 and zeroes W(+960).
//  3. slideBack(object, 0.2f); the table entry at +168 runs with the adjusted object. With a soul,
//     modelGetCharAttachmentPos gives a position that is copied to the soul's point at +16; the soul's
//     halfword +56 gets h56; objectUpdateInGrid(soul).
//  4. A state of 20 or more (halfword compared unsigned) returns; others select a case (2, 4, 7, 11
//     and 12 do nothing).
// Cases (z is enemyInZone2(object, 0.0, d, h56 + a, r, 0.3, 0), alt(d, r) is alternateEnemyInZone(
// object, 0.0, d, h56, r); link(x) is listOnewayLink(object+968, x, x); target is W(+576)):
//   0: target -> SS(3); otherwise W(+256) < W(+808) -> SS(1).
//   1: target -> SS(3); W(+256) < W(+808) -> SS(0); otherwise smartWander.
//   3 and 5 (they differ in the single constants and in the last step): with no target, 3 goes to SS(0)
//      and 5 to bExitStatePersueIdle -> SS(3). The enemy list at +968 is destroyed and zeroed. Then,
//      when W(+960) is zero: z(120, 32767) true -> link(W(target + 12)), else alt(120, 32767) ->
//      link(result); with an enemy chosen: z(120, 4000) -> SS(6); then for W(+972) not 17: z(120,
//      4000 at +11000) -> SS(17); not 18: -11000 -> SS(18); not 19: z(120, 6000 at -24000) -> SS(19);
//      then turnToFaceEnemy. With no enemy chosen: when W(+980) < W(+808), alt(120 / 350, 3000) ->
//      SS(16); then SS(14). When W(+960) is nonzero: z(170, 18000) / alt(170, 18000) as above; an
//      enemy -> SS(13); else W(+984) < W(+808): z(275 / 500, 2000) / alt(275 / 500, 2000); an enemy ->
//      SS(15). Then case 3 calls steerAndRouteToEnemy; case 5 calls bExitStatePersueIdle -> SS(3).
//   6, 17, 18, 19: move2 with zero; h294 bit 4 clear -> nothing else here. With R bit 0 the damage
//      call produceGeneralDamage(0.0, 120 / 170, ...) is made for the current state (6, 17, 18, 19
//      with different offset, range and flags) and W(+976) = 1; without a W(+976), an enemy (not in 17,
//      18, 19): V176(point of the enemy); in state 6 otherwise: enemyInZone(120, 4000) ->
//      turnToFaceEnemy. Then h294 bit 3 clear -> SS(3).
//   13, 15, 14, 16, 8, 9, 10: see the code.
// Inferred: this is a summoned minion; its soul is a linked object. Unknown: what the states mean.
// Nothing here ran on the EE.
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* header);
int everyFrame__8CreatureP12_modelHeader(void* self, void* header);
int objectActivationCheck__8Creaturei(void* self, int range);
unsigned creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
int enemyInZone__8Creaturefs(void* self, float range, short angle);
int enemyInZone2__8Creatureffssfi(void* self, float a, float b, short heading, short c, float d, int e);
void* alternateEnemyInZone__8Creatureffss(void* self, float a, float b, short c, short d);
int bExitStatePersueIdle__8Creature(void* self);
void smartWander__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
void move2__8CreatureG6Point3(void* self, const void* delta);
void produceGeneralDamage__8CreatureffssiiiiPQ24item11description(void* self, float a, float b, short heading, short c, int d, int e, int f,
                                                                  int flags, void* description);
float icos__Fi(int angle);
float isin__Fi(int angle);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* header, const void* point, short heading, void* animation, int index, void* position, void* other, float scale);
void* blockAlloc__Fi(int size);
void objectRemoveFromRunList__FP10GameObject(void* object);
void objectAlwaysDraw__FP10GameObject(void* object);
void objectUpdateInGrid__FP10GameObject(void* object);
void soundFadeStream__Fi(int value);
void listDestroy__FP8ListHead(void* list);
void listOnewayLink__FP8ListHeadT0Pv(void* list, void* first, void* second);
void soul_construct(void* self, const void* position, short heading, void* names, void* owner) __asm__("__4SoulG6Point3sPPcP6Minion");
void rain_of_fire_construct(void* self, const void* position, int a, float b, int c, void* owner, const void* velocity, void* header)
    __asm__("__15RainOfFireSpellG6Point3ifiP10GameObjectT1P15AnimationHeader");
extern float worldDifficulty;
extern unsigned skills[];
extern signed char cutMode;
}

namespace {
struct Point3 { float x, y, z; };
float bits(unsigned value) { return __builtin_bit_cast(float, value); }
unsigned& word(unsigned char* o, unsigned offset) { return *reinterpret_cast<unsigned*>(o + offset); }
int sword(unsigned char* o, unsigned offset) { return *reinterpret_cast<int*>(o + offset); }
float& real(unsigned char* o, unsigned offset) { return *reinterpret_cast<float*>(o + offset); }
int half(unsigned char* o, unsigned offset) { return *reinterpret_cast<short*>(o + offset); }
unsigned uhalf(unsigned char* o, unsigned offset) { return *reinterpret_cast<unsigned short*>(o + offset); }
unsigned char* pointer_at(unsigned char* o, unsigned offset) { return *reinterpret_cast<unsigned char**>(o + offset); }

void slot_call(unsigned char* o, unsigned table_offset, int n) {
    unsigned char* e = pointer_at(o, 72) + table_offset;
    reinterpret_cast<void (*)(void*, int)>(word(e, 4))(o + half(e, 0), n);
}
void set_state(unsigned char* o, int n) { slot_call(o, 184, n); }
// The soul and the enemy are list nodes whose word +12 is the object.
unsigned char* node_object(unsigned char* node) { return pointer_at(node, 12); }
void virtual_176(unsigned char* o, unsigned char* enemy_node) {
    Point3 position = *reinterpret_cast<const Point3*>(node_object(enemy_node) + 16);
    unsigned char* e = pointer_at(o, 72) + 176;
    reinterpret_cast<void (*)(void*, int, void*)>(word(e, 4))(o + half(e, 0), 0, &position);
}
void attachment(unsigned char* o, Point3* out, Point3* other) {
    const Point3 here = *reinterpret_cast<const Point3*>(o + 16);
    modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
        pointer_at(pointer_at(o, 36), 12), &here, static_cast<short>(half(o, 56)), o + 288, 0, out, other, real(o, 224));
}
bool zone2(unsigned char* o, float d, int offset, int range) {
    return enemyInZone2__8Creatureffssfi(o, 0.0f, d, static_cast<short>(uhalf(o, 56) + offset), static_cast<short>(range), bits(0x3e999999), 0) != 0;
}
void link_enemy(unsigned char* o, void* x) { listOnewayLink__FP8ListHeadT0Pv(o + 968, x, x); }
// Choose an enemy: the target's object first, else an alternate in the zone.
void pick(unsigned char* o, float a, float b, int range) {
    if (enemyInZone2__8Creatureffssfi(o, a, b, static_cast<short>(half(o, 56)), static_cast<short>(range), bits(0x3e999999), 0) != 0) {
        link_enemy(o, node_object(pointer_at(o, 576)));
    } else {
        void* r = alternateEnemyInZone__8Creatureffss(o, a, b, static_cast<short>(half(o, 56)), static_cast<short>(range));
        if (r != nullptr) link_enemy(o, r);
    }
}
void pick_range(unsigned char* o, float near_value, float far_value, int range) {
    pick(o, near_value, far_value, range);
}
bool probe_ring(unsigned char* o) {
    // z(120, 4000) -> 6; then the +11000, -11000 and -24000 probes keyed on W(+972)
    if (enemyInZone2__8Creatureffssfi(o, 0.0f, bits(0x42f00000), static_cast<short>(half(o, 56)), 4000, bits(0x3e999999), 0) != 0) {
        set_state(o, 6);
        return true;
    }
    if (word(o, 972) != 17u) {
        if (zone2(o, bits(0x42f00000), 11000, 7000)) {
            set_state(o, 17);
            return true;
        }
    }
    if (word(o, 972) != 18u) {
        if (zone2(o, bits(0x42f00000), -11000, 7000)) {
            set_state(o, 18);
            return true;
        }
    }
    if (word(o, 972) != 19u) {
        if (zone2(o, bits(0x42f00000), -24000, 6000)) {
            set_state(o, 19);
            return true;
        }
    }
    return false;
}

// Cases 3 and 5 share all of their steps; 'last' is the step that differs.
void chase(unsigned char* o, bool is_five) {
    if (word(o, 576) == 0u) {
        if (!is_five) {
            set_state(o, 0);
        } else if (bExitStatePersueIdle__8Creature(o) != 0) {
            set_state(o, 3);
        }
        return;
    }
    listDestroy__FP8ListHead(o + 968);
    word(o, 968) = 0u;
    if (word(o, 960) == 0u) {
        pick_range(o, 0.0f, bits(0x42f00000), 32767);
        if (word(o, 968) != 0u) {
            if (probe_ring(o)) return;
            turnToFaceEnemy__8Creature(o);
            return;
        }
        if (sword(o, 980) < sword(o, 808)) {
            if (alternateEnemyInZone__8Creatureffss(o, bits(0x42f00000), bits(0x43af0000), static_cast<short>(half(o, 56)), 3000) != nullptr) {
                set_state(o, 16);
                return;
            }
        }
        set_state(o, 14);
        return;
    }
    pick_range(o, 0.0f, bits(0x432a0000), 18000);
    if (word(o, 968) != 0u) {
        set_state(o, 13);
        return;
    }
    if (sword(o, 984) < sword(o, 808)) {
        pick_range(o, bits(0x43898000), bits(0x43fa0000), 2000);
        if (word(o, 968) != 0u) {
            set_state(o, 15);
            return;
        }
    }
    if (!is_five) {
        steerAndRouteToEnemy__8Creature(o);
        return;
    }
    if (bExitStatePersueIdle__8Creature(o) != 0) set_state(o, 3);
}

void tail_enemy(unsigned char* o) {
    if ((uhalf(o, 294) & 8u) == 0u) set_state(o, 3);
}
}

extern "C" void msg_run__6Minion(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    newDamageHandler__8CreatureP12_modelHeader(self, pointer_at(pointer_at(o, 36), 12));
    if (everyFrame__8CreatureP12_modelHeader(self, pointer_at(pointer_at(o, 36), 12)) != 0) return;
    objectActivationCheck__8Creaturei(self, 600);
    const unsigned R = creatureAdvanceAnimation__8Creaturef(self, bits(0x3f800000));
    if (cutMode != 0) {
        if ((R & 0x10u) != 0u) {
            Point3 position;
            Point3 other;
            attachment(o, &position, &other);
            unsigned char* soul = static_cast<unsigned char*>(blockAlloc__Fi(984));
            if (soul != nullptr) {
                soul_construct(soul, &position, static_cast<short>(half(o, 56)), nullptr, self);
                listOnewayLink__FP8ListHeadT0Pv(o + 960, soul, soul);
                const signed char saved = static_cast<signed char>(soul[570]);
                soul[570] = 0;
                set_state(soul, 22);
                soul[570] = static_cast<unsigned char>(saved);
                word(soul, 12) |= 2u;
            }
        }
        if (cutMode != 0) {
            if ((R & 0x20u) != 0u && word(o, 960) != 0u) {
                unsigned char* soul = node_object(pointer_at(o, 960));
                word(soul, 12) &= ~2u;
                objectAlwaysDraw__FP10GameObject(soul);
            }
            if (cutMode != 0) {
                if ((R & 0x40u) != 0u && word(o, 960) != 0u) {
                    unsigned char* soul = node_object(pointer_at(o, 960));
                    const signed char saved = static_cast<signed char>(soul[570]);
                    soul[570] = 0;
                    set_state(node_object(pointer_at(o, 960)), 18);
                    node_object(pointer_at(o, 960))[570] = static_cast<unsigned char>(saved);
                }
                if (cutMode != 0) {
                    if ((R & 0x80u) != 0u && word(o, 960) != 0u) {
                        unsigned char* soul = node_object(pointer_at(o, 960));
                        const signed char saved = static_cast<signed char>(soul[570]);
                        soul[570] = 0;
                        set_state(node_object(pointer_at(o, 960)), 17);
                        node_object(pointer_at(o, 960))[570] = static_cast<unsigned char>(saved);
                        listDestroy__FP8ListHead(o + 960);
                        word(o, 960) = 0u;
                    }
                }
            }
        }
    }
    slideBack__8Creaturef(self, bits(0x3e4ccccc));
    {
        unsigned char* e = pointer_at(o, 72) + 168;
        reinterpret_cast<void (*)(void*)>(word(e, 4))(o + half(e, 0));
    }
    if (word(o, 960) != 0u) {
        Point3 position;
        Point3 other;
        attachment(o, &position, &other);
        unsigned char* soul = node_object(pointer_at(o, 960));
        *reinterpret_cast<Point3*>(soul + 16) = position;
        unsigned char* again = node_object(pointer_at(o, 960));
        *reinterpret_cast<unsigned short*>(again + 56) = static_cast<unsigned short>(uhalf(o, 56));
        objectUpdateInGrid__FP10GameObject(again);
    }
    const int state = half(o, 572);
    if (static_cast<unsigned>(state) >= 20u) return;
    switch (state) {
    case 0:
        if (word(o, 576) != 0u) set_state(o, 3);
        else if (sword(o, 256) < sword(o, 808)) set_state(o, 1);
        return;
    case 1:
        if (word(o, 576) != 0u) set_state(o, 3);
        else if (sword(o, 256) < sword(o, 808)) set_state(o, 0);
        else smartWander__8Creature(self);
        return;
    case 3:
        chase(o, false);
        return;
    case 5:
        chase(o, true);
        return;
    case 6:
    case 17:
    case 18:
    case 19: {
        const float zero_vector[3] = {0.0f, 0.0f, 0.0f};
        move2__8CreatureG6Point3(self, zero_vector);
        if ((R & 1u) != 0u) {
            float range = bits(0x42f00000);
            int offset = 0;
            int distance = 4000;
            int flags = 1;
            switch (state) {
            case 6: break;
            case 17: offset = 11000; distance = 7000; flags = 0x80001; break;
            case 18: offset = -11000; distance = 7000; flags = 0x80001; break;
            default: offset = -24000; distance = 6000; flags = 0x80001; break;
            }
            produceGeneralDamage__8CreatureffssiiiiPQ24item11description(
                self, 0.0f, range, static_cast<short>(uhalf(o, 56) + offset), static_cast<short>(distance), half(o, 200), half(o, 202),
                half(o, 204), flags, nullptr);
            word(o, 976) = 1u;
        }
        if (word(o, 976) == 0u && word(o, 968) != 0u && state != 18 && state != 17 && state != 19) {
            virtual_176(o, pointer_at(o, 968));
        } else if (state == 6) {
            if (enemyInZone__8Creaturefs(self, bits(0x42f00000), 4000) != 0) turnToFaceEnemy__8Creature(self);
        }
        tail_enemy(o);
        return;
    }
    case 13: {
        const float zero_vector[3] = {0.0f, 0.0f, 0.0f};
        move2__8CreatureG6Point3(self, zero_vector);
        if ((R & 1u) != 0u) {
            produceGeneralDamage__8CreatureffssiiiiPQ24item11description(self, 0.0f, bits(0x432a0000), static_cast<short>(half(o, 56)), 18000,
                                                                          half(o, 200), half(o, 202), half(o, 204), 1, nullptr);
            word(o, 976) = 1u;
            listDestroy__FP8ListHead(o + 960);
            word(o, 960) = 0u;
        }
        if (word(o, 976) == 0u && word(o, 968) != 0u) virtual_176(o, pointer_at(o, 968));
        tail_enemy(o);
        return;
    }
    case 14: {
        const float zero_vector[3] = {0.0f, 0.0f, 0.0f};
        move2__8CreatureG6Point3(self, zero_vector);
        if ((uhalf(o, 294) & 0x10u) == 0u) set_state(o, 3);
        if ((R & 1u) != 0u) {
            Point3 position;
            Point3 other;
            attachment(o, &position, &other);
            unsigned char* soul = static_cast<unsigned char*>(blockAlloc__Fi(984));
            if (soul != nullptr) {
                soul_construct(soul, &position, static_cast<short>(half(o, 56)), nullptr, self);
                listOnewayLink__FP8ListHeadT0Pv(o + 960, soul, soul);
                set_state(soul, 16);
            }
        }
        return;
    }
    case 15: {
        const float zero_vector[3] = {0.0f, 0.0f, 0.0f};
        move2__8CreatureG6Point3(self, zero_vector);
        if ((R & 1u) != 0u) {
            word(o, 976) = 1u;
            if (word(o, 960) != 0u) {
                void* r = alternateEnemyInZone__8Creatureffss(self, bits(0x43898000), bits(0x43fa0000), static_cast<short>(half(o, 56)), 2000);
                if (r != nullptr) listOnewayLink__FP8ListHeadT0Pv(node_object(pointer_at(o, 960)) + 964, r, r);
                set_state(node_object(pointer_at(o, 960)), 17);
            }
            listDestroy__FP8ListHead(o + 960);
            word(o, 960) = 0u;
        }
        if (word(o, 976) == 0u && word(o, 968) != 0u) virtual_176(o, pointer_at(o, 968));
        tail_enemy(o);
        return;
    }
    case 16: {
        if ((R & 1u) != 0u) {
            unsigned char* skill = reinterpret_cast<unsigned char*>(skills[58]);
            int counts[4];
            for (int i = 0; i < 4; ++i) {
                skill = reinterpret_cast<unsigned char*>(skills[58]);
                unsigned char* e = pointer_at(skill, 96) + 104;
                counts[i] = reinterpret_cast<int (*)(void*, int, int, void*)>(word(e, 4))(skill + half(e, 0), i, static_cast<int>(worldDifficulty), self);
            }
            const int spread = counts[1] * 12;
            const int radius = counts[2] * 60;
            const int speed = static_cast<int>(static_cast<float>(spread) * bits(0x3fc00000));
            const float scale = static_cast<float>(counts[3]);
            Point3 velocity;
            float c = icos__Fi(half(o, 56)) * scale;
            velocity.x = c + c;
            float s = isin__Fi(half(o, 56)) * scale;
            velocity.y = s + s;
            velocity.z = 0.0f;
            void* made = blockAlloc__Fi(176);
            if (made != nullptr) {
                const float hundred = bits(0x42c80000);
                float x = real(o, 16) + icos__Fi(half(o, 56)) * hundred;
                float y = real(o, 20) + isin__Fi(half(o, 56)) * hundred;
                Point3 position;
                position.x = x;
                position.y = y;
                position.z = real(o, 24);
                rain_of_fire_construct(made, &position, radius, static_cast<float>(speed), counts[0], self, &velocity, pointer_at(pointer_at(o, 36), 188));
            }
        }
        if ((R & 2u) != 0u) word(o, 976) = 1u;
        if (word(o, 976) == 0u && word(o, 968) != 0u) virtual_176(o, pointer_at(o, 968));
        tail_enemy(o);
        return;
    }
    case 8:
        if ((uhalf(o, 294) & 0x10u) == 0u) set_state(o, 0);
        return;
    case 9:
        if (sword(o, 804) < sword(o, 808)) set_state(o, 3);
        return;
    case 10:
        if ((uhalf(o, 294) & 0x80u) != 0u) {
            soundFadeStream__Fi(0);
            objectRemoveFromRunList__FP10GameObject(self);
        }
        return;
    default:
        return;
    }
}
