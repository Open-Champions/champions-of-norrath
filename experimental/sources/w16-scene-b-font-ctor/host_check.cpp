#include <string.h>
static void* g_arg;
extern "C" int Clear__7VIRFont(void* self) {
    g_arg = self;
    return 7;
}
#include "font_ctor.cpp"

int main() {
    unsigned char o[100];
    memset(o, 0x66, sizeof o);
    unsigned char want[100];
    memset(want, 0x66, sizeof want);
    memset(want, 0, 8);
    memset(want + 84, 0, 4);
    if (__7VIRFont(o) != 7) return 1;
    if (g_arg != o) return 2;
    return memcmp(o, want, sizeof o) ? 3 : 0;
}
