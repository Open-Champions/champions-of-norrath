// Observed msg_run__8HateBeam at 0x001fcd70, 2668 bytes. O is the object, frameNm the global frame counter. Q = the word at
// O+76 (an object), W = the object at word Q+12. "entry k" is the entry at table byte k of the table whose address is the word at
// W+72 (or at O+72 for entry 8), called on that object adjusted by the entry's signed halfword. Steps in order:
//  1. Unless frameNm is below the signed word at O+172, or when Q is zero: entry 8 of O is called with 3 and that ends it.
//  2. A local list head (one word, zero) is filled by objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(copy of the
//     three floats at W+16, the signed halfword at W+56, 200.0, 120.0, the head, null). The list is walked from its first node
//     through the word at each node's +0; a node's creature is its word at +12; each creature C for which W's entry 160 (called with
//     C) is non-zero is stored into BOTH target slots tA and tB (the image writes the same C to both, so both end up the last
//     accepted creature; both start as zero).
//  3. M and N are two points taken from the model that W's entry 128 returns (called twice): modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f
//     (model, copy of W+16, the halfword at W+56, the animation state at W+288, index 0 or 1, out M or N, null, the float at
//     W+224). A third point (M - N) * 0.5 + N is computed and not used again; w = float((int)(isin__Fi(frameNm * 6000) * 6.0) + 120)
//     goes to the light at O+104: floats at +28 and +36 of the light become w, those at +32 and +40 become 1.0 / (w * w), then
//     lightUpdatePosition__FP5Light(O+104) runs.
//  4. Three bolts with handles in the words at O+80, O+84, O+88 start at M: when tA is non-zero the end point is tA's
//     position (floats at +16) with 0.5 * (its float at +32) added to z; for each handle that is non-zero
//     P_MoveLightning__FiG6Point3T1ii(handle, M, end, 94, -1) runs, else the handle is stored from
//     P_StartLightning__FG6Point3T0iiffUi(M, end, 550, 5, 7.9999, 11.9997, 0xff000fdc). When tA is zero the end point of each bolt is
//     W+16 plus (cos(a) * 200.0 * f1, sin(b) * 200.0 * f2, z) with a = (int)halfword at W+56 + 2 * (r % 11000) - 11000 for a fresh
//     roll r, f1 and f2 = float(roll) * 2^-16 for further rolls (cos, roll, sin, roll order, all eRand__10GameObject(O)); the image
//     reads an uninitialised stack float for the z of that point before adding W's z; the authored version uses 0.0 there. The z is then
//     replaced by worldFindHeight__FP12_worldHeaderG6Point3f(world, point, 16.0) and the bolt is moved (94, -1) or started
//     (550, 5, the same two floats, 0xff000fdc) as above.
//  5. The same for tB with N as the start and the handle words at O+92, O+96, O+100.
//  6. When tA is non-zero and checkIfSpellIsRunning__8CreatureRC9type_info(tA, __tf15SpellHateEffect()) is zero: a 48-byte block
//     from blockAlloc__Fi, when it succeeds, is constructed by the SpellHateEffect constructor (3, (int)worldDifficulty, tA, null,
//     skills word 30) and addSpell__8CreatureP5Spell(tA, block) runs (also with a null block). The same for tB.
//  7. listDestroy__FP8ListHead(head) runs.
// Inferred: a hate beam spell making lightning from the caster's attachment points to a target and applying a hate effect.
// Unknown: the meaning of the words at +172 and the handles' roles, and the unreachable z behaviour noted above. The image tests O
// for null before the entry 8 call; that test cannot be true here. Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
extern int frameNm;
extern void* world;
extern float worldDifficulty;
extern unsigned skills[];
unsigned eRand__10GameObject(void* self);
float icos__Fi(int angle);
float isin__Fi(int angle);
void objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(Point3 center, short heading, float a, float b, void* head, void* creature);
void listDestroy__FP8ListHead(void* head);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void* model, Point3 position, short heading, void* animation, int index,
                                                                                       Point3* first, Point3* second, float scale);
void lightUpdatePosition__FP5Light(void* light);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, Point3 position, float radius);
int P_StartLightning__FG6Point3T0iiffUi(Point3* from, Point3* to, int a, int b, float c, float d, unsigned color);
void P_MoveLightning__FiG6Point3T1ii(int handle, Point3* from, Point3* to, int a, int b);
const void* __tf15SpellHateEffect();
int checkIfSpellIsRunning__8CreatureRC9type_info(void* creature, const void* type);
void* blockAlloc__Fi(int size);
void SpellHateEffect_construct(void* self, int a, int b, void* creature, void* object, void* skill) __asm__("__15SpellHateEffectiiP8CreatureP10GameObjectP5Skill");
void addSpell__8CreatureP5Spell(void* creature, void* spell);

