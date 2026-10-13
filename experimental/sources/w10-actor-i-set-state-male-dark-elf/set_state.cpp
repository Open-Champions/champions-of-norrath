// Observed setState__11MaleDarkElfi at 0x00288818, 3264 bytes.
// The state is copied to a local word whose address goes to setStateProcessing__8CreaturePi(object,
// &state); a nonzero result returns at once and nothing else happens. Otherwise bit 0x200 of the
// 64-bit word at object+160 is cleared and the local state selects an action (jump table of 17
// entries at 0x4776b0; 2, 4, 11, 12 and states of 17 or more do nothing but the final store). Terms:
// "block" is the resource block pointed to by the word at object+36 (re-read before each use), "anim"
// the animation state at object+288, v the byte at object+960 (taken unsigned or signed as noted),
// T(i, n) is animAddTransitionTo(anim, block[i], 0.1, n) and O(i, n) is animAddOneShot(anim,
// block[i], 0.1, 0.1, n); 0.1 is the single 0x3dcccccc. "Choice A" for v: 4 or 5 (unsigned) -> slot
// 76; else signed 3 -> 148, 2 -> 196, 1 -> 252, 0 -> 292; otherwise none. "Choice B" for v signed:
// 2 -> 180, 3 -> 132, 1 -> 252, 5 -> 52, 4 -> 44; otherwise none.
//   0: T(Choice A, 3); then the word at +256 becomes the word at +808 plus eRand % 60 plus 120.
//   1: resetRouting; a point from getSmartRandomDestination(object, 200) goes to +272..+283; then
//      v 2..4 (unsigned) T(220, 2051), else signed 1 T(268, 2051), signed 0 T(300, 2051); then
//      animGlobalSync(anim).
//   3: bit 0x08000000 of the word at +160 is cleared, the single 1000.0f is stored at +788 and
//      resetRouting runs; v 2 or 3: T(212, 2051); signed 1: T(260, 2051); signed 0: T(300, 2051);
//      v 4 or 5: T(84, 2051); then animGlobalSync.
//   5: T(Choice B, 3).
//   6: signed v 2: T(180, 3) then O(8 * (eRand % 3) + 156, 2056); 3: T(132, 3) then O(8 * (eRand %
//      3) + 108, 2056); 1: T(252, 3) then O(8 * (eRand & 1) + 228, 2056); 5: T(52, 3) and 4: T(44, 3),
//      each then O(8 * (eRand & 1) + 28, 2056). Then the word at +256 becomes eRand % 20.
//   7: resetRouting; the word at +256 becomes the word at +808 plus 120; T(212, 2051).
//   8: with the current state (halfword +572) 6 and bit 8 of the halfword at +294 set, or 9, the
//      local state becomes the current state (refused). Otherwise one O(index, 16) runs with the
//      index chosen from v (signed) as: 1 -> 244, 0 -> 284, any other value -> 204. The image also
//      has a group for bit 0x200 of the word at +160 being set (v 3 -> 140, 2 -> 188, otherwise
//      204), but it reads that word after step 1 cleared the bit, so it cannot run; it is kept
//      here as the same test.
//   9: T(Choice A, 3).
//  10: T(block[16 * (eRand & 3) + 308], 6) (0.1); dropTreasure(object, copy of the point at +16,
//      halfword +56, 0, 36.0, 2.0, 1.8, 0.6, 2.0, 0.25). When bit 2 of the word at +12 is clear, for
//      each of the words at +964, +968 and +1112 (k, in that order) that is nonzero: modelGet-
//      CharAttachmentPos(block[12], copy of the point, (short)(halfword +56 - 16384), anim, n,
//      &position, &other, 1.0f) with n = 0, 1, 2; then a 584-byte block from blockAlloc, when not
//      null, is built by the PhProp constructor with the position, block[0], block[2 * k], block[2 * k
//      + 2], 1, flags, null description and the single at +224 (flags 8193 for k 52 or 64 on the
//      first word, 16385 otherwise); the word is then set to zero whether or not the allocation
//      worked. In all cases virtual slot 192 (entry at table+192: adjustment halfword, function word
//      at +4) runs on the adjusted object.
//  13, 14, 15: T(76, 3) (state 15: T(292, 3)) then O(100 for 13 / 92 for 14, 2056) (state 15: O(276,
//      2056)); the word at +256 becomes eRand % 15.
//  16: T(Choice B, 3); bit 0x200 of the word at +160 is set and the word at +256 becomes the word at
//      +808 plus 120.
// Finally the local state is stored as the halfword at +572.
// Inferred: v is the variant byte chosen by initMiniboss__11MaleDarkElf; slot indexes name
// animations. Unknown: what the states mean. Nothing here ran on the EE.
extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* animation, void* header, float amount, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* animation, void* header, float first, float second, int flags);
void animGlobalSync__FP14AnimationState(void* animation);
unsigned eRand__10GameObject(void* self);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, const void* point, int heading, float a, float b, float c, float d, float e,
                                              float f, char* name);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* header, const void* point, short heading, void* animation, int index, void* position, void* other, float scale);
