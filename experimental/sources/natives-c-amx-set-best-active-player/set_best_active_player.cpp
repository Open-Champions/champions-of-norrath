// Observed amxSetBestActivePlayer__FP5__amxPi at 0x0013c308, 60 bytes.
// The pointer at player byte 21872 is read. A null pointer, or a header
// whose word at byte 12 has bit 8 set, stores 1 in activePlayer when
// nmPlayers is 2 and stores 0 otherwise. A header whose bit 8 is clear
// stores 0. The native returns the stored word.
extern "C" unsigned char player[];
extern "C" int nmPlayers;
extern "C" int activePlayer;

namespace {
int load_int(const unsigned char* bytes) {
    int value;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    out[0] = bytes[0];
    out[1] = bytes[1];
    out[2] = bytes[2];
    out[3] = bytes[3];
    return value;
}

unsigned char* load_pointer(const unsigned char* bytes) {
    unsigned char* value = nullptr;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    for (unsigned i = 0; i < sizeof value; ++i) out[i] = bytes[i];
    return value;
}
}

extern "C" int amxSetBestActivePlayer__FP5__amxPi(void*, int*) {
    unsigned char* header = load_pointer(player + 21872);
    if (header != nullptr && (load_int(header + 12) & 8) == 0) {
        activePlayer = 0;
    } else {
        activePlayer = nmPlayers == 2 ? 1 : 0;
    }
    return activePlayer;
}
