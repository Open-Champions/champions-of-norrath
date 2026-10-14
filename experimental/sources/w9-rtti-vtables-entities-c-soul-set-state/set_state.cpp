#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed setState__4Souli at 0x1f46c8, 1672 bytes, emitted here as
// norrath_soul_set_state (the name the Soul constructor imports). The animation
// state is at object+288, the word at +36 points to a header table T, and the
// requested state is kept in a stack word.
//  1. When strcasecmp(levelName + 0x51, "airhub") is 0 the state becomes 24.
//  2. setStateProcessing__8CreaturePi(object, &state); a nonzero result returns.
//  3. With a nonzero word at +960: a state outside 16..22 becomes the signed
//     halfword at +572 and the call returns without storing it. A current
//     halfword of 17 also returns at once, without storing. A state of 25 or
//     more only stores.
//  4. Jump table at 0x46e2a0. Rates are 0.3 (0x3e999999) or 0.1 (0x3dcccccc);
//     "t" is animAddTransitionTo, "s" animAddOneShot (both rates equal):
//     0: eRand mod 120 plus +808 plus 60 -> +256; t T+60, 0.3, flags 3.
//     1: resetRouting; getSmartRandomDestination(range 200) copied to +272;
//        t T+76, 0.3, 2051; animGlobalSync; +256 = +808 + eRand mod 120 + 300.
//     5 and 13: t T+52, 0.3, 3.
//     8: virtual slot at byte offset 184 of the table at +72, argument 15 when
//        the current halfword is 14, else 14.
//     9 and the quiet states: the state becomes the current signed halfword
//        for 9; 2, 3, 4, 6, 7, 11 and 12 just store.
//     10: t T+140, 0.1, flags 6; +256 = +808 + 180; word +12 |= 0x4000; virtual
//        slot at byte offset 192.
//     14: t T+68, 0.3, 3; +256 = +808 + eRand mod 120.
//     15: when the current halfword is 14, s T+44, 0.1, flags 16; otherwise the
//        state becomes the current halfword.
//     16: s T+84, 0.1, 8.   17: listDestroy__FP8ListHead(object+960), the word
//        at +960 becomes 0, +256 = +808 + 240.   18: s T+116, 0.1, 12.
//     19: s T+92, 0.5, 8.   20: t T+108, 0.1, 3.   21: t T+124, 0.3, 3.
//     22: s T+132, 0.1, 8.
//     24: t T+148, 0.1, flags 6; +256 = +808 + 300; word +12 |= 9.
//     23: for each of the nm shadow points of the model (virtual slot at byte
//        offset 128): two soulParticle particles with a random unit-ball
//        velocity (x, y, z each 2*roll*2^-16 - 1, rejected while the squared
//        length exceeds 1, z then forced to 0, scaled by 15.0), started at the
//        point modelGetShadowPos computes (moved through the minion's attach
//        matrices when the word at +960 is nonzero); then word +12 |= 0x4002
//        and objectAddToDeleteList.
//  5. Unless a path above returned early, the low 16 bits of the state word go
//     to +572.
// Inferred: Soul is a spirit effect object. Unknown: the state names, the
// layout of the table T and the matrix and particle routines' semantics.
extern "C" {
extern char levelName[];
extern unsigned char soulParticle[];
int strcasecmp(const char* left, const char* right);
int setStateProcessing__8CreaturePi(void* self, int* state);
int eRand__10GameObject(void* object);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void animGlobalSync__FP14AnimationState(void* state);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
void listDestroy__FP8ListHead(void* list);
void objectAddToDeleteList__FP10GameObject(void* object);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* animation, const void* point, short a, int index, void* out_point, float* out_value, void* matrix, float scale);
void getSoulAttachMatrix__6MinionP8Matrix34(void* minion, void* matrix);
void modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(void* model, void* animation, int a, void* matrix, float scale);
void matInvert__FP8Matrix34T0(void* a, void* b);
void matMul__FP8Matrix34N20(void* a, void* b, void* c);
void matTransform__FP6Point3P8Matrix34T0(void* a, void* b, void* c);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, void* position, void* velocity, float value);
}

