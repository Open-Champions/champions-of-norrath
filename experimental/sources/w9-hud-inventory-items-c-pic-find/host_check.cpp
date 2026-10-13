// Host check for picFind__4itemPc (includes the source; build the file alone with -fno-builtin).
// Provides small stand-ins for the five C string routines the source declares.
#define NORRATH_INVENTORY_HOST 1
#include "pic_find.cpp"
unsigned char player[1];
unsigned char* _fbss;
int _4item$nmPicGroups;
extern "C" {
char* strcpy(char* to, const char* from) { char* r = to; while ((*to++ = *from++)) {} return r; }
char* strrchr(const char* t, int c) { const char* r = 0; for (; ; ++t) { if (*t == (char)c) r = t; if (!*t) break; } return (char*)r; }
unsigned strlen(const char* t) { unsigned n = 0; while (t[n]) ++n; return n; }
int strcmp(const char* a, const char* b) { while (*a && *a == *b) { ++a; ++b; } return (unsigned char)*a - (unsigned char)*b; }
int atoi(const char* t) { int s = 1, v = 0; if (*t == '-') { s = -1; ++t; } while (*t >= '0' && *t <= '9') v = v * 10 + *t++ - '0'; return s * v; }
}
namespace { const void* words[64]; unsigned used = 0; }
static unsigned intern(const void* p) { words[used] = p; return 0x1000 + 0x10 * used++; }
const void* inventory_items::host_resolve(unsigned w) { return words[(w - 0x1000) / 0x10]; }
struct Entry { short id; unsigned char rest[8]; };
struct Group { unsigned entries; unsigned name; int count; };
int main() {
    static Entry a[3] = {{1}, {2}, {7}}, b[2] = {{-1}, {5}}, c[1] = {{7}};
    static Group groups[3] = {};
    groups[0] = {intern(a), intern("sword"), 3};
    groups[1] = {intern(b), intern("axe"), 2};
    groups[2] = {intern(c), intern("sword"), 1};   // second group with the same stem
    _fbss = reinterpret_cast<unsigned char*>(groups);
    _4item$nmPicGroups = 3;
    if (picFind__4itemPc("sword2.pic") != reinterpret_cast<const unsigned char*>(&a[1])) return 1;
    if (picFind__4itemPc("sword7") != reinterpret_cast<const unsigned char*>(&a[2])) return 2;          // first of two stem matches
    if (picFind__4itemPc("axe.dat") != reinterpret_cast<const unsigned char*>(&b[0])) return 3;          // no digits: wanted -1
    if (picFind__4itemPc("axe5.x") != reinterpret_cast<const unsigned char*>(&b[1])) return 4;         // only the last '.' is cut
    if (picFind__4itemPc("axe9") != 0) return 5;
    if (picFind__4itemPc("") != 0 || picFind__4itemPc(".x") != 0) return 6;
    if (picFind__4itemPc("sword1") != reinterpret_cast<const unsigned char*>(&a[0])) return 7;
    if (picFind__4itemPc("sword") != 0) return 8;                // wanted -1 is absent from sword groups
    if (picFind__4itemPc("sword003") != 0) return 9;             // atoi gives 3, not present
    // The first character is never part of the digit run: "7" has stem "7" and wanted -1.
    static Entry d[1] = {{-1}};
    static Group g1[1] = {{intern(d), intern("7"), 1}};
    _fbss = reinterpret_cast<unsigned char*>(g1); _4item$nmPicGroups = 1;
    if (picFind__4itemPc("7") != reinterpret_cast<const unsigned char*>(&d[0])) return 10;
    if (picFind__4itemPc("77") != 0) return 11;                  // stem "7", wanted 7
    _4item$nmPicGroups = 0;
    if (picFind__4itemPc("7") != 0) return 12;
    return 0;
}
