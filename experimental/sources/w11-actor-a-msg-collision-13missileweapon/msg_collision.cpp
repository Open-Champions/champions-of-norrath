// Observed msg_collision__13MissileWeaponP10GameObject at 0x00146d58, 1312 bytes. Arguments: the missile o and
// the object it touched, T (may be null). Nothing is returned. Names: Q = the word at o+76 and, when Q is
// nonzero, W = the word at Q+12 (an object; the owner); "flags" of an object = its word at +12; the state byte
// is the signed byte at o+204; kind = the word at o+208. A run-time type is read the way the image does it:
// the object's table (word at +72) holds at offset 4 a function that is called with no argument and returns
// the type-info pointer, compared with __tf...() through __eq__C9type_infoRC9type_info.
//  1. Nothing happens when the state byte is nonzero.
//  2. When T is non-null and its flags lack 0x20: if its type is 13AutoContainer, the table entry at offset 16
//     is called on T with (0, 0.0, 0); the state byte becomes 2 either way and the function returns.
//  3. When T's flags have 0x20 and W is non-null with flags 0x20: the entry at offset 160 of W's table is
//     called on W; a zero result returns.
//  4. When T's flags (0 for a null T) have 0x20, W is non-null and W's flags have 0x40: return if T's flags
//     have 0x40 or 0x01000000. Otherwise, when kind is 0: listOnewayLink(o+292, T, T); the entry at offset
//     136 of o's table is called on o with T and its result is stored at o+244. A zero result calls
//     listDestroy(o+292). A non-zero one: unless T's type is 8Skeleton (then listDestroy(o+292) too), the
//     entry at offset 128 of T's table gives a model; when it is non-null, modelGetShadowPos(model, T+288,
//     T's position, T's halfword at +56, the word at o+244, &point, null, &R, T's float at +224) fills a 3x3
//     matrix R, then R is transposed in place, an identity matrix S is rotated about axis 2 by the halfword at
//     o+56, and o+256 = S times R (matMul(o+256, S, R)). Bit 0x1 of o's flags is then set. For every kind the
//     step ends with a table: kind 1 stores 32 at o+232, kind 5 stores 1024, kind 6 stores 128 (other kinds
//     store nothing there), and the same three kinds store 10000 at o+228.
//  5. Nothing more for a null T. Otherwise a block chance c (an int percent) is found: when T's flags have
//     0x40 (a player), c = (int)(getDeflectionChance(skills[2], playerGetSkillRank(word at T+960, 2)) *
//     100.0); else when the flags have 0x20 and the word at T+216 is nonzero, c = the result of the table entry
//     at offset 96 of the table whose address is the word at skills[2]+96, called on skills[2] with (0, (int)
//     worldDifficulty); otherwise c = 0. syncRandState = syncRandState * 0x19660d + 0x3c6ef35f and the roll is
//     ((syncRandState >> 16) mod 100) + 1.
//  6. When the roll < c: playerBlockSparks(T, 0) runs for a player T; when the words at o+444 and o+448 are
//     both nonzero, a 584-byte block is allocated and, if it succeeded, the PhProp constructor (model/texture
//     form) runs on it with o's position, those two words, 1, 42, a null description and 1.0; then
//     objectAddToDeleteList(o) and return.
//  7. Otherwise: when T's flags have 0x20, r = gameAttackResolver(T, W or o when W is null, (float)word o+212,
//     (float)word o+216, word o+228, word o+220, word o+224, word o+232, word o+296, false). r != 1 sets the
//     state byte to 2; r == 1 sets it to 3 for a player T and to 1 for any other T. When T's flags lack 0x20,
//     the state byte becomes 2 if T's type is 15PushPhysicsProp (and nothing happens otherwise).
// Inferred: this handles a projectile hit. Unknown: what the flag bits 0x20, 0x40, 0x01000000 and the states
// are, the roles of o+76, o+212..232, and what the table entries at 128, 136, 160 and 16 do. A null T makes the
// image read the word at address 12 before it notices; that read is not reproduced. Nothing here ran on the EE.
#include "../w11-actor-a-common/access.hpp"

