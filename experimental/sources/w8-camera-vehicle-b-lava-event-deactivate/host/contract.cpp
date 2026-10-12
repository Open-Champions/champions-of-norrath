// Host contract for eventDeActivate__11LavaTractor. Not an EE plan input.
#include <cstdio>
#include <cstring>
extern "C" void eventDeActivate__11LavaTractor(void* object);
static int seen[4], count = 0;
extern "C" void soundStopSound__Fi(int handle) { if (count < 4) seen[count] = handle; ++count; }
int main() {
    static unsigned char object[2400];
    const int a = 0x11223344, b = -7;
    std::memcpy(object + 2360, &a, 4);
    std::memcpy(object + 2364, &b, 4);
    eventDeActivate__11LavaTractor(object);
    if (count != 2) { std::fprintf(stderr, "fail: call count %d\n", count); return 1; }
    if (seen[0] != a || seen[1] != b) { std::fprintf(stderr, "fail: handles or order\n"); return 1; }
    return 0;
}
