#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_run__9Generator at 0x001f1b50, 1180 bytes. h is the signed halfword at +56, p the position
// floats at +16, +20, +24, f the word at +980 (flags), and setState(3) the virtual slot at table byte 184 on
// this object (adjusted by that slot's signed halfword).
//  1. everyFrame(object, null); a non-zero result returns. objectActivationCheck(object, 600); a zero result
//     returns. Then the signed halfword state at +572 selects: 0 and 3 are handled, every other value returns.
//  2. State 0:
//     a. f bit 1 set: worldHitscanWithObjects(world, start = (p.x, p.y, p.z + 10), direction =
//        (icos(h) * 500, isin(h) * 500, 0 * 500), 1.1, this, &hit, 0, 0). When the result r satisfies 0 < r < 1,
//        hit is non-null and bit 6 (0x40) of the word at hit+12 is set: setState(3) and return. Otherwise,
//        unless ((frameNm xor the signed byte at +569) & 7) is zero, return. Then a box is searched: the
//        centre is p + (icos(h) * 200, isin(h) * 200, 0), the half-size 200 on each axis;
//        objectFindInBox(low, high, list, 128). Every listed object with bit 6 set and bit 3 (8) clear in its
//        word at +12 triggers setState(3) (once per such object, the loop continues).
//     b. f bit 1 clear and bit 0 clear: the same hitscan with start/direction as above (the constant 1.1 is a
//        second, separate word with the same bits); with 0 < r < 1, a non-null hit and bit 6 set, setState(3).
//        No frame check and no box search.
//     c. f bit 1 clear and bit 0 set: return.
//  3. State 3: when the word at +984 is non-zero, bit 3 (8) of the word at (word at (+984) + 12) + 12 must be
//     set, else return. Then, when the signed word at +976 is below the signed word at +972:
//     listDestroy(object+984), the word at +984 is cleared, and objectCreateByName(the pointer word at +960,
//     a copy of p, h, null) runs. When that returns an object whose word at +12 has bit 5 (0x20) set, the
//     64-bit word at its +160 gains 0x01000000 and listOnewayLink(object+984, created, created) runs.
//     The word at +976 is incremented whether or not an object came back.
// Unknown: what the flags, the words at +960, +972, +976 and +984, and the 1.1 value mean.
extern "C" {
extern void* world;
extern int frameNm;
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
int objectActivationCheck__8Creaturei(void* self, int distance);
float icos__Fi(int angle);
float isin__Fi(int angle);
float worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(
    void* world, const void* start, const void* direction, float scale, void* self, void** hit, int first, int second);
void objectFindInBox__FG6Point3T0PP10GameObjecti(const void* low, const void* high, void** found, int capacity);
void listDestroy__FP8ListHead(void* head);
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* node, void* item);
void* objectCreateByName__FPcG6Point3sPPc(char* name, const void* point, short angle, char** arguments);
}

namespace {
using namespace vslots_b;
inline float constant(unsigned bits) { return __builtin_bit_cast(float, bits); }

void set_state_3(void* self) {
    typedef void (*StateFunction)(void* object, int state);
    void* adjusted;
    StateFunction state = slot_function<StateFunction>(self, 184u, &adjusted);
    state(adjusted, 3);
}

// Returns true when the hitscan found a blocking object whose bit 6 is set.
bool hitscan_blocked(void* self, float scale) {
    const Address object = address(self);
    float direction[3];
    direction[0] = icos__Fi(read_i16(object + 56u));
    direction[1] = isin__Fi(read_i16(object + 56u));
    direction[2] = constant(0u);
    float start[3];
    start[0] = read_single(object + 16u);
    start[1] = read_single(object + 20u);
    start[2] = read_single(object + 24u) + constant(0x41200000u);
    float far_point[3];
    far_point[0] = direction[0] * constant(0x43fa0000u);
    far_point[1] = direction[1] * constant(0x43fa0000u);
    far_point[2] = direction[2] * constant(0x43fa0000u);
    void* hit = nullptr;
    const float result = worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(
        world, start, far_point, scale, self, &hit, 0, 0);
    return constant(0u) < result && result < constant(0x3f800000u) && hit != nullptr &&
           (read_word(address(hit) + 12u) & 0x40u) != 0u;
}
}  // namespace

extern "C" void msg_run__9Generator(void* self) {
    const Address object = address(self);
    if (everyFrame__8CreatureP12_modelHeader(self, nullptr) != 0) return;
    if (objectActivationCheck__8Creaturei(self, 600) == 0) return;
    const short state = read_i16(object + 572u);
    if (state == 3) {
        const unsigned first = read_word(object + 984u);
        if (first != 0u &&
            (read_word(address(pointer(read_word(address(pointer(first)) + 12u))) + 12u) & 8u) == 0u) return;
        if (!(signed_word(read_word(object + 976u)) < signed_word(read_word(object + 972u)))) return;
        listDestroy__FP8ListHead(bytes(object + 984u));
        write_word(object + 984u, 0u);
        unsigned char point[12];
        for (unsigned i = 0; i < 12u; ++i) point[i] = bytes(object + 16u)[i];
        void* created = objectCreateByName__FPcG6Point3sPPc(
            static_cast<char*>(pointer(read_word(object + 960u))), point, read_i16(object + 56u), nullptr);
        if (created != nullptr && (read_word(address(created) + 12u) & 0x20u) != 0u) {
            write_u64(address(created) + 160u, read_u64(address(created) + 160u) | 0x01000000ull);
            listOnewayLink__FP8ListHeadT0Pv(bytes(object + 984u), created, created);
        }
        write_word(object + 976u, read_word(object + 976u) + 1u);
        return;
    }
    if (state != 0) return;
    const unsigned flags = read_word(object + 980u);
    if ((flags & 2u) == 0u) {
        if ((flags & 1u) != 0u) return;
        if (hitscan_blocked(self, constant(0x3f8ccccc))) set_state_3(self);
        return;
    }
    if (hitscan_blocked(self, constant(0x3f8ccccc))) {
        set_state_3(self);
        return;
    }
    const unsigned tag = read_u8(object + 569u);
    if (((frameNm ^ signed_word(tag < 0x80u ? tag : tag | 0xffffff00u)) & 7) != 0) return;
    float centre[3];
    const float cosine = icos__Fi(read_i16(object + 56u));
    const float sine = isin__Fi(read_i16(object + 56u));
    const float two_hundred = constant(0x43480000u);
    centre[0] = read_single(object + 16u) + cosine * two_hundred;
    centre[1] = read_single(object + 20u) + sine * two_hundred;
    centre[2] = read_single(object + 24u) + constant(0u) * two_hundred;
    float low[3] = {centre[0] - two_hundred, centre[1] - two_hundred, centre[2] - two_hundred};
    float high[3] = {centre[0] + two_hundred, centre[1] + two_hundred, centre[2] + two_hundred};
    unsigned found[128];
    objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, reinterpret_cast<void**>(found), 128);
    for (unsigned index = 0; found[index] != 0u; ++index) {
        const unsigned other_flags = read_word(address(pointer(found[index])) + 12u);
        if ((other_flags & 0x40u) != 0u && (other_flags & 8u) == 0u) set_state_3(self);
    }
}
