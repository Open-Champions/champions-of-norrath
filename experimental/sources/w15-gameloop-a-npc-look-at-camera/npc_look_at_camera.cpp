#include "../w9-animation-object-c-common/mem.hpp"
// Observed npcLookAtCamera__Fv at 0x002806b8, 168 bytes, 64-byte frame. For each of the ten object pointers of
// targetObjs (0x008d5d40), in order, with no null test: it subtracts the three floats of worldCameraPos (0x0037ee80)
// from the three floats at object + 16, keeps the result in a local 12-byte vector, calls iatan2__Fff with the first two
// differences and stores the halfword -16384 minus the integer result at object + 56. The third difference is
// stored to the local vector and never read here, so this source does not compute it. Nothing is returned.
// Inferred: object + 16 is a world position and object + 56 a heading, so each selectable character turns to face the
// camera; the -16384 offset is a quarter turn in the engine's 16-bit angle units. No field name is claimed.
// Unknown: the unit of the angle (only the 16384 offset suggests a 65536 circle) and which axis is the second argument.
extern "C" {
extern unsigned targetObjs[];   // ten 32-bit object pointers
extern float worldCameraPos[];
int iatan2__Fff(float a, float b);
}

extern "C" void npcLookAtCamera__Fv() {
    using namespace norrath_aoc;
    for (int i = 0; i < 10; ++i) {
        unsigned char* object = load_pointer(reinterpret_cast<unsigned char*>(&targetObjs[i]));
        float dx = load_float(object + 16) - worldCameraPos[0];
        float dy = load_float(object + 20) - worldCameraPos[1];
        store_half(object + 56, static_cast<unsigned>(-16384 - iatan2__Fff(dx, dy)));
    }
}
