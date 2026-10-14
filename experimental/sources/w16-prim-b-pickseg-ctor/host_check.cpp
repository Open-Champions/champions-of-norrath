#include <initializer_list>
#include "ctor.cpp"
#include <string.h>

int main() {
    unsigned char o[112];
    memset(o, 0x44, sizeof o);
    unsigned char want[112];
    memcpy(want, o, sizeof o);
    const std::initializer_list<int> zero_at = {0, 12, 24, 36, 64, 76};
    for (int off : zero_at) memset(want + off, 0, 12);
    memset(want + 88, 0, 4);
    int minus1 = -1;
    memcpy(want + 60, &minus1, 4);
    __13VIPickSegment(o);
    if (memcmp(o, want, sizeof o) != 0) return 1;
    return 0;
}
