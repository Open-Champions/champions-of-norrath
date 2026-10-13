// Observed drawDistortionParticles__Fv at 0x00181758, 2464 bytes (a local function).
// Only the fourth group of globalSystem (0x0037f730, ranges 12..15 at byte offset 192, 16-byte
// ranges: first node, last node, first index, last index) is drawn. Particles are 16-byte
// records in block chains as in frameNumWrappedPS: halfwords at +0, +2, +4 position, +6 frame,
// +8, +10, +12 velocity, +14 (arithmetic shift right two) the index of a 64-byte descriptor in
// descList (0x00780300).
//  1. ResourceMap::get(&blanktex) is called first. If none of the four ranges has a first node,
//     the function returns. If fewer than 3000 list entries remain
//     (GLISTSIZE - ((glistp - gstart) >> 3)) it returns.
//  2. A packet is opened at glistp: the word at +8 is 0x11000000, the open tag is glistp, zero bytes
//     pad to (cursor & 15) == 12, a VIF word 0x50000000 is written, and a GIF tag
//     0x108ec00000008000 with the dword 14 follows. Seven A+D entries (data, register) follow:
//     (0,63) (0,6) (the frame-buffer word, 80) (97,20) (0x71001,71) (0x1212303012123030,68) (68,66).
//     The frame-buffer word is (engineDrawBufferStartAddress >> 13) | 0x140000 | 0x02000000 |
//     (0x8000 << 42) | (0x8000 << 34). The tag count is patched, and a particle budget counter
//     starts at 100.
//  3. For each non-empty range and each node in its chain (the start index is the range's first
//     index on the first node and 1 on later nodes; the end index is the range's last index on the
//     last node and 256 otherwise) the particles are walked: the index after the particle is
//     advanced by two when it is congruent to 31 modulo 32 (so 31 and 32 are never drawn). While
//     the index is below the end, the counter is decremented, and a negative old value stops the
//     walk. For each particle, age = P_FrameNum - frame; k = age * 2^-12; the position is
//     (x + vx*k, y + vy*k, z + vz*k) and is moved by matTransform(&p, worldViewMatrix, &p) whose
//     result w must be at least 20.0, with |x| <= w*700.0 and |y| <= w*300.0. The point is scaled by
//     1.0 / w. amount = int(desc[44] + desc[60] * age) must be positive. Then a GIF tag
//     0x10aec00000008000 (dword 14) and eighteen A+D entries are written whose values are listed in
//     the code, with the count patched into the tag.
//  4. A closing GIF tag 0x108ec00000008000 with one entry (0x3131757531317575, 68) is added, the first
//     VIF word gets the quadword count, the cursor is padded to 16, the open DMA tag takes the count
//     with ID 1 rewritten as 6 (or a fresh 0x60000000 tag is appended), glistp takes the cursor and
//     drawAddToList(old glistp, 2, the blanktex texture, 0, 68) is called.
// Unknown: what the descriptor fields and the register sequence do (the sequence includes a
// local-to-local transfer, so a screen-copy distortion is inferred only).
// Fresh compilation is not an original-byte match.
struct Point3 {
    float x, y, z;
};

extern "C" {
extern unsigned char globalSystem[];
extern unsigned char blanktex[];
extern unsigned char descList[];
extern unsigned char block[];
extern unsigned blockList[];
extern unsigned char wigglyTab[];
extern short P_FrameNum;
extern int engineDrawBufferStartAddress;
extern unsigned char* glistp;
extern unsigned char* gstart;
extern int GLISTSIZE;
extern unsigned char worldViewMatrix[];
void* get__11ResourceMap(void* map);
float matTransform__FP6Point3P8Matrix44T0(void* result, void* matrix, const void* point);
void* drawAddToList__FPviP8_textureP11_drawRecordi(void* packet, int list, void* texture, void* record, int flag);
}

