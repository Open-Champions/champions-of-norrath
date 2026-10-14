// Observed P_DrawParticles__FP8Matrix34 at 0x00186070, 2556 bytes. One argument: the camera Matrix34.
// Everything it writes to the display list is a literal 32-bit address (glistp is a 32-bit word).
// In order:
//  1. objectDrawEnable (0x00483ac8, signed byte) zero returns. A zero pushLevel (0x00484304) calls drawTrails__Fv.
//     If gameIsPaused__Fv is zero, the 32 descriptor records of descList (0x00780300, 64 bytes each) whose
//     descListMark word is not 0xffffffff are scrolled: kind = signed word at +4 shifted right by 9; the record is
//     scrolled when kind is 4, or, on an even frameNm, when kind is 1 or 2. Scrolling adds 512 to the words at +0 and
//     +16; if the new word at +0 is 8192 or more (signed), 7680 is subtracted from both original words instead.
//     A zero pushLevel then calls drawSingleParticles__Fi(1).
//  2. matMakeViewMatrix__FP8Matrix44fffffff builds a view matrix with float arguments 0x3f7fffff, 0x3eeeeeee,
//     worldScrz, 1.0f, 0x457fff00, 18.0f, 1800.0f. Elements 0, 5 and 14 are multiplied by 0x3aa3d70a, 0x3b23d70a
//     and 0x3a000080; element 10 is multiplied by 0x3a000080 and 1.0f is subtracted.
//  3. A display-list "packet" is a cursor, a tag start and an auxiliary word (three words, passed by address to
//     drawPS__FR6PacketP14ParticleSystem). Segments begin with an 8-byte DMA tag whose first word is filled in
//     later as ((end - start) / 8 - 2) / 2 ORed with 0x10000000, and a VIF word at +8. The first segment is
//     built at glistp: word +8 is 0x11000000; padding bytes of zero move the cursor to 12 modulo 16; a DIRECT
//     word 0x50000006 and a GIF block (tag, then register/data pairs, ending in the words 0x13000000 and
//     0x6c8ac000) follow, then the 16 floats of camera x view (matMul__FP8Matrix44P8Matrix34T0), the integer 32,
//     the frame number as a float, and constants. A REF tag (descList address in the high word, 0x30000080) streams
//     the 128 qwords of descList. A new segment tag follows with MSCAL word 0x14000014.
//  4. drawPS runs for globalSystem (0x0037f730), then for each of the 16 PS records (0x006f5ad0, stride 312) with a
//     nonzero word at +304: the 16 floats of (record matrix at +256 x camera) x view are written after a tag with
//     UNPACK word 0x6c04c000, then MSCAL 0x1400004a and 0x10000000, padded to 16 bytes, and drawPS is called.
//     A final segment holds camera x view the same way.
//  5. When the display list has fewer than 3000 slots left (GLISTSIZE - ((glistp - gstart) >> 3)) the function
//     returns. Otherwise a DIRECT 0x50000003 GIF block follows, the last tag is converted to a RET tag (ID 6), the
//     new cursor is stored in glistp and the old glistp is registered with drawAddToList__FPviP8_textureP11_drawRecordi
//     (list 4, texture from particletex).
//  6. With room left: pushLevel zero calls drawSingleParticles__Fi(0); nmSingle is cleared; with room still left,
//     drawLines__Fv runs, and with 6000 slots left drawDistortionParticles__Fv.
// Constants whose meaning is not established (the VIF/GIF words and float bit patterns) are given as written in the
// image. Fresh compilation is not an original-byte match.
extern "C" {
extern signed char objectDrawEnable;
extern unsigned char pushLevel[];
extern unsigned frameNm;
extern unsigned glistp;
extern unsigned gstart;
extern unsigned GLISTSIZE;
extern unsigned nmSingle;
extern unsigned char descListMark[];
extern unsigned char descList[];
extern unsigned char PS[];
extern unsigned char globalSystem[];
extern unsigned char particletex[];
extern unsigned short P_FrameNum;
extern float worldScrz;

void drawTrails__Fv();
int gameIsPaused__Fv();
void drawSingleParticles__Fi(int pass);
void matMakeViewMatrix__FP8Matrix44fffffff(float* matrix, float a, float b, float c, float d, float e, float f, float g);
void matMul__FP8Matrix44P8Matrix34T0(float* out, const void* matrix34, const float* matrix44);
void matMul__FP8Matrix34N20(float* out, const void* left, const void* right);
void drawPS__FR6PacketP14ParticleSystem(void* packet, void* system);
void* norrath_player_resource_map_get(void* map);
void* drawAddToList__FPviP8_textureP11_drawRecordi(void* list, int kind, void* texture, void* record, int flag);
void drawLines__Fv();
void drawDistortionParticles__Fv();
}

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void* norrath_host_pointer(unsigned address);
#endif