namespace {
using namespace w13smb;

// A bolt end point near W: used when there is no target creature.
void random_end(unsigned char* o, unsigned char* w, Point3& end) {
    const int heading = half(w, 56);
    int angle = heading + static_cast<int>((eRand__10GameObject(o) % 11000u) << 1) - 11000;
    float f20 = icos__Fi(angle) * 200.0f;
    f20 = f20 * (static_cast<float>(static_cast<int>(eRand__10GameObject(o))) * bits(0x37800000u));
    end.x = f20;
    angle = half(w, 56) + static_cast<int>((eRand__10GameObject(o) % 11000u) << 1) - 11000;
    f20 = isin__Fi(angle) * 200.0f;
    f20 = f20 * (static_cast<float>(static_cast<int>(eRand__10GameObject(o))) * bits(0x37800000u));
    end.y = f20;
    end.z = 0.0f;
    end.x = end.x + real(w, 16);
    end.y = end.y + real(w, 20);
    end.z = end.z + real(w, 24);
    end.z = worldFindHeight__FP12_worldHeaderG6Point3f(world, end, 16.0f);
}

// A bolt end point at a target: its position with half its float at +32 added to z.
void target_end(unsigned char* target, Point3& end) {
    Point3 lift;
    lift.x = 0.0f;
    lift.y = 0.0f;
    lift.z = real(target, 32) * 0.5f;
    end.x = real(target, 16) + lift.x;
    end.y = real(target, 20);
    end.z = real(target, 24) + lift.z;
}

void add_hate_effect(unsigned char* creature) {
    if (checkIfSpellIsRunning__8CreatureRC9type_info(creature, __tf15SpellHateEffect()) != 0) return;
    void* const block = blockAlloc__Fi(48);
    if (block != nullptr) SpellHateEffect_construct(block, 3, static_cast<int>(worldDifficulty), creature, nullptr, reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(skills[30])));
    addSpell__8CreatureP5Spell(creature, block);
}
}  // namespace

void msg_run__8HateBeam(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (!(frameNm < sword(o, 172)) || word(o, 76) == 0u) {
        call_entry(o, 8, 3);
        return;
    }
    unsigned char* const w = from_word(word(from_word(word(o, 76)), 12));
    unsigned head = 0;
    Point3 center = *reinterpret_cast<Point3*>(w + 16);
    unsigned char* target_a = nullptr;
    unsigned char* target_b = nullptr;
    objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(center, half(w, 56), 200.0f, 120.0f, &head, nullptr);
    for (unsigned node = head; node != 0u; node = word(from_word(node), 0)) {
        unsigned char* const creature = from_word(word(from_word(node), 12));
        if (call_entry(w, 160, to_word(creature)) != 0u) {
            target_b = creature;
            target_a = creature;
        }
    }
    Point3 m, n;
    {
        void* const model = from_word(call_entry(w, 128));
        Point3 at = *reinterpret_cast<Point3*>(w + 16);
        modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(model, at, half(w, 56), w + 288, 0, &m, nullptr, real(w, 224));
    }
    {
        void* const model = from_word(call_entry(w, 128));
        Point3 at = *reinterpret_cast<Point3*>(w + 16);
        modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(model, at, half(w, 56), w + 288, 1, &n, nullptr, real(w, 224));
    }
    {
        Point3 middle;
        middle.x = m.x - n.x;
        middle.y = m.y - n.y;
        middle.z = m.z - n.z;
        middle.x = middle.x * 0.5f;
        middle.y = middle.y * 0.5f;
        middle.z = middle.z * 0.5f;
        middle.x = middle.x + n.x;
        middle.y = middle.y + n.y;
        middle.z = middle.z + n.z;
        (void)middle;
    }
    const float width = static_cast<float>(static_cast<int>(isin__Fi(frameNm * 6000) * 6.0f) + 120);
    unsigned char* const light = o + 104;
    real(light, 28) = width;
    real(light, 36) = width;
    const float inverse = 1.0f / (width * width);
    real(light, 40) = inverse;
    real(light, 32) = inverse;
    lightUpdatePosition__FP5Light(light);
    // Bolts that start at M.
    for (int i = 0; i < 3; ++i) {
        unsigned& handle = word(o, 80 + 4 * i);
        Point3 end;
        if (target_a != nullptr) {
            if (handle != 0u) {
                target_end(target_a, end);
                P_MoveLightning__FiG6Point3T1ii(static_cast<int>(handle), &m, &end, 94, -1);
            } else {
                target_end(target_a, end);
                handle = static_cast<unsigned>(P_StartLightning__FG6Point3T0iiffUi(&m, &end, 550, 5, bits(0x40ffff27u), bits(0x413ffedbu), 0xff000fdcu));
            }
        } else {
            random_end(o, w, end);
            if (handle != 0u) P_MoveLightning__FiG6Point3T1ii(static_cast<int>(handle), &m, &end, 94, -1);
            else handle = static_cast<unsigned>(P_StartLightning__FG6Point3T0iiffUi(&m, &end, 550, 5, bits(0x40ffff27u), bits(0x413ffedbu), 0xff000fdcu));
        }
    }
    // Bolts that start at N.
    for (int i = 0; i < 3; ++i) {
        unsigned& handle = word(o, 92 + 4 * i);
        Point3 end;
        if (target_b != nullptr) {
            target_end(target_b, end);
            if (handle != 0u) P_MoveLightning__FiG6Point3T1ii(static_cast<int>(handle), &n, &end, 94, -1);
            else handle = static_cast<unsigned>(P_StartLightning__FG6Point3T0iiffUi(&n, &end, 550, 5, bits(0x40ffff27u), bits(0x413ffedbu), 0xff000fdcu));
        } else {
            random_end(o, w, end);
            if (handle != 0u) P_MoveLightning__FiG6Point3T1ii(static_cast<int>(handle), &n, &end, 94, -1);
            else handle = static_cast<unsigned>(P_StartLightning__FG6Point3T0iiffUi(&n, &end, 550, 5, bits(0x40ffff27u), bits(0x413ffedbu), 0xff000fdcu));
        }
    }
    if (target_a != nullptr) add_hate_effect(target_a);
    if (target_b != nullptr) add_hate_effect(target_b);
    listDestroy__FP8ListHead(&head);
}
}
