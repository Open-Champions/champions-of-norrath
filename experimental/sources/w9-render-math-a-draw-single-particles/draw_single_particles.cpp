// Observed drawSingleParticles__Fi at 0x001820f8, 2012 bytes (a local function). a0 = mode.
// Records are the 32-byte entries of singleBuffer (0x006ec880) written by P_FaceSprite:
// +0 three singles (position), +12 single (called size here), +16 single (called pull),
// +20..23 four bytes (colour, byte 20 lowest), +24 texture pointer, +28 halfword angle,
// +30 halfword flags. nmSingle (0x004869f8) is the record count. Flag bit 0 selects a rotated
// quad (otherwise a sprite), bit 1 selects the alpha word 72 over 68, bit 2 is the mode.
//  1. nmSingle == 0 returns. If GLISTSIZE - ((glistp - gstart) >> 3) is below 6000, nmSingle is
//     zeroed and the function returns.
//  2. Repeat: scan from record 0 for the first record whose texture pointer is non-zero and whose
//     flag bit 2 equals mode (mode is compared as a whole int). None found returns. The found
//     record fixes the texture T and the signed flag halfword K.
//  3. A packet is opened at glistp: the word at +8 is 0x11000000, the open tag is glistp, the
//     cursor starts 12 bytes in and zero bytes are written until (cursor & 15) == 12. The VIF word
//     0x50000000 is written there (its count is patched later) and a GIF tag follows: 0x10ae400000008000 when flag bit 0 of the first record is set,
//     else 0x10af400000008000, then the dword 14. A+D entries follow (data, register):
//     (0,63) (0,6) (97,20) (0x71001,71) (0x1212303012123030,68), then (72,66) when flag bit 1
//     of the first record is set else (68,66).
//  4. For each record from the found index up whose texture is T and whose flag halfword equals K
//     (sign-extended): the texture word is cleared first; if less than 6000 list entries remain
//     the record is skipped. Otherwise the point P = position - pull * camera (the singles at
//     byte offsets 8, 20, 32 of worldCameraMatrix, as x, y, z) is transformed by
//     matTransform(&P, worldViewMatrix, &P) whose single result r gives w = r + pull. The record
//     is skipped when w < 20.0, when w*700.0 < |P.x| or when w*300.0 < |P.y|. Otherwise inv =
//     1.0 / w, depth = w - pull, and the entry (colour word, 0 in the upper half of the first dword,
//     register 1) is written. P.x and P.y are scaled by inv, P.z by 1.0/depth. Then x, y, z and the
//     size scaled as ((single * 16) * inv for the size) are converted to integers; x and y get
//     0x8000 added. With flag bit 0 the offsets C = int(icos(angle) * size) and
//     S = int(isin(angle) * size) give eight entries (UV, XYZ3, UV, XYZ3, UV, XYZ2, UV, XYZ2) for
//     a rotated quad; without it four entries (UV, XYZ2, UV, XYZ2) draw a sprite from
//     (x - 2*size, y - size) to (x + 2*size, y + size). The texture's first two halfwords
//     are its width and height.
//  5. The tail entry (0x3131757531317575, 68) is added, the GIF tag count and the VIF word's
//     quadword count are patched, the cursor is padded to 16, the open DMA tag gets the quadword
//     count with ID 1 and is then rewritten as ID 6 (or, if ID was not 1, a new 0x60000000 tag is
//     appended). drawAddToList(old glistp, 3 when mode == 0 else 4, T, 0, 0) is called after glistp
//     takes the cursor. The scan repeats.
// Inferred only: the names, the pull and size roles, and the meaning of the flag bits. Fresh
// compilation is not an original-byte match.
struct Point3 {
    float x, y, z;
};

struct Single {
    Point3 position;
    float size;
    float pull;
    unsigned char color[4];
    short* texture;
    short angle;
    unsigned short flags;
};
static_assert(sizeof(Single) == 32, "record size");

