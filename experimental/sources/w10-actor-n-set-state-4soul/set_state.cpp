// Observed setState__4Souli at 0x001f46c8, 1672 bytes. R is the block at the word object+36, "anim"
// the animation state at +288, s the halfword at +572 (the current state), t the word at +808 and
// e = eRand(this). "transition(h, time, flags)" is animAddTransitionTo(anim, R[h], time, flags),
// "oneShot(h, a, b, flags)" animAddOneShot(anim, R[h], a, b, flags); "setState(n)" is the table
// entry at (word +72)+184 called with n on the adjusted object.
// Start: n = argument; when strcasecmp(levelName + 81, "airhub") is 0, n becomes 24. A copy of n
// goes to setStateProcessing__8CreaturePi (which may change it); a non-zero result ends the call
// with nothing else done. Then the call ends without storing anything when
//  - the word at +960 is non-zero and n is not in 16..22, or
//  - s is 17.
// Otherwise n selects a case (n >= 25 and 2, 3, 4, 6, 7, 11, 12 do nothing more) and the low
// halfword of n is stored at +572 at the end.
//  0: word +256 = t + e % 120 + 60; transition(60, 0.3, 3).
//  1: resetRouting; getSmartRandomDestination(point, this, 200) is copied to +272..283;
//     transition(76, 0.3, 0x803); animGlobalSync(anim); word +256 = t + e % 120 + 300.
//  5, 13: transition(52, 0.3, 3).
//  8: setState(15) when s is 14, otherwise setState(14).
//  9: nothing (n becomes s).
//  10: transition(140, 0.1, 6); word +256 = t + 180; word +12 |= 0x4000; the table entry at +192
//      is called.
//  14: transition(68, 0.3, 3); word +256 = t + e % 120.
//  15: when s is 14: oneShot(44, 0.1, 0.1, 16); otherwise n becomes s.
//  16: oneShot(84, 0.1, 0.1, 8).
//  17: listDestroy on the member at +960; word +960 = 0; word +256 = t + 240.
//  18: oneShot(116, 0.1, 0.1, 12).
//  19: oneShot(92, 0.5, 0.5, 8).
//  20: transition(108, 0.1, 3).
//  21: transition(124, 0.3, 3).
//  22: oneShot(132, 0.1, 0.1, 8).
//  23: with m the table entry at +128 called with no arguments (it yields a model) and c =
//      modelGetNmShadowPoints(m): for each index 0..c-1, modelGetShadowPos(m (entry called again),
//      anim, (0,0,0), -16384, index, point, scratch float, null, float +224) gives a point; when the
//      word at +960 is non-zero (a record whose word +12 is a Minion) that point is carried through
//      getSoulAttachMatrix(minion, matrix A), modelGetAttachmentMatrix(R[28], anim, 0, matrix B, 1.0),
//      matInvert(B, B), matMul(A, B, A), matTransform(point, A, point). Two particles per point:
//      rejection-sampled unit-ball directions with z cleared and scaled by 15.0, added with
//      P_AddParticle(soulParticle, point, direction, 0.0). Afterwards (also when c <= 0) the word at
//      +12 gains 0x4002 and objectAddToDeleteList(this) runs.
//  24: transition(148, 0.1, 6); word +256 = t + 300; word +12 |= 9.
// soulParticle (0x007d6d80) is file-local in the original; here it is an unresolved data import.
// The meaning of the states is unknown. Nothing here ran on the EE.
#include "../w10-actor-n-common/access.hpp"

extern "C" {
extern char levelName[];
extern unsigned char norrath_w10_actor_n_view_7d6d80[];   // soulParticle in the original unit
int strcasecmp(const char* a, const char* b);
int setStateProcessing__8CreaturePi(void* self, int* state);
unsigned eRand__10GameObject(void* self);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float time, int flag);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flag);
void animGlobalSync__FP14AnimationState(void* state);
void listDestroy__FP8ListHead(void* head);
void objectAddToDeleteList__FP10GameObject(void* object);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(void* model, void* anim, Point3 position, short a, int index,
                                                                                         Point3* out, float* scratch, void* matrix, float scale);
void getSoulAttachMatrix__6MinionP8Matrix34(void* minion, float* matrix);
void modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(void* model, void* anim, int index, float* matrix, float scale);
void matInvert__FP8Matrix34T0(float* destination, float* source);
void matMul__FP8Matrix34N20(float* destination, float* a, float* b);
void matTransform__FP6Point3P8Matrix34T0(Point3* destination, float* matrix, Point3* source);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float extra);

namespace {
inline float roll_component(void* self) {
    const float v = static_cast<float>(static_cast<int>(eRand__10GameObject(self))) * w10n::bits(0x37800000u);
    return v + v - 1.0f;
}
}  // namespace

