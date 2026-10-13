// Observed msg_run__3NPC at 0x002320f0, 1212 bytes: the per-frame run routine of NPC. Offsets are
// bytes; "set(n)" below is the table entry at (word +72) + 184 called with the adjusted object
// and n. Order of events:
//  1. everyFrame(this, model word +1056) runs and its result is ignored.
//  2. When the text at +1068 equals "firiona" (case-insensitive) and the text at levelName+81 is
//     not "airhub", the halfwords at +140, +142, +144, +146 become 50, 50, 154, 83.
//  3. With the word at +1132 zero, a zero objectActivationCheck(this, 600) ends the call.
//  4. creatureAdvanceAnimation(this, 1.0) runs (result unused).
//  5. With the word at +1132 set (call it P, an object): the table entry at (word +72 of P) + 144
//     is called on the adjusted P with (this+16, this+56), then objectUpdateInGrid(this); go to 8.
//  6. Otherwise on the state, the halfword at +572: state 1 saves (int)float +176; a double
//     compare of |float +180| then |float +184| against 0.5 (the second only when the first is
//     not above it) sets float +176 to 0 when either exceeds it; smartWander(this) runs; the
//     saved integer is converted back and stored to +176. State 0: when bit 27 of the 64-bit
//     word at +160 is set, set(1) runs. Other states do nothing.
//  8. When the text at +960 equals "we_king.lmp": modelGetCharAttachmentPos(model, copy of
//     the point at +16, halfword +56, animation state at +288, 0, A, B, float +224) fills two
//     points A and B; if eRand(this) & 3 is zero, two random points in the unit ball are drawn
//     (components 2*(eRand*2^-16)-1, redrawn until the squared length is at most 1; the first
//     ball is scaled by 12 and added to B but never used), the second ball is added to A, and
//     P_AddParticle(particle block 0x007d7dd0, A, B, 0.0) runs.
// Nothing here ran on the EE. What the fields mean is unknown.
#include "../w10-actor-j-common/access.hpp"

extern "C" {
int everyFrame__8CreatureP12_modelHeader(void* self, void* header);
int strcasecmp(const char* a, const char* b);
int strcmp(const char* a, const char* b);
int objectActivationCheck__8Creaturei(void* self, int range);
unsigned creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void objectUpdateInGrid__FP10GameObject(void* object);
// Software double-precision helpers of the executable (the image calls these, not libgcc names):
// float to double, signed three-way compare (negative, zero, positive), and a - b.
double fptodp(float value);
int dpcmp(double a, double b);
double dpsub(double a, double b);
void smartWander__8Creature(void* self);
unsigned eRand__10GameObject(void* self);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, Point3 position, short a, void* animation, int b, Point3* first, Point3* second, float f);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float f);
extern char levelName[];
extern unsigned char npc_msg_run_particle_7d7dd0[];
}

namespace {
unsigned roll(unsigned char* o) { return eRand__10GameObject(o); }
}

extern "C" void msg_run__3NPC(void* self) {
    using namespace w10j;
    unsigned char* const o = static_cast<unsigned char*>(self);
    everyFrame__8CreatureP12_modelHeader(self, from_word(word(o, 1056)));
    if (strcasecmp(reinterpret_cast<const char*>(o + 1068), "firiona") == 0 && strcasecmp(levelName + 81, "airhub") != 0) {
        half(o, 142) = 50;
        half(o, 144) = 154;
        half(o, 146) = 83;
        half(o, 140) = 50;
    }
    if (word(o, 1132) == 0 && objectActivationCheck__8Creaturei(self, 600) == 0) return;
    creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    if (word(o, 1132) != 0) {
        unsigned char* const parent = from_word(word(o, 1132));
        call_entry(parent, 144, to_word(o + 16), to_word(o + 56));
        objectUpdateInGrid__FP10GameObject(self);
    } else {
        const int state = half(o, 572);
        if (state == 1) {
            const int saved = static_cast<int>(real(o, 176));
            double d = fptodp(real(o, 180));
            if (dpcmp(d, 0.0) < 0) d = dpsub(0.0, d);
            if (dpcmp(d, 0.5) > 0) {
                real(o, 176) = 0.0f;
            } else {
                d = fptodp(real(o, 184));
                if (dpcmp(d, 0.0) < 0) d = dpsub(0.0, d);
                if (dpcmp(d, 0.5) > 0) real(o, 176) = 0.0f;
            }
            smartWander__8Creature(self);
            real(o, 176) = static_cast<float>(saved);
        } else if (state == 0) {
            if ((dword(o, 160) & 0x08000000ull) != 0) call_entry(o, 184, 1);
        }
    }
    if (strcmp(reinterpret_cast<const char*>(o + 960), "we_king.lmp") != 0) return;
    Point3 first, second;
    const Point3 position = *reinterpret_cast<Point3*>(o + 16);
    modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
        from_word(word(o, 1056)), position, half(o, 56), o + 288, 0, &first, &second, real(o, 224));
    if ((eRand__10GameObject(o) & 3) != 0) return;
    Point3 a, b;
    unit_ball(o, a, roll);
    unit_ball(o, b, roll);
    first.x = first.x + b.x;
    first.y = first.y + b.y;
    first.z = first.z + b.z;
    P_AddParticle__FP11ParticleDefP6Point3T1f(npc_msg_run_particle_7d7dd0, &first, &second, 0.0f);
}
