// animInit__Fv at 0x001629f0, 508 bytes, read from the pinned SLUS-20565 image.
// Fresh compilation is not an original-byte match. No animation has run.
//
// frameStates at 0x006162c0 is 150 records of 3868 bytes. memset clears
// 150 * 3868 bytes; the symbol continues 24 bytes past that, and those bytes
// are not written. Word +12 of records 0..148 stores the next record. Word +16
// of records 1..148 stores the previous record. Word +16 of record 149 stores
// record 148 plus 0x40. fsList receives frameStates. Record 0's word +16 and
// record 149's word +12 stay the memset zero.
//
// animResults at 0x006a3d40 is 100 records of 1568 bytes, fully cleared.
// Word +1556 of records 0..98 stores the next record. Word +1560 of records
// 1..99 stores the previous record. arList receives animResults. arListBack
// receives the last record.
//
// identityResult + 4 receives 0x6dc18000 and the halfword at +8 receives 64.
// From identityResult + 16, 64 groups of 12 halfwords store 4096 at halfword
// indexes 0, 5, and 10. powf is then called for exponents -64 through 63 with
// the .lit4 base at 0x0047ff80, bits 0x3f879c7c. Results are stored at
// table + 8408. glistp is loaded in the image and only spilled to the stack;
// that spill is not represented.
extern "C" {
void* memset(void* destination, int value, unsigned count);
float powf(float base, float exponent);

extern unsigned char frameStates[];
extern unsigned char animResults[];
extern unsigned char identityResult[];
extern unsigned char table[];
extern unsigned char* fsList;
extern unsigned char* arList;
extern unsigned char* arListBack;

static void norrath_store_pointer(void* slot, const void* value) {
    const auto bits = static_cast<unsigned>(reinterpret_cast<unsigned long>(value));
    auto* dest = static_cast<unsigned char*>(slot);
    dest[0] = static_cast<unsigned char>(bits);
    dest[1] = static_cast<unsigned char>(bits >> 8);
    dest[2] = static_cast<unsigned char>(bits >> 16);
    dest[3] = static_cast<unsigned char>(bits >> 24);
}

static void norrath_store_u32(void* slot, unsigned value) {
    auto* dest = static_cast<unsigned char*>(slot);
    dest[0] = static_cast<unsigned char>(value);
    dest[1] = static_cast<unsigned char>(value >> 8);
    dest[2] = static_cast<unsigned char>(value >> 16);
    dest[3] = static_cast<unsigned char>(value >> 24);
}

static float norrath_bits_float(unsigned bits) {
    union {
        float value;
        unsigned bits;
    } converted;
    converted.bits = bits;
    return converted.value;
}

static unsigned norrath_float_bits(float value) {
    union {
        float value;
        unsigned bits;
    } converted;
    converted.value = value;
    return converted.bits;
}

void animInit__Fv() {
    constexpr int kFrameStride = 3868;
    constexpr int kFrameRecords = 150;
    constexpr int kResultStride = 1568;
    constexpr int kResultRecords = 100;
    memset(frameStates, 0, static_cast<unsigned>(kFrameRecords * kFrameStride));
    fsList = frameStates;
    for (int index = 0; index < kFrameRecords - 1; ++index) {
        norrath_store_pointer(frameStates + index * kFrameStride + 12,
                              frameStates + (index + 1) * kFrameStride);
    }
    for (int index = 1; index < kFrameRecords - 1; ++index) {
        norrath_store_pointer(frameStates + index * kFrameStride + 16,
                              frameStates + (index - 1) * kFrameStride);
    }
    norrath_store_pointer(frameStates + (kFrameRecords - 1) * kFrameStride + 16,
                          frameStates + (kFrameRecords - 2) * kFrameStride + 0x40);

    memset(animResults, 0, static_cast<unsigned>(kResultRecords * kResultStride));
    arList = animResults;
    for (int index = 0; index < kResultRecords - 1; ++index) {
        norrath_store_pointer(animResults + index * kResultStride + 1556,
                              animResults + (index + 1) * kResultStride);
    }
    for (int index = 1; index < kResultRecords; ++index) {
        norrath_store_pointer(animResults + index * kResultStride + 1560,
                              animResults + (index - 1) * kResultStride);
    }
    arListBack = animResults + (kResultRecords - 1) * kResultStride;

    norrath_store_u32(identityResult + 4, 0x6dc18000u);
    identityResult[8] = 64;
    identityResult[9] = 0;
    for (int group = 0; group < 64; ++group) {
        auto* half = reinterpret_cast<unsigned short*>(identityResult + 16 + group * 24);
        half[0] = 4096;
        half[5] = 4096;
        half[10] = 4096;
    }

    const float base = norrath_bits_float(0x3f879c7cu);
    for (int exponent = -64; exponent < 64; ++exponent) {
        const float value = powf(base, static_cast<float>(exponent));
        norrath_store_u32(table + 8408 + (exponent + 64) * 4, norrath_float_bits(value));
    }
}
}
