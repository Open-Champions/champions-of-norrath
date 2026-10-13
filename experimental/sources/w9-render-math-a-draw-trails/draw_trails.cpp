// Observed drawTrails__Fv at 0x001852d0, 2404 bytes (a local function). Two stages.
// Stage 1: for each of 20 bolts (1844-byte records at bolt, 0x006f6e50), skipped when the byte at
// +1836 is zero:
//   the clock is trueNetFrameNm when the signed byte at +1838 is non-zero, else frameNm. When the
//   word at +1832 is below the clock, endLightning(bolt) is called and the bolt is done.
//   Otherwise linePos is set to 0; unless gameIsPaused() is non-zero, moveFractalLine(bolt, the
//   single at +1816, the signed byte at +1837) is called. linePos is set to 0 again. The bytes at
//   +1028 and +1029 and the halfword at +1026 of the two records pointed to by the words at +1824
//   and +1828 are cleared. With the bolt index n, eRandState is saved, set to
//   frameNm * 553 + n * 1011 (read from frameNm at that point) and drawFractalLine(bolt, start,
//   end, 1.0f, the signed byte at +1837) is called with the two Point3 copies at +1792 and +1804.
//   eRandState is then restored.
// Stage 2: a DMA packet at glistp holds the VU1 program reference and, for each of the 64 trail
// records (1040 bytes at trail, 0x00780b00) whose halfword at +1024 is non-zero and whose bytes
// at +1028 and +1029 differ, a header block and chunks of up to 32 points. The words involved
// are the observed constants below. Finally glistp takes the cursor and
// drawAddToList(old glistp, 4, ResourceMap::get(&trailTexture), 0, 0) is called.
// Unknown: what the bolts, trail fields, constants and VU1 words mean. Fresh compilation is not
// an original-byte match.
struct Point3 {
    float x, y, z;
};
struct Bolt;

extern "C" {
extern int frameNm;
extern int trueNetFrameNm;
extern int linePos;
extern int eRandState;
extern unsigned char bolt[];
extern unsigned char trail[];
extern unsigned char trailTexture[];
extern unsigned char vu1swooshstart[];
extern unsigned char vu1swooshend[];
extern unsigned char* glistp;
void endLightning__FPv(void* bolt_pointer);
int gameIsPaused__Fv();
void moveFractalLine__FP4Boltfi(Bolt* b, float value, int flag);
void drawFractalLine__FP4BoltG6Point3T1fi(Bolt* b, const Point3* start, const Point3* end, float value, int flag);
void* get__11ResourceMap(void* map);
void* drawAddToList__FPviP8_textureP11_drawRecordi(void* packet, int list, void* texture, void* record, int flag);
}

namespace {
inline void store16(unsigned char* address, unsigned value) { *reinterpret_cast<unsigned short*>(address) = static_cast<unsigned short>(value); }
inline void store32(unsigned char* address, unsigned value) { *reinterpret_cast<unsigned*>(address) = value; }
inline void store64(unsigned char* address, unsigned long long value) {
    *reinterpret_cast<unsigned long long*>(address) = value;
}
inline unsigned load8(const unsigned char* address) { return *address; }
inline int load_s8(const unsigned char* address) { return *reinterpret_cast<const signed char*>(address); }
inline int load_s16(const unsigned char* address) { return *reinterpret_cast<const short*>(address); }
inline unsigned load32(const unsigned char* address) { return *reinterpret_cast<const unsigned*>(address); }
inline float loadf(const unsigned char* address) { return *reinterpret_cast<const float*>(address); }
inline void storef(unsigned char* address, float value) { *reinterpret_cast<float*>(address) = value; }
inline unsigned long long widen(int value) { return static_cast<unsigned long long>(static_cast<long long>(value)); }
inline unsigned char* ptr_at(unsigned value) { return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(value)); }
inline void pad16(unsigned char*& cursor) {
    while ((reinterpret_cast<__UINTPTR_TYPE__>(cursor) & 15u) != 0u) *cursor++ = 0;
}
inline void close_tag(unsigned char* cursor_value, unsigned char* open_tag, unsigned char*& cursor) {
    const int count = ((static_cast<int>((cursor_value - open_tag) >> 3)) - 2) >> 1;
    const unsigned long long word = widen(count) | 0x10000000ull;
    store64(open_tag, word);
    if (((static_cast<long long>(word) >> 28) & 7) == 1) {
        store64(open_tag, (word & 0x8fffffffull) | 0x60000000ull);
    } else {
        store64(cursor + 0, 0x60000000ull);
        store64(cursor + 8, 0ull);
        cursor += 16;
    }
}
}