namespace {
using Address = __UINTPTR_TYPE__;
unsigned low32(const void* p) { return static_cast<unsigned>(reinterpret_cast<Address>(p)); }
unsigned char* bytes(unsigned a) {
#if defined(NORRATH_HOST_CONTRACT)
    return static_cast<unsigned char*>(norrath_host_pointer(a));
#else
    return reinterpret_cast<unsigned char*>(static_cast<Address>(a));
#endif
}
unsigned get32(unsigned a) {
    const unsigned char* p = bytes(a);
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void put32(unsigned a, unsigned v) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(v);
    p[1] = static_cast<unsigned char>(v >> 8);
    p[2] = static_cast<unsigned char>(v >> 16);
    p[3] = static_cast<unsigned char>(v >> 24);
}
void put64(unsigned a, unsigned long long v) {
    put32(a, static_cast<unsigned>(v));
    put32(a + 4u, static_cast<unsigned>(v >> 32));
}
unsigned long long get64(unsigned a) {
    return static_cast<unsigned long long>(get32(a)) | (static_cast<unsigned long long>(get32(a + 4u)) << 32);
}
float bits(unsigned value) { return __builtin_bit_cast(float, value); }
unsigned float_bits(float value) { return __builtin_bit_cast(unsigned, value); }

void put8(unsigned a, unsigned char v) { *bytes(a) = v; }

// Zero bytes until the address is congruent to `wanted` modulo 16; returns the new address.
unsigned pad_to(unsigned address, unsigned wanted) {
    while ((address & 15u) != wanted) {
        put8(address, 0u);
        ++address;
    }
    return address;
}

// Closes a segment: its tag word becomes ((end - start) / 8 - 2) / 2 ORed with the given ID bits.
unsigned long long tag_value(unsigned start, unsigned end, unsigned id_bits) {
    const int words = (static_cast<int>(end - start) >> 3) - 2;
    return static_cast<unsigned long long>(static_cast<unsigned>(words >> 1) | id_bits);
}

bool low_on_room(int limit) {
    const int used = static_cast<int>(glistp - gstart) >> 3;
    return static_cast<int>(GLISTSIZE) - used < limit;
}

// One particle-system block: UNPACK word, 16 matrix floats, MSCAL words, padded to 16 bytes.
// `packet[0]` is the cursor and `packet[1]` the tag start; both are updated.
void write_matrix_block(unsigned* packet, const float* m) {
    const unsigned c = packet[0];
    put32(c + 8u, 0x6c04c000u);
    for (unsigned n = 0u; n < 16u; ++n) put32(c + 12u + n * 4u, float_bits(m[n]));
    put32(c + 76u, 0x1400004au);
    put32(c + 80u, 0x10000000u);
    packet[1] = c;
    packet[0] = pad_to(c + 84u, 0u);
}
}

