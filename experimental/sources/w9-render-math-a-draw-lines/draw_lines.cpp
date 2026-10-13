// Observed drawLines__Fv at 0x00184cd8, 1528 bytes (a local function). The records are 36
// bytes in lineBuffer (0x006f4880): +0 three singles (start point), +12 three singles (end
// point), +24 four colour bytes for the start (byte 24 lowest), +28 four colour bytes for
// the end, +32 a single (called pull). nmLines (0x004869fc) is the count; zero returns with
// nothing changed.
//  1. A packet is opened at glistp exactly as in drawSingleParticles: the word at +8 is
//     0x11000000, the open tag is glistp, zero bytes pad until (cursor & 15) == 12, the VIF
//     word 0x50000000 is written there, the GIF tag follows as 0x1064c00000008000 and the
//     dword 14, then A+D entries (0,63) (0x71001,71) (72,66). The cursor ends 68 bytes past the
//     VIF word.
//  2. For each record: each endpoint is shifted to P = point - pull * camera, where camera is
//     the singles at byte offsets 8, 20, 32 of worldCameraMatrix, transformed by
//     matTransform(&P, worldViewMatrix, &P) whose single result r gives w = r + pull. The line is
//     skipped when either endpoint has w < 20.0, w*1400.0 < |x| or w*1000.0 < |y| (the start is
//     tested first, and a failed start leaves the end untouched).
//  3. Otherwise each endpoint is scaled by 1.0 / w (x, y, z), the colour word of that endpoint
//     (byte 0 lowest) is written as the entry (colour, register 1) and a second entry holds
//     the endpoint as (int(x*16) + 0x8000) | ((int(y*16) + 0x8000) << 16) | (int(z*16) << 32)
//     with register 5.
//  4. The tail is closed as in drawSingleParticles: the GIF tag count and the VIF word's
//     quadword count are patched, the cursor is padded to 16, and the open DMA tag gets the
//     quadword count with ID 1, rewritten as ID 6, or a fresh 0x60000000 tag is appended if the
//     ID was not 1. drawAddToList(old glistp, 3, 0, 0, 0) is called after glistp takes the
//     cursor, and nmLines is set to zero.
// Inferred only: this draws lines; the pull and colour roles are guesses. Fresh compilation is
// not an original-byte match.
struct Point3 {
    float x, y, z;
};
struct Line {
    Point3 start;
    Point3 end;
    unsigned char start_color[4];
    unsigned char end_color[4];
    float pull;
};
static_assert(sizeof(Line) == 36, "record size");

extern "C" {
extern int nmLines;
extern unsigned char lineBuffer[];
extern unsigned char* glistp;
extern float worldCameraMatrix[];
extern unsigned char worldViewMatrix[];
float matTransform__FP6Point3P8Matrix44T0(void* result, void* matrix, const void* point);
void* drawAddToList__FPviP8_textureP11_drawRecordi(void* packet, int list, void* texture, void* record, int flag);
}

namespace {
inline void store32(unsigned char* address, unsigned value) { *reinterpret_cast<unsigned*>(address) = value; }
inline void store64(unsigned char* address, unsigned long long value) {
    *reinterpret_cast<unsigned long long*>(address) = value;
}
inline unsigned long long widen(int value) { return static_cast<unsigned long long>(static_cast<long long>(value)); }
inline void entry(unsigned char*& cursor, unsigned long long data, unsigned long long reg) {
    store64(cursor, data);
    store64(cursor + 8, reg);
    cursor += 16;
}
inline unsigned color_word(const unsigned char* c) {
    return static_cast<unsigned>(c[0]) | (static_cast<unsigned>(c[1]) << 8) | (static_cast<unsigned>(c[2]) << 16) |
           (static_cast<unsigned>(c[3]) << 24);
}
// Shift by the camera axis, transform, and apply the visibility tests; false when culled.
inline bool prepare(const Point3& source, float pull, Point3& out, float& w) {
    const float camera_x = worldCameraMatrix[2];
    const float camera_y = worldCameraMatrix[5];
    const float camera_z = worldCameraMatrix[8];
    out.x = source.x - camera_x * pull;
    out.y = source.y - camera_y * pull;
    out.z = source.z - camera_z * pull;
    const float r = matTransform__FP6Point3P8Matrix44T0(&out, worldViewMatrix, &out);
    w = r + pull;
    if (w < 20.0f) return false;
    if (w * 1400.0f < __builtin_fabsf(out.x)) return false;
    if (w * 1000.0f < __builtin_fabsf(out.y)) return false;
    return true;
}
}

