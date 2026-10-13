// Host contract for objectAddString; not an EE input. A plain model of the hash and probing is
// compared against the routine on random names, including quoted and mixed-case ones.
#define strcpy norrath_hc_strcpy
#define strchr norrath_hc_strchr
#define strlen norrath_hc_strlen
#define strcasecmp norrath_hc_strcasecmp
#include "object_add_string.cpp"
#undef strcpy
#undef strchr
#undef strlen
#undef strcasecmp
#include <cstdio>
#include <cstring>
#include <strings.h>
char* stringHash[512];
void* objectHash[512];
int objectStrings;
unsigned char _ctype_[257];
extern "C" char* norrath_hc_strcpy(char* d, const char* s) { return std::strcpy(d, s); }
extern "C" char* norrath_hc_strchr(const char* t, int c) { return const_cast<char*>(std::strchr(t, c)); }
extern "C" unsigned norrath_hc_strlen(const char* t) { return static_cast<unsigned>(std::strlen(t)); }
extern "C" int norrath_hc_strcasecmp(const char* a, const char* b) { return strcasecmp(a, b); }
static int ref_slot(const char* s) {
    int h = 0;
    for (; *s; ++s) { int c = static_cast<signed char>(*s); if (c >= 'A' && c <= 'Z') c += 32; h += c; h += h << 10; h ^= h >> 6; }
    h += h << 3; h ^= h >> 11;
    return h & 0x1ff;
}
int main() {
    int bad = 0;
    for (int c = 'A'; c <= 'Z'; ++c) _ctype_[1 + c] = 1;
    static char names[600][40];
    unsigned seed = 3;
    for (int i = 0; i < 400; ++i) {
        int len = 1 + (seed = seed * 1664525u + 1013904223u) % 8;
        for (int k = 0; k < len; ++k) names[i][k] = "abAB_x"[(seed = seed * 1664525u + 1013904223u) % 6 + 0];
        names[i][len] = 0;
    }
    int obj[600];
    for (int i = 0; i < 300; ++i) {
        int before = objectStrings;
        int want = ref_slot(names[i]);
        while (stringHash[want] && (objectHash[want] || strcasecmp(stringHash[want], names[i]) != 0)) want = (want + 1) & 0x1ff;
        objectAddString__FPcP10GameObject(names[i], &obj[i]);
        if (objectStrings != before + 1 || stringHash[want] != names[i] || objectHash[want] != &obj[i]) { std::printf("fail: slot %d\n", i); bad = 1; break; }
    }
    // quoted name: both quotes removed in place
    char quoted[] = "\"Hello World\" tail";
    objectAddString__FPcP10GameObject(quoted, &obj[400]);
    if (std::strcmp(quoted, "Hello World") != 0) { std::puts("fail: quote strip"); bad = 1; }
    char open[] = "\"Open";
    objectAddString__FPcP10GameObject(open, &obj[401]);
    if (std::strcmp(open, "Open") != 0) { std::puts("fail: open quote"); bad = 1; }
    // a name reserved without an object is taken over by the same name in another case
    std::memset(stringHash, 0, sizeof stringHash); std::memset(objectHash, 0, sizeof objectHash);
    char first[] = "Gate", second[] = "gATE", third[] = "gate";
    int s = ref_slot("gate");
    stringHash[s] = first;
    objectAddString__FPcP10GameObject(second, &obj[402]);
    if (stringHash[s] != second || objectHash[s] != &obj[402]) { std::puts("fail: reuse"); bad = 1; }
    objectAddString__FPcP10GameObject(third, &obj[403]);
    int t = (s + 1) & 0x1ff;
    if (stringHash[t] != third || objectHash[t] != &obj[403]) { std::puts("fail: probe past object"); bad = 1; }
    if (!bad) std::puts("objectAddString ok");
    return bad;
}
