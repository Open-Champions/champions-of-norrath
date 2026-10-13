// Host contract for __11SkillCharge; not an EE input. A 104-byte slot filled with 0x5a must end up with
// exactly the listed stores and no other changes.
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
unsigned char vt_skill[8];
int main() {
    alignas(16) unsigned char o[112], want[112];
    std::memset(o, 0x5a, sizeof o);
    std::memset(want, 0x5a, sizeof want);
    __11SkillCharge(o);
    const unsigned w[3] = {word(text), 0, 0};
    std::memcpy(want, w, 12);
    const unsigned short half = 30;
    std::memcpy(want + 12, &half, 2);
    const unsigned vt = word(vt_skill);
    std::memcpy(want + 96, &vt, 4);
    if (std::memcmp(o, want, sizeof o) != 0) { std::puts("bytes differ"); return 1; }
    if (std::strcmp(text, "charge") != 0) return 1;
    return 0;
}
