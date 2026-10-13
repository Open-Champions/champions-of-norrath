// Host contract for findTagInt__10GameObjectPci. Not an EE plan input. The library calls
// are replaced by the host's own, and _ctype_ is rebuilt from the documented class masks.
#include <cstdlib>
#include <cstring>
// The source declares C library names and _ctype_ itself; rename them for the host.
#define strlen w_strlen
#define strncmp w_strncmp
#define atoi w_atoi
#define _ctype_ w_ctype
#include "find_tag_int.cpp"
#undef strlen
#undef strncmp
#undef atoi
extern "C" {
unsigned w_strlen(const char* s) { return (unsigned)std::strlen(s); }
int w_strncmp(const char* a, const char* b, unsigned n) { return std::strncmp(a, b, n); }
int w_atoi(const char* s) { return std::atoi(s); }
// class masks: 1 for A-Z and 2 for a-z at index 1 + c (documented in w8-startup-sce-a-ctype-table)
const unsigned char w_ctype[257] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0};
}

int main() {
    char* list[] = {(char*)"name=orc", (char*)"Height=12", (char*)"height=34", (char*)"heightx=9",
                    (char*)"height", (char*)"height=-5", (char*)"foo=7", 0};
    alignas(8) unsigned char obj[64] = {};
    char** lp = list;
    std::memcpy(obj + 44, &lp, sizeof lp);
    char key[16];
    auto q = [&](const char* k, int n) { std::strcpy(key, k); return findTagInt__10GameObjectPci(obj, key, n); };
    if (q("name", 0) != 0) return 1;               // "orc" is not numeric
    if (q("height", 0) != 34) return 2;            // "Height=12" is not lower case and does not match
    if (q("height", 1) != -5) return 3;            // second match; "heightx" and "height" are skipped
    if (q("height", 2) != 0) return 4;
    if (q("FOO", 0) != 7) return 5;                // key lower-cased
    if (std::strcmp(key, "foo") != 0) return 6;    // ... in the caller's buffer
    if (q("hei", 0) != 0) return 7;                // prefix without '='
    if (q("foo", -1) != 0) return 8;
    list[0] = 0;
    if (q("foo", 0) != 0) return 9;
    lp = 0;
    std::memcpy(obj + 44, &lp, sizeof lp);
    std::strcpy(key, "ABC");
    if (findTagInt__10GameObjectPci(obj, key, 0) != 0) return 10;
    if (std::strcmp(key, "ABC") != 0) return 11;   // untouched when the list is null
    return 0;
}
