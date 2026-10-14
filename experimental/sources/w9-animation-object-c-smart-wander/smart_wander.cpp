#include "../w9-animation-object-c-common/mem.hpp"
// Observed smartWander__8Creature at 0x00113128, 212 bytes.
//  1. Copies the 12-byte point at creature+272 to a local and calls
//     steerAndRouteToPosition__8CreatureG6Point3(creature, copy). If that returns nonzero,
//     calls the virtual slot at byte 184 of the vtable word at creature+72 with the
//     creature adjusted by the slot's signed halfword and the single argument 0.
//  2. Then, when the signed word at creature+716 is below 4, returns 1.
//  3. Otherwise runs resetRouting__8Creature, asks
//     getSmartRandomDestination__8Creaturei for a point (range 300, through a hidden first
//     pointer) and stores the 12 bytes at creature+272; returns 0.
// The result is the register value the original leaves; no caller in the tree reads it.
// Inferred: +716 counts routing attempts (resetRouting clears it) and +272 is the wander
// goal; slot 184 is called when the steering step reports a result. Names are not claimed.
// Unknown: what the virtual slot is (not named from this function) and the return type.
extern "C" {
int steerAndRouteToPosition__8CreatureG6Point3(unsigned char* self, unsigned char* point);
void resetRouting__8Creature(unsigned char* self);
void getSmartRandomDestination__8Creaturei(unsigned char* result, unsigned char* self, int range);
}

extern "C" int smartWander__8Creature(unsigned char* self) {
    using namespace norrath_aoc;
    unsigned char* goal = self + 272;
    unsigned copy[3];
    copy[0] = load_word(goal);
    copy[1] = load_word(goal + 4);
    copy[2] = load_word(goal + 8);
    if (steerAndRouteToPosition__8CreatureG6Point3(self, reinterpret_cast<unsigned char*>(copy)) != 0)
        call_virtual_pointer_int(self, 184, 0);
    if (static_cast<int>(load_word(self + 716)) < 4) return 1;
    resetRouting__8Creature(self);
    unsigned fresh[3];
    getSmartRandomDestination__8Creaturei(reinterpret_cast<unsigned char*>(fresh), self, 300);
    store_word(goal, fresh[0]);
    store_word(goal + 4, fresh[1]);
    store_word(goal + 8, fresh[2]);
    return 0;
}