void* blockAlloc__Fi(int size);
void phprop_construct(void* self, const void* position, void* first, void* second, void* third, int count, int flags,
                      void* description, float scale) __asm__("__6PhPropG6Point3PcN22liPQ24item11descriptionf");
}

namespace {
struct Point3 { float x, y, z; };
float bits(unsigned value) { return __builtin_bit_cast(float, value); }
unsigned char* block_of(unsigned char* object) { return *reinterpret_cast<unsigned char**>(object + 36); }
void* slot(unsigned char* block, unsigned offset) { return *reinterpret_cast<void**>(block + offset); }

// Slot offsets for the two lookups on the variant byte; zero means no animation.
unsigned choice_a(unsigned char v) {
    if (static_cast<unsigned char>(v - 4) < 2u) return 76;
    switch (static_cast<signed char>(v)) {
    case 3: return 148;
    case 2: return 196;
    case 1: return 252;
    case 0: return 292;
    default: return 0;
    }
}
unsigned choice_b(signed char v) {
    switch (v) {
    case 2: return 180;
    case 3: return 132;
    case 1: return 252;
    case 5: return 52;
    case 4: return 44;
    default: return 0;
    }
}

struct Context {
    unsigned char* object;
    void* animation;
    float tenth;
    void transition(unsigned offset, int flags) const {
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, slot(block_of(object), offset), tenth, flags);
    }
    void one_shot(unsigned offset, int flags) const {
        animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(animation, slot(block_of(object), offset), tenth, tenth, flags);
    }
};

// One of the three attachment pieces at words +964, +968 and +1112 of the dying creature.
void release_piece(const Context& c, unsigned word_offset, int index, int flags_for_first) {
    unsigned char* object = c.object;
    const unsigned piece = *reinterpret_cast<unsigned*>(object + word_offset);
    if (piece == 0u) return;
    const Point3 position_in = *reinterpret_cast<const Point3*>(object + 16);
    Point3 position = *reinterpret_cast<const Point3*>(object + 16);
    Point3 other;
    modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
        slot(block_of(object), 12), &position_in, static_cast<short>(*reinterpret_cast<unsigned short*>(object + 56) - 16384),
        c.animation, index, &position, &other, 1.0f);
    int flags = flags_for_first;
    if (index == 0) {
        flags = (piece == 52u || piece == 64u) ? 8193 : 16385;
    }
    void* made = blockAlloc__Fi(584);
    if (made != nullptr) {
        unsigned char* block = block_of(object);
        unsigned char* entry = block + piece * 8;
        phprop_construct(made, &position, slot(block, 0), slot(entry, 0), slot(entry, 8), 1, flags, nullptr,
                         *reinterpret_cast<float*>(object + 224));
    }
    *reinterpret_cast<unsigned*>(object + word_offset) = 0u;
}
}

