#define NORRATH_SQ_HOST 1
#include <cstdio>
#include <cstring>
extern "C" {
int nmPlayers;
unsigned char player[4 * 23984];
}
#include "remove_quest.cpp"
static int count_of(int p) { int v; std::memcpy(&v, player + p * 23984 + 21864, 4); return v; }
static void set_count(int p, int v) { std::memcpy(player + p * 23984 + 21864, &v, 4); }
static char* name_of(int p, int i) { return (char*)player + p * 23984 + 17768 + i * 64; }
static char* desc_of(int p, int i) { return (char*)player + p * 23984 + 17800 + i * 64; }
static void put(int p, int i, const char* n, const char* d) {
    std::strcpy(name_of(p, i), n);
    std::strcpy(desc_of(p, i), d);
}
int main() {
    char a[] = "a", b[] = "b", c[] = "c", missing[] = "zz";
    nmPlayers = 2;
    for (int p = 0; p < 2; ++p) {
        put(p, 0, "a", "da");
        put(p, 1, "b", "db");
        put(p, 2, "c", "dc");
        set_count(p, 3);
    }
    // remove the middle entry from every record; the rest shift down
    removeQuestFromLog__FPc(b);
    for (int p = 0; p < 2; ++p) {
        if (count_of(p) != 2) return 1;
        if (std::strcmp(name_of(p, 0), "a") || std::strcmp(desc_of(p, 0), "da")) return 2;
        if (std::strcmp(name_of(p, 1), "c") || std::strcmp(desc_of(p, 1), "dc")) return 3;
    }
    // removing the last entry leaves the nine fill bytes in its slot
    removeQuestFromLog__FPc(c);
    if (count_of(0) != 1) return 4;
    if (std::memcmp(name_of(0, 1), "hit\0\0\0\0\0c", 9) != 0) return 5;
    if (std::memcmp(desc_of(0, 1), "hit\0\0\0\0\0c", 9) != 0) return 6;
    // removing the first entry shifts the survivor to the front
    put(1, 0, "x", "dx");
    put(1, 1, "y", "dy");
    set_count(1, 2);
    char x[] = "x";
    removeQuestFromLog__FPc(x);
    if (count_of(1) != 1 || std::strcmp(name_of(1, 0), "y") || std::strcmp(desc_of(1, 0), "dy")) return 7;
    // an unknown name changes nothing
    removeQuestFromLog__FPc(missing);
    if (count_of(0) != 1 || count_of(1) != 1) return 8;
    // a record without that name is skipped while the others are still handled
    removeQuestFromLog__FPc(a);
    if (count_of(0) != 0 || count_of(1) != 1) return 9;
    // a non-positive count is not scanned
    set_count(1, -3);
    std::strcpy(name_of(1, 0), "y");
    char y[] = "y";
    removeQuestFromLog__FPc(y);
    if (count_of(1) != -3) return 10;
    nmPlayers = 0;
    set_count(0, 1);
    put(0, 0, "a", "da");
    removeQuestFromLog__FPc(a);
    if (count_of(0) != 1) return 11;
    std::puts("ok");
    return 0;
}
