// Observed soundAddSound__FPcP10_vagHeaderb at 0x0024c2a8, 536 bytes.
// convertEnd of the word at +12 is stored at +44, and getStockPitch at +32.
// Existing soundList names are compared with strcasecmp. A match whose entry
// word at +4 is nonzero copies that object's halfword +40 and word +36, then
// returns. Otherwise a nonzero third argument returns. A zero argument aligns
// the next address up to 64 and, when it fits under 0x00200000 minus
// effectsBufferSize, stores it, scans 16-byte groups from +48 for bit 2, and
// calls transferVAG. The list entry and nmSounds are updated on both the fit
// and the 20480 fallback.
extern "C" {
int nmSounds = 0;
int effectsBufferSize = 0;
unsigned char soundList[0x3000];
int convertEnd__Fi(int value);
int getStockPitch__FP10_vagHeader(void* header);
int strcasecmp(const char* left, const char* right);
void transferVAG__FP10_vagHeaderi(void* header, int address);
#if defined(NORRATH_HOST_CONTRACT)
unsigned char* norrath_sony_cd_resolve_sound(unsigned bits);
#endif
static unsigned load_word(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) | (static_cast<unsigned>(slot[3]) << 24);
}
static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
static void store_half(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
}
static unsigned pointer_bits(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
static unsigned char* object_from(unsigned bits) {
#if defined(NORRATH_HOST_CONTRACT)
    return norrath_sony_cd_resolve_sound(bits);
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
static const char* name_from(unsigned bits) {
    return reinterpret_cast<const char*>(object_from(bits));
}
void soundAddSound__FPcP10_vagHeaderb(char* name, void* payload, int allow) {
    auto* header = static_cast<unsigned char*>(payload);
    const int converted = convertEnd__Fi(static_cast<int>(load_word(header + 12)));
    store_word(header + 44, static_cast<unsigned>(converted));
    store_word(header + 32, static_cast<unsigned>(getStockPitch__FP10_vagHeader(header)));
    unsigned matched = 0;
    int found = 0;
    if (nmSounds > 0) {
        int index = 0;
        while (index < nmSounds) {
            const unsigned entry = static_cast<unsigned>(index) * 16u;
            const int compared = strcasecmp(name_from(load_word(soundList + entry)), name);
            index += 1;
            if (compared == 0) {
                matched = load_word(soundList + entry + 4u);
                found = 1;
                break;
            }
        }
    }
    if (found != 0 && matched != 0u) {
        unsigned char* previous = object_from(matched);
        const unsigned half = static_cast<unsigned>(previous[40]) | (static_cast<unsigned>(previous[41]) << 8);
        store_half(header + 40, half);
        store_word(header + 36, load_word(previous + 36));
        return;
    }
    if (allow != 0) return;
    unsigned address = nmSounds == 0 ? 0x8010u : load_word(soundList + static_cast<unsigned>(nmSounds) * 16u - 8u);
    address = (address + 63u) & ~63u;
    const unsigned limit = 0x00200000u - static_cast<unsigned>(effectsBufferSize);
    if (address + static_cast<unsigned>(converted) + 1024u < limit) {
        store_word(header + 36, address);
        int marker = -1;
        if (converted > 0) {
            if ((header[49] & 4u) != 0u) marker = 0;
            else {
                for (int cursor = 16; cursor < converted; cursor += 16) {
                    if ((header[48 + cursor + 1] & 4u) != 0u) {
                        marker = cursor;
                        break;
                    }
                }
            }
        }
        if (marker >= 0 && marker < converted - 16) store_half(header + 40, 1u);
        else store_half(header + 40, 0u);
        transferVAG__FP10_vagHeaderi(header, static_cast<int>(address));
    } else {
        store_half(header + 40, 0u);
        store_word(header + 36, 20480u);
    }
    const unsigned slot = static_cast<unsigned>(nmSounds) * 16u;
    const int end = static_cast<int>(load_word(header + 44));
    store_word(soundList + slot + 4u, pointer_bits(header));
    store_word(soundList + slot + 8u, address + static_cast<unsigned>(end));
    store_word(soundList + slot, pointer_bits(name));
    soundList[slot + 12u] = 0;
    nmSounds += 1;
}
}