extern "C" {
extern unsigned char skills[];
extern unsigned syncRandState;
extern float worldDifficulty;
void* __tf13AutoContainer();
void* __tf8Skeleton();
void* __tf15PushPhysicsProp();
int __eq__C9type_infoRC9type_info(const void* left, const void* right);
void __throw_bad_typeid();
void* listOnewayLink__FP8ListHeadT0Pv(void* a, void* b, void* v);
void listDestroy__FP8ListHead(void* head);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* animation, Point3 position, short heading, int index, Point3* outPoint, float* outValue, void* matrix, float scale);
void matTranspose__FP8Matrix33T0(void* a, void* b);
void matIdent__FP8Matrix33(void* matrix);
void matRot__FP8Matrix33is(void* matrix, int axis, short angle);
void matMul__FP8Matrix33N20(void* destination, void* a, void* b);
int playerGetSkillRank__Fii(int owner, int skill);
float getDeflectionChance__10SkillDodgei(void* dodge, int rank);
void playerBlockSparks__6Playeri(void* player, int a);
void* blockAlloc__Fi(int size);
void __6PhPropG6Point3P12_modelHeaderP8_textureliPQ24item11descriptionf(void* self, Point3 position, void* model, void* texture, long n, int flags,
                                                                          void* description, float scale);
void objectAddToDeleteList__FP10GameObject(void* object);
int gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void* b, float x, float y, int p, int q, int r, int s, void* description,
                                                                      bool flag);
}

namespace {
void* type_of(unsigned char* object) {
    using namespace w11a;
    const unsigned function = *reinterpret_cast<unsigned*>(table_entry(object, 0) + 4);
#if defined(W11_ACTOR_A_HOST)
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(w11_actor_a_host_call(function, nullptr, 0, 0, 0)));
#else
    return reinterpret_cast<void* (*)()>(static_cast<__UINTPTR_TYPE__>(function))();
#endif
}

void call_container_entry(unsigned char* object) {
    using namespace w11a;
    unsigned char* const entry = table_entry(object, 16);
    unsigned char* const adjusted = object + *reinterpret_cast<short*>(entry);
    const unsigned function = *reinterpret_cast<unsigned*>(entry + 4);
#if defined(W11_ACTOR_A_HOST)
    w11_actor_a_host_call(function, adjusted, 2, 0, 0);
#else
    reinterpret_cast<void (*)(void*, int, float, int)>(static_cast<__UINTPTR_TYPE__>(function))(adjusted, 0, 0.0f, 0);
#endif
}
}  // namespace