extern "C" void drawLines__Fv() {
    if (nmLines == 0) return;
    unsigned char* base = glistp;
    unsigned char* cursor = base + 12;
    store32(base + 8, 0x11000000u);
    unsigned char* const open_tag = base;
    unsigned char* pad = cursor;
    while ((reinterpret_cast<__UINTPTR_TYPE__>(pad) & 15u) != 12u) *pad++ = 0;
    unsigned char* const vif_word = pad;
    unsigned char* const gif_tag = vif_word + 4;
    store64(gif_tag, 0x1064c00000008000ull);
    store64(gif_tag + 8, 14ull);
    cursor = vif_word + 20;
    entry(cursor, 0ull, 63ull);
    entry(cursor, 0x00071001ull, 71ull);
    entry(cursor, 72ull, 66ull);
    store32(vif_word, 0x50000000u);
    for (int n = 0; n < nmLines; ++n) {
        const Line* line = reinterpret_cast<const Line*>(lineBuffer) + n;
        Point3 a;
        float wa;
        if (!prepare(line->start, line->pull, a, wa)) continue;
        const float inv_a = 1.0f / wa;
        a.x = a.x * inv_a;
        a.y = a.y * inv_a;
        a.z = a.z * inv_a;
        Point3 b;
        float wb;
        if (!prepare(line->end, line->pull, b, wb)) continue;
        const float inv_b = 1.0f / wb;
        b.x = b.x * inv_b;
        b.y = b.y * inv_b;
        b.z = b.z * inv_b;
        entry(cursor, static_cast<unsigned long long>(color_word(line->start_color)), 1ull);
        const int ay = static_cast<int>(a.y * 16.0f);
        const int ax = static_cast<int>(a.x * 16.0f);
        const int az = static_cast<int>(a.z * 16.0f);
        entry(cursor, widen(ax + 0x8000) | (widen(ay + 0x8000) << 16) | (static_cast<unsigned long long>(
                                                                              static_cast<long long>(az))
                                                                          << 32),
              5ull);
        const int bz = static_cast<int>(b.z * 16.0f);
        const int by = static_cast<int>(b.y * 16.0f);
        const int bx = static_cast<int>(b.x * 16.0f);
        entry(cursor, static_cast<unsigned long long>(color_word(line->end_color)), 1ull);
        entry(cursor, widen(bx + 0x8000) | (widen(by + 0x8000) << 16) | (static_cast<unsigned long long>(
                                                                              static_cast<long long>(bz))
                                                                          << 32),
              5ull);
    }
    const unsigned long long gif_count = widen(((static_cast<int>((cursor - gif_tag) >> 3)) - 2) >> 1);
    const int vif_count = (static_cast<int>(cursor - vif_word) - 4) >> 4;
    const unsigned long long gif_old = *reinterpret_cast<unsigned long long*>(gif_tag);
    const unsigned vif_old = *reinterpret_cast<unsigned*>(vif_word);
    store64(gif_tag, gif_old | gif_count);
    store32(vif_word, vif_old | static_cast<unsigned>(vif_count));
    unsigned char* tail = cursor;
    while ((reinterpret_cast<__UINTPTR_TYPE__>(tail) & 15u) != 0u) *tail++ = 0;
    cursor = tail;
    const int tag_count = ((static_cast<int>((cursor - open_tag) >> 3)) - 2) >> 1;
    const unsigned long long tag_word = widen(tag_count) | 0x10000000ull;
    store64(open_tag, tag_word);
    if (((static_cast<long long>(tag_word) >> 28) & 7) == 1) {
        store64(open_tag, (tag_word & 0x8fffffffull) | 0x60000000ull);
    } else {
        store64(cursor + 8, 0ull);
        store64(cursor, 0x60000000ull);
        cursor += 16;
    }
    unsigned char* const old_list = glistp;
    glistp = cursor;
    drawAddToList__FPviP8_textureP11_drawRecordi(old_list, 3, 0, 0, 0);
    nmLines = 0;
}
