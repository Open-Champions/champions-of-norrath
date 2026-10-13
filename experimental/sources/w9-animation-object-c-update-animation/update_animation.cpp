#include "../w9-animation-object-c-common/mem.hpp"
// Observed updateAnimation__FP13animComponent at 0x00161528, 2248 bytes (a local function of the
// original object). Argument: a component record with the animation header pointer at +0, a
// frame-state pointer at +4, the float time at +24 and the halfword flags at +28. The header
// holds a track count (int at +0), a frame count (+4) and a pointer to a packed bit stream of
// halfwords (+8). Bits are read from the stream most significant first, through a 32-bit window
// of two consecutive halfwords: a field of n bits at bit position p is
// ((hw[p >> 4] << 16 | hw[(p >> 4) + 1]) << (p & 15)) >> (32 - n), logical for unsigned fields
// and arithmetic for signed ones. "Width" fields are 4 bits holding width - 1.
//  1. Frame state S = component+4. A null state is replaced by getFreeFrameState__Fv(), which is
//     linked both ways (S+20 = component, component+4 = S). A state that is not already first
//     (S+16 nonzero) is moved to the front of the list (head fsList 0x48690c, tail fsListBack
//     0x486910; S+12 is the next older state, S+16 the next newer one).
//  2. target = (int)time. If target < S+8 (the state's current frame) the state is rebuilt from
//     the start of the stream: S+8 = 0, then for every track i with record B = S + 28 + 60*i:
//     halfwords B+0 and B+2 become 0; a width w is read; unless (flags & 0x800) and i == 0, two
//     signed values of width w become floats B+4 and B+8 (scaled by 2^-6), otherwise those 2w
//     bits are skipped; a third value of width w becomes B+12 (2^-6); then a second width w2 and
//     four signed values of width w2 go to B+40, B+28, B+32, B+36 in that order (scaled by 2^-12);
//     floats B+16, B+20, B+24, B+44, B+48, B+52 and B+56 become 0. Afterwards S+4 = the bit position,
//     S+0 = the stream pointer, and the halfword at S+24 is iatan2(2 (x y + z w), (x x - y y - z z) +
//     w w) of the floats x, y, z, w at S+56, S+60, S+64, S+68 (track 0's B+28, B+32, B+36, B+40).
//  3. Then events are applied while they are not after target. At bit position S+4 an 8-bit frame
//     delta is read; the event frame is S+8 + delta, and nothing more happens (and S+4 is not
//     advanced) when it is above target. Otherwise S+8 = that frame, a flag bit and a 6-bit track
//     number are read, then a width w:
//     flag 1: B+4, B+8, B+12 gain B+16, B+20, B+24 times (frame - signed halfword B+0); three signed
//             values of width w, scaled by 2^-8, replace B+16, B+20, B+24; B+0 = frame.
//     flag 0: B+40, B+28, B+32, B+36 gain B+56, B+44, B+48, B+52 times (frame - signed halfword B+2);
//             four signed values of width w, scaled by 2^-17, replace B+56, B+44, B+48, B+52; B+2 = frame.
//     S+4 is advanced past everything read.
// Inferred: tracks are animation channels; B+4..B+12 a position with its velocity at B+16..B+24, and
// B+28..B+40 a four-float rotation with its rate at B+44..B+56; the yaw at S+24 is taken from track
// 0's rotation. The stream scales and field roles come from the arithmetic only.
// Unknown: the record layouts' declared types and why flag 0x800 skips track 0's planar position.
extern "C" {
extern unsigned char* fsList;
extern unsigned char* fsListBack;
unsigned char* getFreeFrameState__Fv();
int iatan2__Fff(float y, float x);
}

namespace {
using namespace norrath_aoc;
inline unsigned window(const unsigned char* stream, unsigned position) {
    const unsigned char* at = stream + ((position >> 4) << 1);
    return (load_half(at) << 16) | load_half(at + 2);
}
inline unsigned unsigned_field(const unsigned char* stream, unsigned position, unsigned width) {
    return (window(stream, position) << (position & 15)) >> (32 - width);
}
inline int signed_field(const unsigned char* stream, unsigned position, unsigned width) {
    return static_cast<int>(window(stream, position) << (position & 15)) >> (32 - width);
}
inline float scaled(int value, float scale) { return static_cast<float>(value) * scale; }
}

