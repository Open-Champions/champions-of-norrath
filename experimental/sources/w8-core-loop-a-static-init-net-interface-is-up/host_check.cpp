// Host check for _GLOBAL_$I$netInterfaceIsUp. Not an EE plan input; memset is the host's.
#include "static_init_net_interface_is_up.cpp"
#include <cstring>

extern "C" {
unsigned char compImmediateCompressor[80];
unsigned char compImmediateDecompressor[80];
unsigned char compLocalCompressor[80];
unsigned char compLocalDecompressor[80];
unsigned char partners[4 * 17632];
}

static bool objectOk(const unsigned char* o) {
    for (int i = 0; i < 80; ++i) {
        unsigned char want = 0;
        if (i == 11 || i == 27 || i == 43 || i == 59) want = 0x43;  // top byte of 128.0f, little endian
        if (o[i] != want) return false;
    }
    return true;
}

int main() {
    unsigned char* standalone[4] = {compImmediateCompressor, compImmediateDecompressor, compLocalCompressor,
                                    compLocalDecompressor};
    for (unsigned char* s : standalone) std::memset(s, 0x77, 80);
    std::memset(partners, 0x77, sizeof partners);
    _GLOBAL_$I$netInterfaceIsUp();
    for (unsigned char* s : standalone) if (!objectOk(s)) return 1;
    for (int i = 0; i < 4; ++i) {
        unsigned char* r = partners + i * 17632;
        for (int b = 0; b < 16; ++b) if (r[b] != 0x77) return 2;          // head of the record untouched
        if (!objectOk(r + 16) || !objectOk(r + 96)) return 3;
        for (int b = 176; b < 17632; ++b) if (r[b] != 0x77) return 4;     // everything after the two objects
    }
    // calling again from a dirty state restores the same bytes
    for (unsigned char* s : standalone) std::memset(s, 0x33, 80);
    std::memset(partners + 3 * 17632 + 96, 0x33, 80);
    _GLOBAL_$I$netInterfaceIsUp();
    for (unsigned char* s : standalone) if (!objectOk(s)) return 5;
    if (!objectOk(partners + 3 * 17632 + 96)) return 6;
    return 0;
}
