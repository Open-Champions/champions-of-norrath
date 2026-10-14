// Observed gameSetCameraShake at 0x001d0a88, 212 bytes.
// The first argument is the float in f12. gameCameraShake is replaced only
// when it compares less than that float. The mangled second parameter is the
// bool. It is tested as a GPR word: any nonzero word returns after the
// replacement above. A zero word walks nmPlayers records. The float at byte
// 172 and the word at byte 21872 are read. A zero word, or a float that is
// not greater than 0, skips that record and does not reload nmPlayers. A hit
// calls padBigRumble with the record index, the incoming float times the
// 0x40400000 constant times the .lit4 word 0x3c888888 at 0x00480688, and the
// 0x3f800000 constant. nmPlayers is loaded again after that call.
extern "C" float gameCameraShake;
extern "C" int nmPlayers;
extern "C" unsigned char player[];
extern "C" void padBigRumble__Fiff(int index, float amount, float second);

namespace {
unsigned load_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}

float load_f32(const unsigned char* address) {
    union {
        unsigned as_bits;
        float as_float;
    } value;
    value.as_bits = load_u32(address);
    return value.as_float;
}

float bits_f32(unsigned bits) {
    union {
        unsigned as_bits;
        float as_float;
    } value;
    value.as_bits = bits;
    return value.as_float;
}
}

extern "C" void gameSetCameraShake__Ffb(float amount, int skip) {
    if (gameCameraShake < amount) gameCameraShake = amount;
    if (skip != 0) return;
    int remaining = nmPlayers;
    if (remaining <= 0) return;
    int index = 0;
    while (index < remaining) {
        const unsigned char* record = player + static_cast<unsigned>(index) * 23984u;
        if (load_u32(record + 21872) != 0u && 0.0f < load_f32(record + 172)) {
            padBigRumble__Fiff(index, amount * bits_f32(0x40400000u) * bits_f32(0x3c888888u),
                               bits_f32(0x3f800000u));
            remaining = nmPlayers;
        }
        ++index;
    }
}