extern "C" void setState__11MaleDarkElfi(void* self, int requested) {
    unsigned char* object = static_cast<unsigned char*>(self);
    int state = requested;
    if (setStateProcessing__8CreaturePi(self, &state) != 0) return;
    unsigned long long& flags = *reinterpret_cast<unsigned long long*>(object + 160);
    flags &= ~0x200ull;
    const Context c = {object, object + 288, bits(0x3dcccccc)};
    const unsigned char variant = object[960];
    const signed char signed_variant = static_cast<signed char>(variant);
    unsigned pick;
    switch (static_cast<unsigned>(state)) {
    case 0:
    case 9: {
        const unsigned offset = choice_a(variant);
        if (offset != 0) c.transition(offset, 3);
        if (state == 0) {
            *reinterpret_cast<int*>(object + 256) = static_cast<int>(*reinterpret_cast<unsigned*>(object + 808) + eRand__10GameObject(self) % 60u + 120u);
        }
        break;
    }
    case 1: {
        resetRouting__8Creature(self);
        Point3 destination;
        getSmartRandomDestination__8Creaturei(&destination, self, 200);
        *reinterpret_cast<Point3*>(object + 272) = destination;
        if (static_cast<unsigned char>(variant - 2) < 3u) c.transition(220, 2051);
        else if (signed_variant == 1) c.transition(268, 2051);
        else if (signed_variant == 0) c.transition(300, 2051);
        animGlobalSync__FP14AnimationState(c.animation);
        break;
    }
    case 3:
        flags &= 0xfffffffff7ffffffull;
        *reinterpret_cast<float*>(object + 788) = bits(0x447a0000);
        resetRouting__8Creature(self);
        if (static_cast<unsigned char>(variant - 2) < 2u) c.transition(212, 2051);
        else if (signed_variant == 1) c.transition(260, 2051);
        else if (signed_variant == 0) c.transition(300, 2051);
        else if (static_cast<unsigned char>(variant - 4) < 2u) c.transition(84, 2051);
        animGlobalSync__FP14AnimationState(c.animation);
        break;
    case 5: {
        const unsigned offset = choice_b(signed_variant);
        if (offset != 0) c.transition(offset, 3);
        break;
    }
    case 6:
        if (signed_variant == 2) {
            c.transition(180, 3);
            pick = eRand__10GameObject(self) % 3u;
            c.one_shot(pick * 8 + 156, 2056);
        } else if (signed_variant == 3) {
            c.transition(132, 3);
            pick = eRand__10GameObject(self) % 3u;
            c.one_shot((pick & 0xffffu) * 8 + 108, 2056);
        } else if (signed_variant == 1) {
            c.transition(252, 3);
            pick = eRand__10GameObject(self) & 1u;
            c.one_shot(pick * 8 + 228, 2056);
        } else if (signed_variant == 5) {
            c.transition(52, 3);
            pick = eRand__10GameObject(self) & 1u;
            c.one_shot(pick * 8 + 28, 2056);
        } else if (signed_variant == 4) {
            c.transition(44, 3);
            pick = eRand__10GameObject(self) & 1u;
            c.one_shot(pick * 8 + 28, 2056);
        }
        *reinterpret_cast<int*>(object + 256) = static_cast<int>(eRand__10GameObject(self) % 20u);
        break;
    case 7:
        resetRouting__8Creature(self);
        *reinterpret_cast<int*>(object + 256) = static_cast<int>(*reinterpret_cast<unsigned*>(object + 808) + 120u);
        c.transition(212, 2051);
        break;
    case 8: {
        const int current = *reinterpret_cast<short*>(object + 572);
        if ((current == 6 && (*reinterpret_cast<unsigned short*>(object + 294) & 8u) != 0u) || current == 9) {
            state = current;
        } else if ((flags & 0x200ull) != 0ull) {
            c.one_shot(signed_variant == 3 ? 140 : signed_variant == 2 ? 188 : 204, 16);
        } else {
            c.one_shot(signed_variant == 1 ? 244 : signed_variant == 0 ? 284 : 204, 16);
        }
        break;
    }
    case 10: {
        pick = eRand__10GameObject(self) & 3u;
        c.transition(pick * 16 + 308, 6);
        const Point3 position = *reinterpret_cast<const Point3*>(object + 16);
        dropTreasure__8CreatureG6Point3iffffffPc(self, &position, *reinterpret_cast<short*>(object + 56), bits(0x42100000), 2.0f,
                                                 bits(0x3fe66666), bits(0x3f199999), 2.0f, 0.25f, nullptr);
        if ((*reinterpret_cast<unsigned*>(object + 12) & 2u) == 0u) {
            release_piece(c, 964, 0, 0);
            release_piece(c, 968, 1, 16385);
            release_piece(c, 1112, 2, 16385);
        }
        unsigned char* entry = *reinterpret_cast<unsigned char**>(object + 72) + 192;
        reinterpret_cast<void (*)(void*)>(*reinterpret_cast<unsigned*>(entry + 4))(object + *reinterpret_cast<short*>(entry));
        break;
    }
    case 13:
    case 14:
    case 15:
        if (state == 15) {
            c.transition(292, 3);
            c.one_shot(276, 2056);
        } else {
            c.transition(76, 3);
            c.one_shot(state == 13 ? 100 : 92, 2056);
        }
        *reinterpret_cast<int*>(object + 256) = static_cast<int>(eRand__10GameObject(self) % 15u);
        break;
    case 16: {
        const unsigned offset = choice_b(signed_variant);
        if (offset != 0) c.transition(offset, 3);
        flags |= 0x200ull;
        *reinterpret_cast<int*>(object + 256) = static_cast<int>(*reinterpret_cast<unsigned*>(object + 808) + 120u);
        break;
    }
    default:
        break;
    }
    *reinterpret_cast<short*>(object + 572) = static_cast<short>(state);
}