extern "C" void updateAnimation__FP13animComponent(unsigned char* component) {
    unsigned char* state = load_pointer(component + 4);
    if (state == nullptr) {
        state = getFreeFrameState__Fv();
        store_pointer(state + 20, component);
        store_pointer(component + 4, state);
    }
    if (load_pointer(state + 16) != nullptr) {
        unsigned char* const older = load_pointer(state + 12);
        if (older == nullptr) fsListBack = load_pointer(state + 16);
        else store_pointer(older + 16, load_pointer(state + 16));
        unsigned char* const newer = load_pointer(state + 16);
        store_pointer(newer + 12, load_pointer(state + 12));
        store_pointer(state + 12, fsList);
        store_pointer(fsList + 16, state);
        fsList = state;
        store_pointer(state + 16, nullptr);
    }
    const unsigned char* const header = load_pointer(component);
    const int target = static_cast<int>(load_float(component + 24));
    const int tracks = static_cast<int>(load_word(header));
    const unsigned char* const stream = load_pointer(header + 8);
    if (target < static_cast<int>(load_word(state + 8))) {
        store_word(state + 8, 0);
        unsigned position = 0;
        for (int track = 0; track < tracks; ++track) {
            unsigned char* const record = state + 28 + 60 * track;
            store_half(record + 0, 0);
            store_half(record + 2, 0);
            const unsigned width = unsigned_field(stream, position, 4) + 1;
            position += 4;
            if ((load_half(component + 28) & 0x800u) != 0 && track == 0) {
                position += width;
                position += width;
            } else {
                const int first = signed_field(stream, position, width);
                position += width;
                const int second = signed_field(stream, position, width);
                position += width;
                store_float(record + 4, scaled(first, 0x1p-6f));
                store_float(record + 8, scaled(second, 0x1p-6f));
            }
            const int third = signed_field(stream, position, width);
            position += width;
            const unsigned width2 = unsigned_field(stream, position, 4) + 1;
            position += 4;
            const int v2 = signed_field(stream, position, width2);
            position += width2;
            const int v3 = signed_field(stream, position, width2);
            position += width2;
            const int v4 = signed_field(stream, position, width2);
            position += width2;
            const int v5 = signed_field(stream, position, width2);
            position += width2;
            store_float(record + 12, scaled(third, 0x1p-6f));
            store_float(record + 16, 0.0f);
            store_float(record + 20, 0.0f);
            store_float(record + 24, 0.0f);
            store_float(record + 28, scaled(v3, 0x1p-12f));
            store_float(record + 32, scaled(v4, 0x1p-12f));
            store_float(record + 36, scaled(v5, 0x1p-12f));
            store_float(record + 40, scaled(v2, 0x1p-12f));
            store_float(record + 44, 0.0f);
            store_float(record + 48, 0.0f);
            store_float(record + 52, 0.0f);
            store_float(record + 56, 0.0f);
        }
        store_word(state + 4, position);
        store_pointer(state + 0, const_cast<unsigned char*>(stream));
        const float x = load_float(state + 56);
        const float y = load_float(state + 60);
        const float z = load_float(state + 64);
        const float w = load_float(state + 68);
        const int yaw = iatan2__Fff((x * y + z * w) + (x * y + z * w), ((x * x - y * y) - z * z) + w * w);
        store_half(state + 24, static_cast<unsigned>(yaw));
    }
    const unsigned char* const bits = load_pointer(state + 0);
    for (;;) {
        unsigned position = load_word(state + 4);
        const int frame = static_cast<int>(load_word(state + 8)) + static_cast<int>(unsigned_field(bits, position, 8));
        if (target < frame) return;
        const unsigned marker = position + 8;
        const bool position_event = unsigned_field(bits, marker, 1) != 0;
        const unsigned track = unsigned_field(bits, marker + 1, 6);
        store_word(state + 8, static_cast<unsigned>(frame));
        unsigned cursor = marker + 7;
        const unsigned width = unsigned_field(bits, cursor, 4) + 1;
        cursor = marker + 11;
        unsigned char* const record = state + 28 + 60 * track;
        if (position_event) {
            const float elapsed = static_cast<float>(frame - load_signed_half(record + 0));
            const float dx = load_float(record + 16) * elapsed;
            const float dy = load_float(record + 20) * elapsed;
            const float dz = load_float(record + 24) * elapsed;
            const int x = signed_field(bits, cursor, width);
            const int y = signed_field(bits, cursor + width, width);
            const int z = signed_field(bits, cursor + 2 * width, width);
            store_float(record + 4, load_float(record + 4) + dx);
            store_float(record + 8, load_float(record + 8) + dy);
            store_float(record + 12, load_float(record + 12) + dz);
            store_half(record + 0, static_cast<unsigned>(frame));
            store_float(record + 16, scaled(x, 0x1p-8f));
            store_float(record + 20, scaled(y, 0x1p-8f));
            store_float(record + 24, scaled(z, 0x1p-8f));
            store_word(state + 4, cursor + 3 * width);
        } else {
            const float elapsed = static_cast<float>(frame - load_signed_half(record + 2));
            const float r0 = load_float(record + 44) * elapsed;
            const float r1 = load_float(record + 48) * elapsed;
            const float r2 = load_float(record + 52) * elapsed;
            const float r3 = load_float(record + 56) * elapsed;
            const int a = signed_field(bits, cursor, width);
            const int b = signed_field(bits, cursor + width, width);
            const int c = signed_field(bits, cursor + 2 * width, width);
            const int d = signed_field(bits, cursor + 3 * width, width);
            store_float(record + 40, load_float(record + 40) + r3);
            store_float(record + 28, load_float(record + 28) + r0);
            store_float(record + 32, load_float(record + 32) + r1);
            store_float(record + 36, load_float(record + 36) + r2);
            store_half(record + 2, static_cast<unsigned>(frame));
            store_float(record + 56, scaled(a, 0x1p-17f));
            store_float(record + 44, scaled(b, 0x1p-17f));
            store_float(record + 48, scaled(c, 0x1p-17f));
            store_float(record + 52, scaled(d, 0x1p-17f));
            store_word(state + 4, cursor + 4 * width);
        }
    }
}
