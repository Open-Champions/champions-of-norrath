#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_run__7FireFly at 0x00211d08, 612 bytes. Words: A is +968 (a toggle), B is +972 (a due time),
// C is +808 (the clock), D is +256 (a second due time), all compared as signed. Positions: p is the three
// floats at +16, W the 12 bytes at +976 (the current wander target), R the 12 bytes at +272 (a routing target).
//  1. newDamageHandler, then everyFrame, each with the object and the word at +12 of the group at +36
//     (re-read before each call). A non-zero everyFrame result returns at once.
//  2. objectActivationCheck(object, 600) with its result unused, creatureAdvanceAnimation(1.0), slideBack(1.0).
//  3. When B < C: with A zero, a new wander target is built: heading h = signed halfword at +56 plus or minus
//     8192 (minus when eRand's low bit is set, wrapped to 16 bits); W = (p.x + icos(h) * 100,
//     p.y + isin(h) * 100, p.z); the due time is C + 20 (C re-read). With A non-zero the due time is C + 30
//     (C as read before). Then B takes the due time and A is flipped (xor 1).
//  4. steerAndRouteToPosition with a copy of W when A is now non-zero, otherwise with a copy of R.
//  5. When that returned non-zero, or D < C: resetRouting; getSmartRandomDestination(object, 300) gives a
//     point that is stored to R; D = C + (eRand mod 3, unsigned) + 300.
//  6. The 12 bytes of p are copied to +1112 in every path that gets this far.
// Unknown: what the toggle, the two due times and the +1112 copy are for.
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
int objectActivationCheck__8Creaturei(void* self, int distance);
void creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
unsigned eRand__10GameObject(void* self);
float icos__Fi(int angle);
float isin__Fi(int angle);
int steerAndRouteToPosition__8CreatureG6Point3(void* self, const void* point);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
}

namespace {
using namespace vslots_b;
void* group_model(Address object) { return pointer(read_word(address(pointer(read_word(object + 36u))) + 12u)); }
}

extern "C" void msg_run__7FireFly(void* self) {
    const Address object = address(self);
    newDamageHandler__8CreatureP12_modelHeader(self, group_model(object));
    if (everyFrame__8CreatureP12_modelHeader(self, group_model(object)) != 0) return;
    objectActivationCheck__8Creaturei(self, 600);
    creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, 1.0f);
    const Address position = object + 16u;
    const int clock = signed_word(read_word(object + 808u));
    if (signed_word(read_word(object + 972u)) < clock) {
        unsigned toggle = read_word(object + 968u);
        unsigned due = static_cast<unsigned>(clock) + 30u;
        if (toggle == 0u) {
            const int heading = read_i16(object + 56u);
            const unsigned roll = eRand__10GameObject(self);
            const short turned = static_cast<short>((roll & 1u) != 0u ? heading - 8192 : heading + 8192);
            const float cosine = icos__Fi(turned);
            const float sine = isin__Fi(turned);
            const float hundred = __builtin_bit_cast(float, 0x42c80000u);
            write_single(object + 976u, read_single(position) + cosine * hundred);
            write_single(object + 980u, read_single(position + 4u) + sine * hundred);
            write_single(object + 984u, read_single(position + 8u));
            due = read_word(object + 808u) + 20u;
            toggle = read_word(object + 968u);
        }
        write_word(object + 972u, due);
        write_word(object + 968u, toggle ^ 1u);
    }
    unsigned char target[12];
    const Address from = read_word(object + 968u) != 0u ? object + 976u : object + 272u;
    for (unsigned i = 0; i < 12u; ++i) target[i] = bytes(from)[i];
    const int arrived = steerAndRouteToPosition__8CreatureG6Point3(self, target);
    if (arrived != 0 || signed_word(read_word(object + 256u)) < signed_word(read_word(object + 808u))) {
        resetRouting__8Creature(self);
        unsigned char destination[12];
        getSmartRandomDestination__8Creaturei(destination, self, 300);
        for (unsigned i = 0; i < 12u; ++i) bytes(object + 272u)[i] = destination[i];
        const unsigned roll = eRand__10GameObject(self);
        write_word(object + 256u, read_word(object + 808u) + roll % 3u + 300u);
    }
    for (unsigned i = 0; i < 12u; ++i) bytes(object + 1112u)[i] = bytes(position)[i];
}