extern "C" void P_DrawParticles__FP8Matrix34(void* camera) {
    if (objectDrawEnable == 0) return;
    unsigned* push_level = reinterpret_cast<unsigned*>(pushLevel);
    if (*push_level == 0u) drawTrails__Fv();
    if (gameIsPaused__Fv() == 0) {
        const bool even = ((frameNm ^ 1u) & 1u) != 0u;
        for (unsigned index = 0u; index < 32u; ++index) {
            const unsigned record = low32(descList) + index * 64u;
            if (get32(low32(descListMark) + index * 4u) == 0xffffffffu) continue;
            const int kind = static_cast<int>(get32(record + 4u)) >> 9;
            bool scroll = (kind == 4);
            if (even && static_cast<unsigned>(kind - 1) < 2u) scroll = true;
            if (!scroll) continue;
            unsigned first = get32(record);
            unsigned second = get32(record + 16u);
            const unsigned wrapped_first = first - 7680u;
            const unsigned wrapped_second = second - 7680u;
            first += 512u;
            second += 512u;
            put32(record, first);
            put32(record + 16u, second);
            if (!(static_cast<int>(first) < 8192)) {
                put32(record + 16u, wrapped_second);
                put32(record, wrapped_first);
            }
        }
    }
    if (*push_level == 0u) drawSingleParticles__Fi(1);

    float view[16];
    unsigned packet[3];
    packet[0] = glistp;
    matMakeViewMatrix__FP8Matrix44fffffff(view, bits(0x3f7fffffu), bits(0x3eeeeeeeu), worldScrz, 1.0f,
                                          bits(0x457fff00u), 18.0f, 1800.0f);
    view[10] = view[10] * bits(0x3a000080u) - 1.0f;
    view[0] = view[0] * bits(0x3aa3d70au);
    view[14] = view[14] * bits(0x3a000080u);
    view[5] = view[5] * bits(0x3b23d70au);

    // First segment, built at the display-list start.
    const unsigned start = packet[0];
    unsigned cursor = start + 12u;
    put32(start + 8u, 0x11000000u);
    packet[1] = start;
    packet[0] = cursor;
    cursor = pad_to(cursor, 12u);
    unsigned block = cursor + 4u;
    put32(cursor, 0x50000006u);
    put64(block, 0x1000000000008000ull);
    put64(block + 8u, 14u);
    packet[2] = block;
    put64(block + 16u, 0u);
    put64(block + 24u, 63u);
    put64(block + 32u, 0u);
    put64(block + 40u, 6u);
    put64(block + 48u, 1u);
    put64(block + 56u, 20u);
    put64(block + 64u, 0x71001ull);
    put64(block + 72u, 71u);
    put64(block + 80u, 0x1212303012123030ull);
    put64(block + 88u, 68u);
    put32(block + 96u, 0x13000000u);
    put32(block + 100u, 0x6c8ac000u);
    put64(block, get64(block) | 5u);
    packet[0] = block + 104u;

    float camera_view[16];
    matMul__FP8Matrix44P8Matrix34T0(camera_view, camera, view);
    const unsigned data = packet[0];
    for (unsigned n = 0u; n < 16u; ++n) put32(data + n * 4u, float_bits(camera_view[n]));
    put32(data + 64u, 32u);
    put32(data + 68u, 0u);
    put32(data + 72u, 0u);
    const unsigned segment_end = data + 152u;
    put32(data + 76u, float_bits(static_cast<float>(static_cast<short>(P_FrameNum))));
    put32(data + 80u, 0x44480000u);
    put64(start, tag_value(start, segment_end, 0x10000000u));
    put64(segment_end, (static_cast<unsigned long long>(low32(descList)) << 32) | 0x30000080ull);
    put64(segment_end + 8u, 0u);
    packet[1] = data + 168u;
    put32(data + 84u, 0x43c80000u);
    put32(data + 88u, 0x5dffff00u);
    put32(data + 92u, 0x3f800000u);
    put32(data + 96u, 0x49008000u);
    put32(data + 100u, 0x49008000u);
    put32(data + 104u, 0x6007fff8u);
    put32(data + 112u, 0x801eu);
    put32(data + 116u, 0x50ab4000u);
    put32(data + 120u, 0x00043431u);
    put32(data + 128u, 0x801eu);
    put32(data + 132u, 0x4024c000u);
    put32(data + 136u, 0x4141u);
    put32(data + 176u, 0x14000014u);
    put32(data + 180u, 0x10000000u);
    put32(data + 108u, 0u);
    put32(data + 124u, 0u);
    put32(data + 140u, 0u);
    put32(data + 144u, 0u);
    put32(data + 148u, 0u);
    packet[0] = data + 184u;
    packet[0] = pad_to(data + 184u, 0u);

    put64(packet[1], tag_value(packet[1], packet[0], 0x10000000u));
    drawPS__FR6PacketP14ParticleSystem(packet, globalSystem);

    for (unsigned index = 0u; index < 16u; ++index) {
        const unsigned record = low32(PS) + index * 312u;
        if (get32(record + 304u) == 0u) continue;
        float system_matrix[12];
        float system_view[16];
        matMul__FP8Matrix34N20(system_matrix, bytes(record + 256u), camera);
        matMul__FP8Matrix44P8Matrix34T0(system_view, system_matrix, view);
        write_matrix_block(packet, system_view);
        put64(packet[1], tag_value(packet[1], packet[0], 0x10000000u));
        drawPS__FR6PacketP14ParticleSystem(packet, reinterpret_cast<void*>(static_cast<Address>(record)));
    }

    float final_view[16];
    matMul__FP8Matrix44P8Matrix34T0(final_view, camera, view);
    write_matrix_block(packet, final_view);
    put64(packet[1], tag_value(packet[1], packet[0], 0x10000000u));
    if (low_on_room(3000)) return;

    // Closing segment: a DIRECT block of three qwords, then the last tag becomes RET.
    unsigned tail = packet[0];
    unsigned tag_start = tail;
    unsigned pos = tail + 8u;
    packet[1] = tail;
    packet[0] = pos;
    pos = pad_to(pos, 12u);
    packet[0] = pos;
    const unsigned direct = pos;
    unsigned gif = direct + 4u;
    put32(direct, 0x50000003u);
    put64(gif, 0x1000000000008000ull);
    put64(gif + 8u, 14u);
    packet[2] = gif;
    put64(gif + 16u, 0x3131757531317575ull);
    put64(gif + 24u, 68u);
    put64(gif + 32u, 97u);
    put64(gif + 40u, 20u);
    const unsigned after = gif + 48u;
    packet[0] = after;
    put64(gif, get64(gif) | 2u);
    unsigned aligned = pad_to(after, 0u);
    packet[0] = aligned;
    unsigned long long final_tag = tag_value(tag_start, aligned, 0x10000000u);
    put64(tag_start, final_tag);
    unsigned new_cursor = aligned;
    if (((final_tag >> 28) & 7u) == 1u) {
        put64(tag_start, (final_tag & 0x8fffffffull) | 0x60000000ull);
    } else {
        put64(new_cursor + 8u, 0u);
        put64(new_cursor, 0x60000000ull);
        new_cursor += 16u;
        packet[1] = 0u;
        packet[0] = new_cursor;
    }
    const unsigned list = glistp;
    glistp = new_cursor;
    void* texture = norrath_player_resource_map_get(particletex);
    drawAddToList__FPviP8_textureP11_drawRecordi(reinterpret_cast<void*>(static_cast<Address>(list)), 4, texture,
                                                 nullptr, 0);
    if (low_on_room(3000)) {
        nmSingle = 0u;
        return;
    }
    if (*push_level == 0u) drawSingleParticles__Fi(0);
    nmSingle = 0u;
    if (low_on_room(3000)) return;
    drawLines__Fv();
    if (low_on_room(6000)) return;
    drawDistortionParticles__Fv();
}