namespace {
inline void store32(unsigned char* address, unsigned value) { *reinterpret_cast<unsigned*>(address) = value; }
inline void store64(unsigned char* address, unsigned long long value) {
    *reinterpret_cast<unsigned long long*>(address) = value;
}
inline void entry(unsigned char* at, unsigned long long data, unsigned long long reg) {
    store64(at, data);
    store64(at + 8, reg);
}
inline unsigned long long ext(long long value) { return static_cast<unsigned long long>(value); }
inline float loadf(const unsigned char* address) { return *reinterpret_cast<const float*>(address); }
inline int remaining(const unsigned char* cursor) { return GLISTSIZE - static_cast<int>((cursor - gstart) >> 3); }
}

extern "C" void drawDistortionParticles__Fv() {
    void* const texture = get__11ResourceMap(blanktex);
    int found = 0;
    for (; found < 4; ++found) {
        if (*reinterpret_cast<unsigned*>(globalSystem + 192 + 16 * found) != 0) break;
    }
    if (found == 4) return;
    if (remaining(glistp) < 3000) return;
    unsigned char* const first_base = glistp;
    unsigned char* open_tag = first_base;
    store32(first_base + 8, 0x11000000u);
    unsigned char* pad = first_base + 12;
    while ((reinterpret_cast<__UINTPTR_TYPE__>(pad) & 15u) != 12u) *pad++ = 0;
    unsigned char* cursor = pad;
    unsigned char* const vif_word = cursor;
    store32(vif_word, 0x50000000u);
    unsigned char* const first_gif = vif_word + 4;
    store64(first_gif, 0x108ec00000008000ull);
    store64(first_gif + 8, 14ull);
    entry(vif_word + 20, 0ull, 63ull);
    entry(vif_word + 36, 0ull, 6ull);
    entry(vif_word + 52,
          ext(static_cast<long long>(engineDrawBufferStartAddress >> 13)) | 0x140000ull | 0x02000000ull |
              (0x8000ull << 42) | (0x8000ull << 34),
          80ull);
    entry(vif_word + 68, 97ull, 20ull);
    entry(vif_word + 84, 0x00071001ull, 71ull);
    entry(vif_word + 100, 0x1212303012123030ull, 68ull);
    entry(vif_word + 116, 68ull, 66ull);
    cursor = vif_word + 132;
    {
        const unsigned long long old = *reinterpret_cast<unsigned long long*>(first_gif);
        store64(first_gif, ext(((static_cast<int>((cursor - first_gif) >> 3)) - 2) >> 1) | old);
    }
    unsigned char* gif_pointer = first_gif;
    int counter = 100;
    const float sixteen = 16.0f;
    for (int range_index = 0; range_index < 4; ++range_index) {
        unsigned char* const range = globalSystem + 192 + 16 * range_index;
        unsigned* const first_node = *reinterpret_cast<unsigned* const*>(range);
        if (first_node == 0) continue;
        unsigned* const last_node = *reinterpret_cast<unsigned* const*>(range + 4);
        const int first_index = *reinterpret_cast<const int*>(range + 8);
        const int last_index = *reinterpret_cast<const int*>(range + 12);
        unsigned* node = first_node;
        for (;;) {
            const int end_index = (node == last_node) ? last_index : 256;
            int slot = (node == first_node) ? first_index : 1;
            unsigned char* const block_base = block + static_cast<unsigned>(node - blockList) * 4096u;
            while (slot < end_index) {
                const int old_counter = counter;
                counter = old_counter - 1;
                if (old_counter < 0) break;
                unsigned char* const p = block_base + slot * 16;
                const int age = P_FrameNum - *reinterpret_cast<const short*>(p + 6);
                const int next = slot + 1;
                const bool skip_pair = ((next & 31) ^ 31) == 0;
                slot = skip_pair ? next + 2 : next;
                const float age_f = static_cast<float>(age);
                const float k = age_f * __builtin_bit_cast(float, 0x39800000u);
                const float vx = static_cast<float>(*reinterpret_cast<const short*>(p + 8)) * k;
                const float vz = static_cast<float>(*reinterpret_cast<const short*>(p + 12)) * k;
                const float vy = static_cast<float>(*reinterpret_cast<const short*>(p + 10)) * k;
                Point3 point;
                point.x = static_cast<float>(*reinterpret_cast<const short*>(p + 0)) + vx;
                point.z = static_cast<float>(*reinterpret_cast<const short*>(p + 4)) + vz;
                point.y = static_cast<float>(*reinterpret_cast<const short*>(p + 2)) + vy;
                const int descriptor_index = static_cast<int>(static_cast<unsigned>(*reinterpret_cast<const unsigned short*>(p + 14)) << 16) >> 18;
                const unsigned char* const desc = descList + (descriptor_index << 6);
                const float w = matTransform__FP6Point3P8Matrix44T0(&point, worldViewMatrix, &point);
                if (w < 20.0f) continue;
                if (w * 700.0f < __builtin_fabsf(point.x)) continue;
                if (w * 300.0f < __builtin_fabsf(point.y)) continue;
                const float inv = 1.0f / w;
                const float growth = loadf(desc + 60) * age_f;
                const float amount_f = loadf(desc + 44) + growth;
                point.x = point.x * inv;
                const int amount = static_cast<int>(amount_f);
                point.y = point.y * inv;
                point.z = point.z * inv;
                if (amount <= 0) continue;
                unsigned char* const t2 = cursor;
                store64(t2, 0x10aec00000008000ull);
                store64(t2 + 8, 14ull);
                const float size_f = (loadf(desc + 28) + loadf(desc + 12) * age_f);
                const float fy = point.y * sixteen;
                const float fx = point.x * sixteen;
                const float size_scaled = size_f * sixteen;
                const float angle_source = static_cast<float>(static_cast<int>(P_FrameNum) + slot * 102);
                const int alpha = amount < 129 ? amount : 128;
                const int iy0 = static_cast<int>(fy);
                const int ix0 = static_cast<int>(fx);
                const float size_final = size_scaled * inv;
                const float angle_f = angle_source * loadf(desc + 32);
                const float fz = point.z * sixteen;
                const int iy = iy0 + 0x8000;
                int size = static_cast<int>(size_final);
                const int ix = ix0 + 0x8000;
                const int angle = static_cast<int>(angle_f);
                const int iz = static_cast<int>(fz);
                if (size < 0) size = 0;
                const int wiggle_offset = (angle & 0x3f) << 3;
                if (!(size < 497)) size = 496;
                const int top = iy - size;
                const int bottom = iy + size;
                const int doubled = size * 2;
                const int wiggle_a = *reinterpret_cast<const int*>(wigglyTab + wiggle_offset);
                const int wiggle_b = *reinterpret_cast<const int*>(wigglyTab + wiggle_offset + 4);
                const int left = ix - doubled;
                const int right = ix + doubled;
                int v1 = ((top - 32768) >> 4) + 224 + wiggle_b;
                const int m = (size + 15) >> 4;
                int a0 = ((left - 32768) >> 4) + 640 + wiggle_a;
                const int m2 = m << 1;
                const int m4 = m << 2;
                const int pair_less = m2 - 1;
                if (v1 < 0) v1 = 0;
                const int limit_s0 = 446 - m2;
                if (a0 < 0) a0 = 0;
                const int limit_s1 = 1278 - m4;
                if (limit_s0 < v1) v1 = limit_s0;
                if (limit_s1 < a0) a0 = limit_s1;
                const long long z_high = static_cast<long long>(iz) << 32;
                entry(t2 + 16, ext(a0) | ext(static_cast<long long>(v1) << 16), 81ull);
                const int quad_less = m4 - 1;
                entry(t2 + 32, ext(static_cast<long long>(m2) << 32) | ext(m4), 82ull);
                const int quad_shift = quad_less << 4;
                const int pair_shift = pair_less << 4;
                const int pair_high = pair_less << 20;
                entry(t2 + 48, 2ull, 83ull);
                const int uv_u = (quad_shift + 16) >> 1;
                const int uv_v = (pair_shift + 16) << 15;
                entry(t2 + 64, 0ull, 63ull);
                entry(t2 + 80, ext(uv_u | uv_v), 3ull);
                const long long alpha_word =
                    ((static_cast<long long>(alpha) << 24) | 128ll) | 0x00808000ll;
                const int rgba = static_cast<int>(alpha_word);
                const int center_x = (left + right) >> 1;
                const int center_y = (top + bottom) >> 1;
                store32(t2 + 96, static_cast<unsigned>(rgba));
                store32(t2 + 100, 0u);
                store64(t2 + 104, 1ull);
                entry(t2 + 112, ext(center_x) | ext(static_cast<long long>(center_y) << 16) | ext(z_high), 13ull);
                store32(t2 + 128, 0x00808080u);
                store32(t2 + 132, 0u);
                store64(t2 + 136, 1ull);
                const long long top_shift = static_cast<long long>(top) << 16;
                entry(t2 + 144, 0x00100010ull, 3ull);
                const unsigned long long corner_a = ext(left) | ext(top_shift) | ext(z_high);
                entry(t2 + 160, corner_a, 13ull);
                entry(t2 + 176, ext(quad_shift) | 0x100000ull, 3ull);
                const long long bottom_shift = static_cast<long long>(bottom) << 16;
                entry(t2 + 192, ext(right) | ext(top_shift) | ext(z_high), 5ull);
                entry(t2 + 208, ext(quad_shift) | ext(pair_high), 3ull);
                entry(t2 + 224, ext(right) | ext(bottom_shift) | ext(z_high), 5ull);
                entry(t2 + 240, ext(pair_high) | 0x10ull, 3ull);
                entry(t2 + 256, ext(left) | ext(bottom_shift) | ext(z_high), 5ull);
                entry(t2 + 272, 0x00100010ull, 3ull);
                entry(t2 + 288, corner_a, 5ull);
                gif_pointer = t2;
                cursor = t2 + 304;
                const unsigned long long old = *reinterpret_cast<unsigned long long*>(t2);
                store64(t2, ext(((static_cast<int>((cursor - t2) >> 3)) - 2) >> 1) | old);
            }
            if (node == last_node) break;
            node = reinterpret_cast<unsigned*>(*node);
        }
    }
    // Closing entry.
    unsigned char* const last_gif = cursor;
    store64(last_gif, 0x108ec00000008000ull);
    store64(last_gif + 8, 14ull);
    entry(last_gif + 16, 0x3131757531317575ull, 68ull);
    cursor = last_gif + 32;
    {
        const unsigned long long old = *reinterpret_cast<unsigned long long*>(last_gif);
        store64(last_gif, ext(((static_cast<int>((cursor - last_gif) >> 3)) - 2) >> 1) | old);
    }
    {
        const unsigned old = *reinterpret_cast<unsigned*>(vif_word);
        store32(vif_word, old | static_cast<unsigned>((static_cast<int>(cursor - vif_word) - 4) >> 4));
    }
    gif_pointer = last_gif;
    unsigned char* tail = cursor;
    while ((reinterpret_cast<__UINTPTR_TYPE__>(tail) & 15u) != 0u) *tail++ = 0;
    cursor = tail;
    const int tag_count = ((static_cast<int>((cursor - open_tag) >> 3)) - 2) >> 1;
    const unsigned long long tag_word = ext(tag_count) | 0x10000000ull;
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
    drawAddToList__FPviP8_textureP11_drawRecordi(old_list, 2, texture, 0, 68);
    (void)gif_pointer;
}