extern "C" void drawTrails__Fv() {
    static const float uvtab[10] = {
        __builtin_bit_cast(float, 0x00000000u), __builtin_bit_cast(float, 0x42000000u),
        __builtin_bit_cast(float, 0x42800000u), __builtin_bit_cast(float, 0x42c00000u),
        __builtin_bit_cast(float, 0x43000000u), __builtin_bit_cast(float, 0x43600000u),
        __builtin_bit_cast(float, 0x43800000u), __builtin_bit_cast(float, 0x43900000u),
        __builtin_bit_cast(float, 0x43af8000u), __builtin_bit_cast(float, 0x43df8000u)};
    for (int n = 0; n < 20; ++n) {
        unsigned char* b = bolt + n * 1844;
        if (load_s8(b + 1836) == 0) continue;
        int clock = frameNm;
        if (load_s8(b + 1838) != 0) clock = trueNetFrameNm;
        if (static_cast<int>(load32(b + 1832)) < clock) {
            endLightning__FPv(b);
            continue;
        }
        linePos = 0;
        if (gameIsPaused__Fv() == 0) {
            moveFractalLine__FP4Boltfi(reinterpret_cast<Bolt*>(b), loadf(b + 1816), load_s8(b + 1837));
        }
        unsigned char* first = ptr_at(load32(b + 1824));
        linePos = 0;
        first[1028] = 0;
        first[1029] = 0;
        unsigned char* second = ptr_at(load32(b + 1828));
        second[1028] = 0;
        second[1029] = 0;
        unsigned char* second_again = ptr_at(load32(b + 1828));
        unsigned char* first_again = ptr_at(load32(b + 1824));
        const int frame = frameNm;
        first_again[1026] = 0;
        first_again[1027] = 0;
        second_again[1026] = 0;
        second_again[1027] = 0;
        const int saved = eRandState;
        Point3 start = *reinterpret_cast<const Point3*>(b + 1792);
        Point3 end = *reinterpret_cast<const Point3*>(b + 1804);
        const int seed = frame * 553 + n * 1011;
        eRandState = seed;
        drawFractalLine__FP4BoltG6Point3T1fi(reinterpret_cast<Bolt*>(b), &start, &end, 1.0f, load_s8(b + 1837));
        eRandState = saved;
    }
    // Stage 2.
    unsigned char* cursor;
    unsigned char* const first_base = glistp;
    unsigned char* open_tag = first_base;
    store32(first_base + 8, 0x11000000u);
    unsigned char* t8 = first_base + 12;
    cursor = t8;
    while ((reinterpret_cast<__UINTPTR_TYPE__>(t8) & 15u) != 12u) *t8++ = 0;
    unsigned char* const gif = t8 + 4;
    cursor = gif;
    store64(gif, 0x1000000000008000ull);
    store64(gif + 8, 14ull);
    unsigned char* const g = gif;
    unsigned char* const end_header = g + 112;
    cursor = end_header;
    store64(g + 16 + 8, 63ull);
    store64(g + 16, 0ull);
    store64(g + 32 + 8, 6ull);
    store64(g + 32, 0ull);
    store64(g + 48 + 8, 20ull);
    store64(g + 48, 97ull);
    store64(g + 64 + 8, 8ull);
    store64(g + 64, 0x08000020000aull);
    store64(g + 80 + 8, 71ull);
    store64(g + 80, 0x00071001ull);
    store64(g + 96 + 8, 69ull);
    store64(g + 96, 0ull);
    store32(t8, static_cast<unsigned>((static_cast<int>(end_header - t8) - 4) >> 4) | 0x50000000u);
    {
        const unsigned long long old = *reinterpret_cast<unsigned long long*>(gif);
        store64(gif, widen(((static_cast<int>((end_header - gif) >> 3)) - 2) >> 1) | old);
    }
    unsigned char* aligned = end_header;
    pad16(aligned);
    cursor = aligned;
    // VU1 program reference.
    const unsigned program_start = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vu1swooshstart));
    const int program_bytes = static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(vu1swooshend) -
                                               reinterpret_cast<__UINTPTR_TYPE__>(vu1swooshstart));
    const int rounded = ((program_bytes + 15) >> 4) << 4;
    unsigned char* const header_tag = open_tag;
    open_tag = cursor;
    {
        unsigned char* here = aligned;
        const int count = ((static_cast<int>((here - header_tag) >> 3)) - 2) >> 1;
        const unsigned long long tag_word = widen(count) | 0x10000000ull;
        unsigned char* t0 = cursor;
        const int mpg_addr = 16384 - rounded;
        const unsigned mpg = static_cast<unsigned>((mpg_addr >> 3) | (rounded << 13)) | 0x4a000000u;
        store64(header_tag, tag_word);
        store64(t0, (static_cast<unsigned long long>(program_start) << 32) | static_cast<unsigned long long>(rounded >> 4) |
                       0x30000000ull);
        store32(t0 + 12, mpg);
        store32(t0 + 8, 0u);
        cursor = t0 + 16;
    }
    const unsigned word52 = 0x50ae4000u;
    const unsigned word56 = 0x00043431u;
    const unsigned word60 = 0x40aa4000u;
    const unsigned mscal_main = static_cast<unsigned>(((16384 - rounded) >> 3)) | 0x14000000u;
    int vu_toggle = 0;
    unsigned char* trail_record = trail + 0;
    for (int index = 63; index >= 0; --index, trail_record += 1040) {
        if (load_s16(trail_record + 1024) == 0) continue;
        if (load8(trail_record + 1028) == load8(trail_record + 1029)) continue;
        unsigned char* const c0 = cursor;
        open_tag = c0;
        unsigned char* v = c0 + 8;
        store32(v + 0, 0u);
        store32(v + 4, 0u);
        store32(v + 8, 0u);
        store32(v + 12, 0x6c060000u);
        unsigned char* a0 = v + 16;
        cursor = a0;
        if ((load8(trail_record + 1031) & 2u) != 0) {
            store32(a0, 0x801eu);
            cursor = v + 32;
            store32(v + 20, word52);
            store32(v + 28, 0u);
            store32(v + 24, word56);
        } else {
            store32(a0, 0x801eu);
            cursor = v + 32;
            store32(v + 24, 17219u);
            store32(v + 20, word60);
            store32(v + 28, 0u);
        }
        unsigned char* c1 = cursor;
        store32(c1, 1u);
        store32(c1 + 4, 0x20000000u);
        store32(c1 + 8, 225u);
        store32(c1 + 12, 0u);
        cursor = c1 + 16;
        cursor = cursor - 4;
        const int uv_index = load_s8(trail_record + 1030);
        const float uv0 = uvtab[uv_index];
        const float uv1 = uvtab[uv_index + 1];
        const float uv_delta = (uv1 - uv0) - 1.0f;
        unsigned char* p = cursor;
        storef(p, uv_delta);
        p += 4;
        unsigned char* const a1 = p + 16;
        store32(p + 0, load8(trail_record + 1034));
        store32(p + 4, load8(trail_record + 1035));
        store32(p + 8, load8(trail_record + 1036));
        store32(p + 12, load8(trail_record + 1037));
        cursor = a1;
        const unsigned flags = load8(trail_record + 1031);
        if ((flags & 8u) != 0) {
            store64(a1, 9ull);
            store64(a1 + 8, 66ull);
        } else if ((flags & 4u) != 0) {
            store64(a1 + 8, 66ull);
            store64(a1, 72ull);
        } else {
            store64(a1, 68ull);
            store64(a1 + 8, 66ull);
        }
        cursor = p + 32;
        p = cursor;
        storef(p, __builtin_bit_cast(float, 0x49000000u));
        p += 4;
        const float width_scaled = static_cast<float>(load_s16(trail_record + 1032));
        const int uv_index2 = load_s8(trail_record + 1030);
        unsigned char* const next = p + 32;
        const float constant_a = __builtin_bit_cast(float, 0x49000008u);
        const int half_1026 = load_s16(trail_record + 1026);
        const float ratio = __builtin_bit_cast(float, 0x43000000u) / width_scaled;
        const float uv_base = uvtab[uv_index2];
        const float half_float = static_cast<float>(half_1026);
        cursor = next;
        unsigned char* t0 = next;
        const float uv_final = uv_base + constant_a;
        storef(p, uv_final);
        storef(p + 4, ratio);
        const unsigned count_byte = load8(trail_record + 1039);
        store32(p + 12, 0u);
        store32(p + 16, 0u);
        storef(p + 8, static_cast<float>(static_cast<int>(count_byte * 40u)));
        const unsigned ring_bit = load8(trail_record + 1028) & 1u;
        storef(p + 24, half_float);
        store32(p + 28, 0x14000028u);
        store32(p + 20, ring_bit + 1u);
        pad16(t0);
        cursor = t0;
        {
            const int count = ((static_cast<int>((cursor - open_tag) >> 3)) - 2) >> 1;
            store64(open_tag, widen(count) | 0x10000000ull);
        }
        unsigned t3 = load8(trail_record + 1028) & 0xfeu;
        int remaining = static_cast<int>(load8(trail_record + 1029) - t3) & 0x7f;
        if (remaining <= 0) continue;
        do {
            int chunk = (remaining < 33) ? remaining : 32;
            const int limit = 128 - static_cast<int>(t3);
            if (limit < chunk) chunk = limit;
            unsigned char* const t0b = cursor;
            open_tag = t0b;
            const int even = (chunk + 1) & -2;
            int imm = vu_toggle - chunk + 128;
            const unsigned vif_upload = static_cast<unsigned>(imm) | 0x60000000u | (static_cast<unsigned>(even) << 16) | 0x01000000u |
                                        0x0c000000u;
            const unsigned vif_itop = static_cast<unsigned>(imm) | 0x04000000u;
            const unsigned data_address = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(trail_record)) + t3 * 8u;
            const unsigned long long ref = (static_cast<unsigned long long>(data_address) << 32) |
                                           static_cast<unsigned long long>((even * 8) >> 4) | 0x30000000ull;
            unsigned char* const dst = cursor;
            store32(dst + 12, vif_upload);
            store64(t0b, ref);
            store32(dst + 8, vif_itop);
            remaining -= chunk;
            cursor = dst + 16;
            t3 = (t3 + static_cast<unsigned>(chunk)) & 0x7fu;
            const bool second_flag = (load8(trail_record + 1031) & 2u) != 0;
            vu_toggle ^= 0x200;
            open_tag = cursor;
            unsigned char* const e = cursor;
            cursor = e + 16;
            store64(e, 0x10000000ull);
            store64(e + 8, second_flag ? static_cast<unsigned long long>(mscal_main) : 0x14000026ull);
        } while (remaining > 0);
    }
    // Closing packet.
    open_tag = cursor;
    unsigned char* head = cursor + 8;
    unsigned char* v1p = head + 4;
    store32(head, 0x10000000u);
    cursor = v1p;
    unsigned char* t4 = v1p;
    while ((reinterpret_cast<__UINTPTR_TYPE__>(t4) & 15u) != 12u) *t4++ = 0;
    unsigned char* const gif2 = t4 + 4;
    cursor = gif2;
    store64(gif2, 0x1000000000008000ull);
    store64(gif2 + 8, 14ull);
    unsigned char* const g2 = cursor;
    unsigned char* const end2 = g2 + 48;
    cursor = end2;
    store64(g2 + 16 + 8, 8ull);
    store64(g2 + 16, 0ull);
    store64(g2 + 32 + 8, 69ull);
    store64(g2 + 32, 1ull);
    const unsigned vif2 = static_cast<unsigned>((static_cast<int>(end2 - t4) - 4) >> 4) | 0x50000000u;
    store32(t4, vif2);
    {
        const unsigned long long old = *reinterpret_cast<unsigned long long*>(gif2);
        store64(gif2, widen(((static_cast<int>((end2 - gif2) >> 3)) - 2) >> 1) | old);
    }
    unsigned char* tail = end2;
    pad16(tail);
    cursor = tail;
    close_tag(cursor, open_tag, cursor);
    unsigned char* const old_list = glistp;
    glistp = cursor;
    void* texture = get__11ResourceMap(trailTexture);
    drawAddToList__FPviP8_textureP11_drawRecordi(old_list, 4, texture, 0, 0);
}