namespace {
using namespace rtti_c;
__attribute__((always_inline)) inline void* header(Address object, unsigned offset) {
    return pointer(read_word(address(pointer(read_word(object + 36u))) + offset));
}
__attribute__((always_inline)) inline void transition(Address object, unsigned offset, unsigned rate_bits, int flags) {
    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(bytes(object + 288u), header(object, offset), single_from_bits(rate_bits), flags);
}
__attribute__((always_inline)) inline void one_shot(Address object, unsigned offset, unsigned rate_bits, int flags) {
    const float rate = single_from_bits(rate_bits);
    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(bytes(object + 288u), header(object, offset), rate, rate, flags);
}
__attribute__((always_inline)) inline void* model_of(void* self) {
    typedef void* (*SlotCall)(void*);
    void* adjusted;
    SlotCall call = slot_function<SlotCall>(self, 128u, &adjusted);
    return call(adjusted);
}
__attribute__((always_inline)) inline float unit_component(void* self) {
    const int roll = eRand__10GameObject(self);
    const float half = static_cast<float>(roll) * single_from_bits(0x37800000u);
    return (half + half) - single_from_bits(0x3f800000u);
}
void shadow_particles(void* self, Address object) {
    const int count = modelGetNmShadowPoints__FP12_modelHeader(model_of(self));
    if (count > 0) {
        void* const animation = bytes(object + 288u);
        float position[3], other[1], origin[3], matrix_a[12], matrix_b[12], velocity[3], sample[3];
        for (int index = 0; index < count; ++index) {
            origin[0] = origin[1] = origin[2] = 0.0f;
            modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
                model_of(self), animation, origin, static_cast<short>(-16384), index, position, other, nullptr, read_single(object + 224u));
            const unsigned minion = read_word(object + 960u);
            if (minion != 0u) {
                getSoulAttachMatrix__6MinionP8Matrix34(pointer(read_word(address(pointer(minion)) + 12u)), matrix_a);
                modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(
                    pointer(read_word(address(pointer(read_word(object + 36u))) + 28u)), animation, 0, matrix_b, single_from_bits(0x3f800000u));
                matInvert__FP8Matrix34T0(matrix_b, matrix_b);
                matMul__FP8Matrix34N20(matrix_a, matrix_b, matrix_a);
                matTransform__FP6Point3P8Matrix34T0(position, matrix_a, position);
            }
            for (int shot = 0; shot < 2; ++shot) {
                do {
                    sample[0] = unit_component(self);
                    sample[1] = unit_component(self);
                    sample[2] = unit_component(self);
                } while (single_from_bits(0x3f800000u) < sample[0] * sample[0] + sample[1] * sample[1] + sample[2] * sample[2]);
                velocity[0] = sample[0];
                velocity[1] = sample[1];
                velocity[2] = 0.0f;
                velocity[0] = velocity[0] * single_from_bits(0x41700000u);
                velocity[1] = velocity[1] * single_from_bits(0x41700000u);
                velocity[2] = velocity[2] * single_from_bits(0x41700000u);
                P_AddParticle__FP11ParticleDefP6Point3T1f(soulParticle, position, velocity, 0.0f);
            }
        }
    }
    write_word(object + 12u, read_word(object + 12u) | 0x4002u);
    objectAddToDeleteList__FP10GameObject(self);
}
}

extern "C" void norrath_soul_set_state(void* self, int requested) {
    const Address object = address(self);
    int state = requested;
    if (strcasecmp(levelName + 0x51, "airhub") == 0) state = 24;
    if (setStateProcessing__8CreaturePi(self, &state) != 0) return;
    if (read_word(object + 960u) != 0u && static_cast<unsigned>(state) - 16u >= 7u) return;
    const int current = read_i16(object + 572u);
    if (current == 17) return;
    switch (static_cast<unsigned>(state)) {
    case 0u: {
        const unsigned roll = static_cast<unsigned>(eRand__10GameObject(self)) % 120u;
        write_word(object + 256u, read_word(object + 808u) + roll + 60u);
        transition(object, 60u, 0x3e999999u, 3);
        break;
    }
    case 1u: {
        resetRouting__8Creature(self);
        unsigned char point[12];
        getSmartRandomDestination__8Creaturei(point, self, 200);
        for (unsigned i = 0; i < 12u; ++i) write_u8(object + 272u + i, point[i]);
        transition(object, 76u, 0x3e999999u, 2051);
        animGlobalSync__FP14AnimationState(bytes(object + 288u));
        const unsigned roll = static_cast<unsigned>(eRand__10GameObject(self)) % 120u;
        write_word(object + 256u, read_word(object + 808u) + roll + 300u);
        break;
    }
    case 5u:
    case 13u:
        transition(object, 52u, 0x3e999999u, 3);
        break;
    case 8u: {
        typedef void (*SlotCall)(void*, int);
        void* adjusted;
        SlotCall call = slot_function<SlotCall>(self, 184u, &adjusted);
        call(adjusted, current == 14 ? 15 : 14);
        break;
    }
    case 9u:
        state = current;
        break;
    case 10u: {
        transition(object, 140u, 0x3dccccccu, 6);
        write_word(object + 256u, read_word(object + 808u) + 180u);
        write_word(object + 12u, read_word(object + 12u) | 0x4000u);
        typedef void (*SlotCall)(void*);
        void* adjusted;
        SlotCall call = slot_function<SlotCall>(self, 192u, &adjusted);
        call(adjusted);
        break;
    }
    case 14u: {
        transition(object, 68u, 0x3e999999u, 3);
        const unsigned roll = static_cast<unsigned>(eRand__10GameObject(self)) % 120u;
        write_word(object + 256u, read_word(object + 808u) + roll);
        break;
    }
    case 15u:
        if (current == 14) one_shot(object, 44u, 0x3dccccccu, 16);
        else state = current;
        break;
    case 16u:
        one_shot(object, 84u, 0x3dccccccu, 8);
        break;
    case 17u:
        listDestroy__FP8ListHead(bytes(object + 960u));
        write_word(object + 960u, 0u);
        write_word(object + 256u, read_word(object + 808u) + 240u);
        break;
    case 18u:
        one_shot(object, 116u, 0x3dccccccu, 12);
        break;
    case 19u:
        one_shot(object, 92u, 0x3f000000u, 8);
        break;
    case 20u:
        transition(object, 108u, 0x3dccccccu, 3);
        break;
    case 21u:
        transition(object, 124u, 0x3e999999u, 3);
        break;
    case 22u:
        one_shot(object, 132u, 0x3dccccccu, 8);
        break;
    case 23u:
        shadow_particles(self, object);
        break;
    case 24u:
        transition(object, 148u, 0x3dccccccu, 6);
        write_word(object + 256u, read_word(object + 808u) + 300u);
        write_word(object + 12u, read_word(object + 12u) | 9u);
        break;
    default:
        break;
    }
    write_u16(object + 572u, static_cast<unsigned>(state));
}