extern "C" void msg_collision__13MissileWeaponP10GameObject(void* self, void* other) {
    using namespace w11a;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const t = static_cast<unsigned char*>(other);
    if (*reinterpret_cast<signed char*>(o + 204) != 0) return;
    unsigned char* owner = nullptr;
    if (word(o, 76) != 0) owner = from_word(word(from_word(word(o, 76)), 12));
    unsigned flags = 0;
    if (t != nullptr) {
        flags = word(t, 12);
        if (!(flags & 0x20u)) {
            if (__eq__C9type_infoRC9type_info(type_of(t), __tf13AutoContainer()) != 0) call_container_entry(t);
            *reinterpret_cast<signed char*>(o + 204) = 2;
            return;
        }
        if (owner != nullptr && (word(owner, 12) & 0x20u)) {
            if (call_entry(owner, 160) == 0) return;
            flags = word(t, 12);
        }
    }
    if ((flags & 0x20u) && owner != nullptr && (word(owner, 12) & 0x40u)) {
        if (flags & 0x40u) return;
        if (flags & 0x01000000u) return;
        if (word(o, 208) == 0) {
            listOnewayLink__FP8ListHeadT0Pv(o + 292, t, t);
            const unsigned result = call_entry(o, 136, reinterpret_cast<__UINTPTR_TYPE__>(t));
            word(o, 244) = result;
            if (result == 0) {
                listDestroy__FP8ListHead(o + 292);
            } else {
                if (t == nullptr) __throw_bad_typeid();
                if (__eq__C9type_infoRC9type_info(type_of(t), __tf8Skeleton()) == 1) {
                    listDestroy__FP8ListHead(o + 292);
                } else {
                    void* const model = reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(call_entry(t, 128)));
                    if (model != nullptr) {
                        Point3 point;
                        float rotation[9];
                        float turn[9];
                        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
                            model, t + 288, *reinterpret_cast<Point3*>(t + 16), half(t, 56), sword(o, 244), &point, nullptr, rotation, real(t, 224));
                        matTranspose__FP8Matrix33T0(rotation, rotation);
                        matIdent__FP8Matrix33(turn);
                        matRot__FP8Matrix33is(turn, 2, half(o, 56));
                        matMul__FP8Matrix33N20(o + 256, turn, rotation);
                    }
                }
            }
            word(o, 12) |= 1u;
        }
        const int kind = sword(o, 208);
        int stored = 0;
        if (kind == 1) stored = 32;
        else if (kind == 5) stored = 1024;
        else if (kind == 6) stored = 128;
        if (stored != 0) {
            word(o, 232) = static_cast<unsigned>(stored);
            word(o, 228) = 10000;
        }
    }
    if (t == nullptr) return;
    flags = word(t, 12);
    unsigned char* player = nullptr;
    int chance = 0;
    if (flags & 0x40u) {
        player = t;
        const int rank = playerGetSkillRank__Fii(sword(t, 960), 2);
        chance = static_cast<int>(getDeflectionChance__10SkillDodgei(from_word(word(skills, 8)), rank) * bits(0x42c80000u));
    } else if ((flags & 0x20u) && word(t, 216) != 0) {
        chance = static_cast<int>(call_entry_from(from_word(word(skills, 8)), 96, 96, 0, static_cast<unsigned long>(static_cast<int>(worldDifficulty))));
    }
    syncRandState = syncRandState * 0x19660du + 0x3c6ef35fu;
    const int roll = static_cast<int>((syncRandState >> 16) % 100u) + 1;
    if (roll < chance) {
        if (word(t, 12) & 0x40u) playerBlockSparks__6Playeri(player, 0);
        if (word(o, 444) != 0 && word(o, 448) != 0) {
            void* const block = blockAlloc__Fi(584);
            if (block != nullptr)
                __6PhPropG6Point3P12_modelHeaderP8_textureliPQ24item11descriptionf(
                    block, *reinterpret_cast<Point3*>(o + 16), from_word(word(o, 444)), from_word(word(o, 448)), 1, 42, nullptr, 1.0f);
        }
        objectAddToDeleteList__FP10GameObject(o);
        return;
    }
    if (word(t, 12) & 0x20u) {
        const int result = gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(
            t, owner != nullptr ? static_cast<void*>(owner) : static_cast<void*>(o), static_cast<float>(sword(o, 212)), static_cast<float>(sword(o, 216)),
            sword(o, 228), sword(o, 220), sword(o, 224), sword(o, 232), from_word(word(o, 296)), false);
        if (result != 1) *reinterpret_cast<signed char*>(o + 204) = 2;
        else if (word(t, 12) & 0x40u) *reinterpret_cast<signed char*>(o + 204) = 3;
        else *reinterpret_cast<signed char*>(o + 204) = 1;
    } else if (__eq__C9type_infoRC9type_info(type_of(t), __tf15PushPhysicsProp()) != 0) {
        *reinterpret_cast<signed char*>(o + 204) = 2;
    }
}
