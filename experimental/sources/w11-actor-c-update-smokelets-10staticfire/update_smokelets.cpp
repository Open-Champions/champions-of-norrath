#include "../w11-actor-c-common/c.hpp"
// Observed updateSmokelets__10StaticFire at 0x0028f518, 208 bytes. The object s keeps a ring buffer of
// 10-byte records of five halfwords (base: the word at s+1188); halfwords at s+118 (capacity), s+120 (head)
// and s+122 (tail). Records from the tail up to, but not including, the head are visited in order
// (nothing happens when the two are equal); the index wraps to 0 on reaching the capacity. With t the
// halfword value of (int)(float at s+88) each visited record is updated:
//   h0 += h6, h2 += h8 and h4 += 7 (16-bit wraparound);
//   when the new h4, read as a signed halfword, is not below t: the tail halfword (re-read from s+122) is
//   advanced by one and set to 0 when it then equals the capacity, which retires the oldest record.
// Inferred: record fields 0, 2 and 4 are a position that moves by the velocity in fields 6 and 8 and a
// 7-per-call counter or height that is compared against the limit t; the tail advance removes records
// whose counter reached the limit (assuming the visited record is the tail one; the code advances the tail
// without checking which record it is). The ring-buffer layout matches newWind__10StaticFirei. Names
// unestablished. Nothing here ran on the EE.
extern "C" void updateSmokelets__10StaticFire(void* self) {
    using namespace w11c;
    int index = shalf(self, 122);
    if (index == shalf(self, 120)) return;
    const short limit = static_cast<short>(static_cast<int>(real(self, 88)));
    do {
        unsigned char* record = from_word(word(self, 1188)) + index * 10;
        ++index;
        const unsigned short h4 = static_cast<unsigned short>(half(record, 4) + 7u);
        const unsigned short h0 = static_cast<unsigned short>(half(record, 0) + half(record, 6));
        const unsigned short h2 = static_cast<unsigned short>(half(record, 2) + half(record, 8));
        half(record, 0) = h0;
        half(record, 2) = h2;
        half(record, 4) = h4;
        if (!(static_cast<short>(h4) < limit)) {
            const short tail = static_cast<short>(half(self, 122) + 1u);
            half(self, 122) = static_cast<unsigned short>(tail);
            if (tail == shalf(self, 118)) half(self, 122) = 0;
        }
        if (index == shalf(self, 118)) index = 0;
    } while (index != shalf(self, 120));
}
