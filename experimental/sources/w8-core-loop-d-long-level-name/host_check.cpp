// Host contract for getLongLevelName__FPc. Not an EE plan input.
#include <cstring>
static unsigned short tbl_colon[8] = {0, 0, ':', 0};
static unsigned short tbl_ok[8] = {0, 0, 'x', 0};
static char seen[8][16];
static int nseen;
static unsigned short* mapped[8];   // key length -> result, 0 default
extern "C" unsigned short* textGetString__FPc(char* key) {
    if (nseen < 8) std::strcpy(seen[nseen++], key);
    unsigned short* r = mapped[std::strlen(key)];
    return r ? r : tbl_ok;
}
#include "long_level_name.cpp"
static void reset() { nseen = 0; for (auto& m : mapped) m = 0; }
int main() {
    char name[] = "abcd";
    reset();   // first lookup is not a ':' string: returned directly, one lookup
    if (getLongLevelName__FPc(name) != tbl_ok || nseen != 1 || std::strcmp(seen[0], "abcd")) return 1;
    reset();   // 'abcd' and 'abc' give ':' strings, 'ab' is fine: trims twice
    mapped[4] = tbl_colon; mapped[3] = tbl_colon;
    if (getLongLevelName__FPc(name) != tbl_ok || nseen != 3 || std::strcmp(seen[1], "abc") ||
        std::strcmp(seen[2], "ab")) return 2;
    reset();   // every length is ':' : loop ends below 2 chars, last result kept
    mapped[4] = mapped[3] = mapped[2] = tbl_colon;
    if (getLongLevelName__FPc(name) != tbl_colon || nseen != 3) return 3;
    reset();   // one-character name: no pass, original name looked up
    char one[] = "z";
    if (getLongLevelName__FPc(one) != tbl_ok || nseen != 1 || std::strcmp(seen[0], "z")) return 4;
    if (std::strcmp(name, "abcd")) return 5;   // caller's string untouched
    return 0;
}
