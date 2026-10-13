#include "../w9-animation-object-a-common/mem.hpp"
// Observed gameLineUpdatePosition__FP17gameLinePathState at 0x00278c38, 716 bytes. Path
// state words used (offsets from the state; gameLineInitPathState clears 56 bytes and sets
// +0, +20, +28 and +32): +0 pointer to the followed position (three floats); +4 three
// floats, a unit vector; +16 and +17 bytes; +20 line name text; +28 line index (-1 = not
// yet looked up); +32 point index; +36 signed count; +40 halfword; +44 float; +48 and +52
// pointers. A line (gameLines[+28]) has: +4 point count, +16 float, +20 pointer to point
// records. A point record is 64 bytes: the position at +0, a plane normal at +12 (distanceToPlane
// takes both), floats at +48 and +52.
// Steps:
//   1. Byte +17 is cleared. When +28 is -1, gameLines[0..gameLineCount) is searched for the
//      first line whose first word (a name pointer) equals the text at +20 under strcasecmp;
//      the found index (or -1) is stored at +28. -1 returns 0; otherwise initPosition is
//      called (it sets +32 and byte +17).
//   2. d = distanceToPlane(point[+32], point[+32]+12, *(+0)) for the current point. When the
//      float at record+48 is <= d, +36 grows by one and +32 becomes (+32 + 1) % count. Else
//      when d < 0, +32 drops by one (to count - 1 from -1) and +36 drops by one.
//   3. +48 receives the address of the (new) current point record. When +36 is negative the
//      float at +44 is set to zero; otherwise it is (float)(+36 / count) +
//      (record[+52] + distanceToPlane(new record, new record + 12, *(+0))) / line[+16].
//   4. Byte +16 is set to (new +32 != old +32). When either byte +16 or +17 is nonzero,
//      gameLineGetNextPoint(state, 1, &p) is called; +4,+8,+12 receive (p - *(+0)) scaled
//      by rsqrt of its squared length (numerator 1), +40 receives iatan2(y, x) of the scaled
//      components, byte +16 is set to 1 and +52 becomes the address of record
//      (+32 + 1) % count.
//   5. Returns 1.
// Inferred: this steps an object along a closed path of planes, keeping a signed lap count
// (+36) and a path fraction (+44), and recomputes the heading when the target point changes.
// Names are not claimed. Unknown: the meaning of the record floats at +48/+52 and line +16.
extern "C" {
extern int gameLineCount;
extern unsigned char* gameLines[];
int strcasecmp(const char* left, const char* right);
void initPosition__FP17gameLinePathState(unsigned char* state);
float distanceToPlane__FP6Point3N20(const void* point, const void* normal, const void* origin);
void gameLineGetNextPoint__FP17gameLinePathStateiP6Point3(unsigned char* state, int step, float* point);
int iatan2__Fff(float y, float x);
}

namespace {
__attribute__((always_inline)) inline float reciprocal_sqrt(float length_squared) {
#if defined(__mips__)
    float result;
    float one = 1.0f;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(length_squared));
    return result;
#else
    return 1.0f / __builtin_sqrtf(length_squared);
#endif
}
}

extern "C" int gameLineUpdatePosition__FP17gameLinePathState(unsigned char* state) {
    using namespace norrath_ao;
    state[17] = 0;
    if (static_cast<int>(load_word(state + 28)) == -1) {
        int found = -1;
        if (gameLineCount > 0) {
            const char* text = reinterpret_cast<const char*>(load_pointer(state + 20));
            for (int index = 0; index < gameLineCount; ++index) {
                if (strcasecmp(reinterpret_cast<const char*>(load_pointer(gameLines[index])), text) == 0) {
                    found = index;
                    break;
                }
            }
        }
        store_word(state + 28, static_cast<unsigned>(found));
        if (found == -1) return 0;
        initPosition__FP17gameLinePathState(state);
    }
    unsigned char* line = gameLines[static_cast<int>(load_word(state + 28))];
    const int old_index = static_cast<int>(load_word(state + 32));
    const float distance = distanceToPlane__FP6Point3N20(
        load_pointer(line + 20) + old_index * 64, load_pointer(line + 20) + old_index * 64 + 12, load_pointer(state));
    int index = static_cast<int>(load_word(state + 32));
    unsigned char* points = load_pointer(line + 20);
    if (load_float(points + index * 64 + 48) <= distance) {
        const int count = static_cast<int>(load_word(line + 4));
        store_word(state + 36, load_word(state + 36) + 1u);
        index = (index + 1) % count;
        store_word(state + 32, static_cast<unsigned>(index));
    } else if (distance < 0.0f) {
        index -= 1;
        store_word(state + 32, static_cast<unsigned>(index));
        if (index < 0) store_word(state + 32, load_word(line + 4) - 1u);
        store_word(state + 36, load_word(state + 36) - 1u);
        index = static_cast<int>(load_word(state + 32));
    }
    const int offset = index * 64;
    store_pointer(state + 48, points + offset);
    if (static_cast<int>(load_word(state + 36)) < 0) {
        store_word(state + 44, 0);
    } else {
        unsigned char* record = load_pointer(line + 20) + offset;
        const float again = distanceToPlane__FP6Point3N20(record, record + 12, load_pointer(state));
        const int laps = static_cast<int>(load_word(state + 36)) / static_cast<int>(load_word(line + 4));
        index = static_cast<int>(load_word(state + 32));
        const float fraction = (load_float(load_pointer(line + 20) + index * 64 + 52) + again) / load_float(line + 16);
        store_float(state + 44, static_cast<float>(laps) + fraction);
    }
    state[16] = index != old_index;
    if (state[16] != 0 || state[17] != 0) {
        float next[3];
        gameLineGetNextPoint__FP17gameLinePathStateiP6Point3(state, 1, next);
        unsigned char* position = load_pointer(state);
        float dx = next[0] - load_float(position + 0);
        store_float(state + 4, dx);
        float dy = next[1] - load_float(position + 4);
        store_float(state + 8, dy);
        float dz = next[2] - load_float(position + 8);
        const float scale = reciprocal_sqrt(dx * dx + dy * dy + dz * dz);
        dz *= scale;
        dx *= scale;
        dy *= scale;
        store_float(state + 12, dz);
        store_float(state + 4, dx);
        store_float(state + 8, dy);
        store_half(state + 40, static_cast<unsigned>(iatan2__Fff(load_float(state + 8), dx)));
        state[16] = 1;
        const int count = static_cast<int>(load_word(line + 4));
        store_pointer(state + 52, load_pointer(line + 20) + ((static_cast<int>(load_word(state + 32)) + 1) % count) * 64);
    }
    return 1;
}
