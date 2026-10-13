// Host check for soundSetAmbientData__FPc. Not an EE plan input.
#include <cstdio>
#include <cstdarg>
#include <cstring>
#define memset norrath_host_memset
#define strchr norrath_host_strchr
#define strstr norrath_host_strstr
#define sscanf norrath_host_sscanf
#define _ctype_ norrath_host_ctype
#include "set_ambient_data.cpp"
#undef memset
#undef strchr
#undef strstr
#undef sscanf
#undef _ctype_

extern "C" {
unsigned char ambientGroup[680 * 16];
int nmAmbientGroups = 9;
unsigned eRandState = 1;
// Entry c + 1 has bit 0 set for the upper-case ASCII letters only.
const unsigned char norrath_host_ctype[257] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0
};
void* norrath_host_memset(void* d, int v, unsigned n) { return std::memset(d, v, n); }
char* norrath_host_strchr(const char* s, int c) { return const_cast<char*>(std::strchr(s, c)); }
char* norrath_host_strstr(const char* s, const char* p) { return const_cast<char*>(std::strstr(s, p)); }
int norrath_host_sscanf(const char* s, const char* f, ...) {
    va_list ap; va_start(ap, f);
    int r = std::vsscanf(s, f, ap);
    va_end(ap);
    return r;
}
}

static int word(const unsigned char* p) { int v; std::memcpy(&v, p, 4); return v; }

int main() {
    soundSetAmbientData__FPc(nullptr);
    if (nmAmbientGroups != 0) return 1;

    char text[] = "; note\r\n1-3\n10\n20\nWolf.VAG\nOwl.vag\n7\n4\nx.vag\n";
    std::memset(ambientGroup, 0xa5, sizeof ambientGroup);
    eRandState = 0;
    soundSetAmbientData__FPc(text);
    if (nmAmbientGroups != 2) return 2;
    if (std::strstr(text, "WOLF") || !std::strstr(text, "wolf.vag")) return 3;
    const unsigned char* g0 = ambientGroup;
    const unsigned char* g1 = ambientGroup + 680;
    if (word(g0) != 1 || word(g0 + 4) != 3 || word(g0 + 8) != 2) return 4;
    if (word(g0 + 16) != 10 || word(g0 + 20) != 20) return 5;
    // eRandState after one step from 0 is 1013904223; its top 16 bits are 15470.
    const float expect0 = (1.0f + (15470.0f * (1.0f / 65536.0f)) * 2.0f) * 60.0f;
    if (word(g0 + 12) != static_cast<int>(expect0)) return 6;
    if (std::strcmp(reinterpret_cast<const char*>(g0 + 24), "wolf.vag") != 0) return 7;
    if (std::strcmp(reinterpret_cast<const char*>(g0 + 24 + 41), "owl.vag") != 0) return 8;
    if (word(g1) != 7 || word(g1 + 4) != 7 || word(g1 + 8) != 1 || word(g1 + 16) != 4) return 9;
    if (std::strcmp(reinterpret_cast<const char*>(g1 + 24), "x.vag") != 0) return 10;
    if (ambientGroup[680 * 2] != 0xa5 - 0xa5) return 11;
    std::puts("set_ambient_data passed");
    return 0;
}