void setState__4Souli(void* self, int state) {
    using namespace w10n;
    unsigned char* const o = static_cast<unsigned char*>(self);
    int n = state;
    if (strcasecmp(levelName + 81, "airhub") == 0) n = 24;
    if (setStateProcessing__8CreaturePi(self, &n) != 0) return;
    if (word(o, 960) != 0u && static_cast<unsigned>(n - 16) >= 7u) return;
    const int current = half(o, 572);
    if (current == 17) return;
    unsigned char* const block = from_word(word(o, 36));
    void* const anim = o + 288;
    auto header = [&](unsigned offset) { return from_word(word(block, offset)); };
    auto transition = [&](unsigned offset, float time, int flag) { animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(anim, header(offset), time, flag); };
    auto one_shot = [&](unsigned offset, float time, int flag) { animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(anim, header(offset), time, time, flag); };
    if (static_cast<unsigned>(n) < 25u) {
        switch (n) {
            case 0: {
                const unsigned e = eRand__10GameObject(self);
                word(o, 256) = word(o, 808) + e % 120u + 60u;
                transition(60, 0.3f, 3);
                break;
            }
            case 1: {
                resetRouting__8Creature(self);
                float point[3];
                getSmartRandomDestination__8Creaturei(point, self, 200);
                real(o, 272) = point[0]; real(o, 276) = point[1]; real(o, 280) = point[2];
                transition(76, 0.3f, 0x803);
                animGlobalSync__FP14AnimationState(anim);
                const unsigned e = eRand__10GameObject(self);
                word(o, 256) = word(o, 808) + e % 120u + 300u;
                break;
            }
            case 5:
            case 13:
                transition(52, 0.3f, 3);
                break;
            case 8:
                call_entry(o, 184, current == 14 ? 15u : 14u);
                break;
            case 9:
                n = current;
                break;
            case 10:
                transition(140, 0.1f, 6);
                word(o, 256) = word(o, 808) + 180u;
                word(o, 12) |= 0x4000u;
                call_entry(o, 192);
                break;
            case 14: {
                transition(68, 0.3f, 3);
                const unsigned e = eRand__10GameObject(self);
                word(o, 256) = word(o, 808) + e % 120u;
                break;
            }
            case 15:
                if (current == 14) one_shot(44, 0.1f, 16);
                else n = current;
                break;
            case 16:
                one_shot(84, 0.1f, 8);
                break;
            case 17:
                listDestroy__FP8ListHead(o + 960);
                word(o, 960) = 0u;
                word(o, 256) = word(o, 808) + 240u;
                break;
            case 18:
                one_shot(116, 0.1f, 12);
                break;
            case 19:
                one_shot(92, 0.5f, 8);
                break;
            case 20:
                transition(108, 0.1f, 3);
                break;
            case 21:
                transition(124, 0.3f, 3);
                break;
            case 22:
                one_shot(132, 0.1f, 8);
                break;
            case 23: {
                int index = 0;
                const int count = modelGetNmShadowPoints__FP12_modelHeader(from_word(call_entry(o, 128)));
                if (count > 0) {
                    Point3 point;
                    float scratch[4];
                    float attach[12];
                    float anim_matrix[12];
                    Point3 direction;
                    const Point3 origin = {0.0f, 0.0f, 0.0f};
                    do {
                        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(from_word(call_entry(o, 128)), anim, origin, -16384, index, &point,
                                                                                                             scratch, nullptr, real(o, 224));
                        if (word(o, 960) != 0u) {
                            getSoulAttachMatrix__6MinionP8Matrix34(from_word(word(from_word(word(o, 960)), 12)), attach);
                            modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(from_word(word(block, 28)), anim, 0, anim_matrix, 1.0f);
                            matInvert__FP8Matrix34T0(anim_matrix, anim_matrix);
                            matMul__FP8Matrix34N20(attach, anim_matrix, attach);
                            matTransform__FP6Point3P8Matrix34T0(&point, attach, &point);
                        }
                        for (int pass = 0; pass < 2; ++pass) {
                            do {
                                direction.x = roll_component(self);
                                direction.y = roll_component(self);
                                direction.z = roll_component(self);
                            } while (1.0f < direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
                            direction.z = 0.0f;
                            direction.x = direction.x * 15.0f;
                            direction.y = direction.y * 15.0f;
                            direction.z = direction.z * 15.0f;
                            P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w10_actor_n_view_7d6d80, &point, &direction, 0.0f);
                        }
                        ++index;
                    } while (index < count);
                }
                word(o, 12) |= 0x4002u;
                objectAddToDeleteList__FP10GameObject(self);
                break;
            }
            case 24:
                transition(148, 0.1f, 6);
                word(o, 256) = word(o, 808) + 300u;
                word(o, 12) |= 9u;
                break;
            default:
                break;
        }
    }
    half(o, 572) = static_cast<short>(n);
}
}