extern "C" {
extern int nmSingle;
extern unsigned char singleBuffer[];
extern unsigned char* glistp;
extern unsigned char* gstart;
extern int GLISTSIZE;
extern float worldCameraMatrix[];
extern unsigned char worldViewMatrix[];
float matTransform__FP6Point3P8Matrix44T0(void* result, void* matrix, const void* point);
float icos__Fi(int angle);
float isin__Fi(int angle);
void* drawAddToList__FPviP8_textureP11_drawRecordi(void* packet, int list, void* texture, void* record, int flag);
}

namespace {
inline void store32(unsigned char* address, unsigned value) { *reinterpret_cast<unsigned*>(address) = value; }
inline void store64(unsigned char* address, unsigned long long value) {
    *reinterpret_cast<unsigned long long*>(address) = value;
}
inline unsigned long long widen(int value) { return static_cast<unsigned long long>(static_cast<long long>(value)); }
inline int remaining(const unsigned char* cursor) {
    return GLISTSIZE - static_cast<int>((cursor - gstart) >> 3);
}
inline void entry(unsigned char*& cursor, unsigned long long data, unsigned long long reg) {
    store64(cursor, data);
    store64(cursor + 8, reg);
    cursor += 16;
}
}

extern "C" void drawSingleParticles__Fi(int mode) {
    int count = nmSingle;
    if (count == 0) return;
    Single* const records = reinterpret_cast<Single*>(singleBuffer);
    if (remaining(glistp) < 6000) {
        nmSingle = 0;
        return;
    }
    for (;;) {
        count = nmSingle;
        int key = 0;
        int index = 0;
        short* texture = 0;
        for (; index < count; ++index) {
            Single* record = records + index;
            if (record->texture != 0 && mode == ((record->flags >> 2) & 1)) {
                texture = record->texture;
                key = static_cast<short>(record->flags);
                break;
            }
        }
        if (index == count) return;
        unsigned char* base = glistp;
        unsigned char* cursor = base + 12;
        store32(base + 8, 0x11000000u);
        unsigned char* const open_tag = base;
        unsigned char* pad = cursor;
        while ((reinterpret_cast<__UINTPTR_TYPE__>(pad) & 15u) != 12u) *pad++ = 0;
        cursor = pad;
        unsigned char* const vif_word = cursor;
        store32(vif_word, 0x50000000u);
        cursor = vif_word + 4;
        unsigned char* const gif_tag = cursor;
        const unsigned first_flags = records[index].flags;
        store64(gif_tag, (first_flags & 1u) != 0 ? 0x10ae400000008000ull : 0x10af400000008000ull);
        store64(gif_tag + 8, 14ull);
        cursor = vif_word + 20;
        entry(cursor, 0ull, 63ull);
        entry(cursor, 0ull, 6ull);
        entry(cursor, 97ull, 20ull);
        entry(cursor, 0x00071001ull, 71ull);
        entry(cursor, 0x1212303012123030ull, 68ull);
        if ((records[index].flags & 2u) != 0) {
            entry(cursor, 72ull, 66ull);
        } else {
            entry(cursor, 68ull, 66ull);
        }
        for (; index < nmSingle; ++index) {
            Single* record = records + index;
            if (record->texture != texture) continue;
            if (static_cast<short>(record->flags) != key) continue;
            const int room = remaining(cursor);
            record->texture = 0;
            if (room < 6000) continue;
            const float pull = record->pull;
            const float px = record->position.x;
            const float py = record->position.y;
            const float shift_x = worldCameraMatrix[2] * pull;
            const float shift_y = worldCameraMatrix[5] * pull;
            const float shift_z = worldCameraMatrix[8] * pull;
            Point3 point;
            point.x = px - shift_x;
            point.y = py - shift_y;
            point.z = record->position.z - shift_z;
            const float transformed = matTransform__FP6Point3P8Matrix44T0(&point, worldViewMatrix, &point);
            const float w = transformed + pull;
            if (w < 20.0f) continue;
            const float limit_x = w * 700.0f;
            if (limit_x < __builtin_fabsf(point.x)) continue;
            const float limit_y = w * 300.0f;
            if (limit_y < __builtin_fabsf(point.y)) continue;
            const float depth = w - pull;
            const float inv = 1.0f / w;
            const float inv_depth = 1.0f / depth;
            const unsigned color = static_cast<unsigned>(record->color[0]) |
                                   (static_cast<unsigned>(record->color[1]) << 8) |
                                   (static_cast<unsigned>(record->color[2]) << 16) |
                                   (static_cast<unsigned>(record->color[3]) << 24);
            const bool rotated = (record->flags & 1u) != 0;
            store32(cursor, color);
            store32(cursor + 4, 0u);
            store64(cursor + 8, 1ull);
            cursor += 16;
            point.y = point.y * inv;
            point.x = point.x * inv;
            point.z = point.z * inv_depth;
            const float scaled_x = point.x * 16.0f;
            const float scaled_y = point.y * 16.0f;
            const float size_scaled = record->size * 16.0f;
            const float scaled_z = point.z * 16.0f;
            const int ix = static_cast<int>(scaled_x);
            const int iy = static_cast<int>(scaled_y);
            const int iz = static_cast<int>(scaled_z);
            const int isize = static_cast<int>(size_scaled * inv);
            const int cx = ix + 0x8000;
            const int cy = iy + 0x8000;
            const unsigned long long z64 = static_cast<unsigned long long>(static_cast<long long>(iz)) << 32;
            const int width = texture[0];
            const int height = texture[1];
            if (rotated) {
                const float fsize = static_cast<float>(isize);
                const int cosine = static_cast<int>(icos__Fi(record->angle) * fsize);
                const int sine = static_cast<int>(isin__Fi(record->angle) * fsize);
                const int u_max = (width - 1) << 4;
                const int v_max = (height - 1) << 20;
                entry(cursor, 0x00100010ull, 3ull);
                entry(cursor, widen(cx - cosine * 2) | (widen(cy - sine) << 16) | z64, 13ull);
                entry(cursor, widen(u_max) | 0x100000ull, 3ull);
                entry(cursor, widen(cx + sine * 2) | (widen(cy - cosine) << 16) | z64, 13ull);
                entry(cursor, widen(v_max) | 0x10ull, 3ull);
                entry(cursor, widen(cx - sine * 2) | (widen(cy + cosine) << 16) | z64, 5ull);
                entry(cursor, widen(u_max) | widen(v_max), 3ull);
                entry(cursor, widen(cx + cosine * 2) | (widen(cy + sine) << 16) | z64, 5ull);
            } else {
                const int u_max = (width - 1) << 4;
                const int v_max = (height - 1) << 20;
                const int twice = isize * 2;
                entry(cursor, 0x00100010ull, 3ull);
                entry(cursor, widen(cx - twice) | (widen(cy - isize) << 16) | z64, 5ull);
                entry(cursor, widen(v_max) | widen(u_max), 3ull);
                entry(cursor, widen(cx + twice) | (widen(cy + isize) << 16) | z64, 5ull);
            }
        }
        entry(cursor, 0x3131757531317575ull, 68ull);
        const unsigned long long gif_count = widen(((static_cast<int>((cursor - gif_tag) >> 3)) - 2) >> 1);
        const int vif_count = (static_cast<int>(cursor - vif_word) - 4) >> 4;
        const unsigned long long gif_old = *reinterpret_cast<unsigned long long*>(gif_tag);
        const unsigned vif_old = *reinterpret_cast<unsigned*>(vif_word);
        store64(gif_tag, gif_count | gif_old);
        store32(vif_word, vif_old | static_cast<unsigned>(vif_count));
        unsigned char* tail = cursor;
        while ((reinterpret_cast<__UINTPTR_TYPE__>(tail) & 15u) != 0u) *tail++ = 0;
        cursor = tail;
        const int tag_count = ((static_cast<int>((cursor - open_tag) >> 3)) - 2) >> 1;
        unsigned long long tag_word = widen(tag_count) | 0x10000000ull;
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
        drawAddToList__FPviP8_textureP11_drawRecordi(old_list, mode == 0 ? 3 : 4, texture, 0, 0);
    }
}
