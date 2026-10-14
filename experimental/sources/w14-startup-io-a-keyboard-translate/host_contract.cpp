#include "translate.cpp"
#include <cstdlib>
#include <cstring>

unsigned char USEnglishKbdMapTable[10 * 80];

int main() {
    srand(11);
    for (int round = 0; round < 40; ++round) {
        for (int i = 0; i < 800; ++i) USEnglishKbdMapTable[i] = static_cast<unsigned char>(rand());
        // Make byte 1 of each record distinct for the first 74 records so the first match is unambiguous,
        // and make record 20 duplicate record 5's code on odd rounds, so code 6 has two matches and
        // the earlier record must win (and code 21 has none).
        for (int i = 0; i < 80; ++i) USEnglishKbdMapTable[i * 10 + 1] = static_cast<unsigned char>(i + 1);
        if (round & 1) USEnglishKbdMapTable[20 * 10 + 1] = USEnglishKbdMapTable[5 * 10 + 1];
        for (int i = 0; i < 80; ++i) {
            for (int a = 0; a < 4; ++a) {
                for (int b = 0; b < 8; ++b) {
                    alignas(4) unsigned char event[16];
                    std::memset(event, 0x77, sizeof event);
                    unsigned short code = static_cast<unsigned short>(i + 1);
                    unsigned short flags_a = a == 0 ? 0 : (a == 1 ? 2 : (a == 2 ? 0xfffd : 0xffff));
                    unsigned short flags_b = b == 0 ? 0 : (b == 1 ? 0x02 : (b == 2 ? 0x20 : (b == 3 ? 0x40 : (b == 4 ? 0x42 : (b == 5 ? 0x22 : (b == 6 ? 0x1000 : 0xffff))))));
                    std::memcpy(event + 4, &code, 2);
                    std::memcpy(event + 8, &flags_a, 2);
                    std::memcpy(event + 10, &flags_b, 2);
                    int first = i;
                    unsigned char want;
                    if (i >= 74 || ((round & 1) && i == 20)) want = 0;
                    else {
                        const unsigned char* rec = USEnglishKbdMapTable + first * 10;
                        want = (flags_b & 0x22) ? rec[4] : ((flags_b & 0x40) ? rec[6] : rec[2]);
                        if ((flags_a & 2) && want >= 97 && want <= 122) want -= 32;
                    }
                    if (TranslateEvent__10VIKeyboardP11_lgKbdEvent(0, event) != want) return 1;
                }
            }
        }
        unsigned char event[16] = {};
        unsigned short none = 0x1234;
        std::memcpy(event + 4, &none, 2);
        if (TranslateEvent__10VIKeyboardP11_lgKbdEvent(0, event) != 0) return 2;
    }
    return 0;
}
