#include "surface_ctor.cpp"
#include <initializer_list>
#include <string.h>

int main() {
    int s[100];
    memset(s, 0x7f, sizeof s);
    int want[100];
    memset(want, 0x7f, sizeof want);
    for (int i : {0, 1, 2, 3, 9, 14, 16, 77, 78}) want[i] = 0;
    want[4] = 2;
    want[5] = want[6] = want[7] = -1;
    want[8] = 1;
    for (int i = 0; i < 10; ++i) want[75 - 6 * i] = 0;
    __9VISurface(s);
    return memcmp(s, want, sizeof s) ? 1 : 0;
}
