// Observed soundComputeSequenceLength__FP15AnimationHeader at 0x0024c4c0, 316 bytes.
// The header word at +56 points at 28-byte key entries that end at a first
// word above 0x7ffffffe; a null pointer returns 0. Each entry starts with an
// integer time that is converted to float and multiplied by the word
// 0x3c888888 (0x00482024). The signed halfword at +4 selects an entry of the
// 8-byte table at header+52 when it is not negative. Word +4 of that entry,
// when not null, points at a sound record. Its word +44 shifted right four
// times 28, divided by convertEnd of its word +16, is added to the time. The
// largest value is returned. The compiled code is not byte identical.
extern "C" {
int convertEnd__Fi(int value);

#if defined(NORRATH_HOST_CONTRACT)
void* norrath_host_pointer(unsigned address);
#endif
static unsigned char* pointer_at(unsigned address) {
#if defined(NORRATH_HOST_CONTRACT)
    return static_cast<unsigned char*>(norrath_host_pointer(address));
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(address));
#endif
}

static unsigned load_word(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}

static float bits(unsigned value) {
    float out;
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&out);
    bytes[0] = static_cast<unsigned char>(value);
    bytes[1] = static_cast<unsigned char>(value >> 8);
    bytes[2] = static_cast<unsigned char>(value >> 16);
    bytes[3] = static_cast<unsigned char>(value >> 24);
    return out;
}

float soundComputeSequenceLength__FP15AnimationHeader(unsigned char* header) {
    float longest = bits(0);
    unsigned char* keys = pointer_at(load_word(header + 56));
    if (keys == nullptr) return longest;
    int offset = 0;
    for (;;) {
        unsigned char* key = keys + offset;
        const int first = static_cast<int>(load_word(key));
        if (first > 0x7ffffffe) break;
        float length = static_cast<float>(first) * bits(0x3c888888u);
        const unsigned halfword = key[4] | (key[5] << 8);
        const int sound = static_cast<int>(halfword ^ 0x8000u) - 0x8000;
        if (sound >= 0) {
            unsigned char* table = pointer_at(load_word(header + 52));
            unsigned char* record = pointer_at(load_word(table + sound * 8 + 4));
            if (record != nullptr) {
                const int units = static_cast<int>(load_word(record + 44)) >> 4;
                const int size = units * 7 * 4;
                const int rate = convertEnd__Fi(static_cast<int>(load_word(record + 16)));
                length += static_cast<float>(size) / static_cast<float>(rate);
                keys = pointer_at(load_word(header + 56));
            }
        }
        if (longest < length) longest = length;
        offset += 28;
        if (static_cast<int>(load_word(keys + offset)) > 0x7ffffffe) break;
    }
    return longest;
}
}
