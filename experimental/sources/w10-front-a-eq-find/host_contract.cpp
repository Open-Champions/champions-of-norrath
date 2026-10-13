#define NORRATH_HOST_CONTRACT 1
#include "eq_find.cpp"

#include <cstdio>
#include <cstring>

unsigned char* norrath_host_base;
int nmEqGroups, nmWGroups;
unsigned eqGroup, wGroup;

// A 32-bit offset arena stands for the target address space.
static unsigned char arena[4096];
static unsigned put = 16;
static unsigned text(const char* s) {
    unsigned at = put;
    std::strcpy(reinterpret_cast<char*>(arena + at), s);
    put += static_cast<unsigned>(std::strlen(s)) + 1;
    return at;
}
static void word(unsigned at, unsigned v) { std::memcpy(arena + at, &v, 4); }
static void half(unsigned at, short v) { std::memcpy(arena + at, &v, 2); }
static unsigned table(int count, const short* ids) {
    unsigned at = put;
    for (int i = 0; i < count; ++i) { half(at + 8 * i, ids[i]); word(at + 8 * i + 4, 0xabc0 + i); }
    put += 8 * count;
    return at;
}
static unsigned groups(int n, const char* const* names, const unsigned* tabs, const int* counts) {
    unsigned at = put;
    put += 12 * n;
    for (int i = 0; i < n; ++i) {
        word(at + 12 * i, tabs[i]);
        word(at + 12 * i + 4, text(names[i]));
        word(at + 12 * i + 8, static_cast<unsigned>(counts[i]));
    }
    return at;
}

int main() {
    norrath_host_base = arena;
    const short idsA[] = {0, 3, 12, -1};
    const short idsB[] = {7, 8};
    const short idsC[] = {5};
    unsigned ta = table(4, idsA), tb = table(2, idsB), tc = table(1, idsC);
    const char* eqNames[] = {"helm", "boots"};
    unsigned eqTabs[] = {ta, tb};
    int eqCounts[] = {4, 2};
    eqGroup = groups(2, eqNames, eqTabs, eqCounts);
    nmEqGroups = 2;
    const char* wNames[] = {"axe"};
    unsigned wTabs[] = {tc};
    int wCounts[] = {1};
    wGroup = groups(1, wNames, wTabs, wCounts);
    nmWGroups = 1;
    char n1[] = "helm12.tex", n2[] = "helm3", n3[] = "boots8.a.b", n4[] = "helm", n5[] = "helm99", n6[] = "axe5",
         n7[] = "", n8[] = "boots", n9[] = "12", n10[] = ".x", n11[] = "helm12";
    if (eqFind__FPci(n1, 0) != arena + ta + 16) return 1;
    if (eqFind__FPci(n2, 0) != arena + ta + 8) return 2;
    if (eqFind__FPci(n3, 0) != nullptr) return 3;  // cut at the last '.' leaves "boots8.a"
    if (eqFind__FPci(n4, 0) != arena + ta + 24) return 4;  // no digits: number -1 matches id -1
    if (eqFind__FPci(n5, 0) != nullptr) return 5;
    if (eqFind__FPci(n6, 0) != nullptr) return 6;  // axe is only in the w table
    if (eqFind__FPci(n6, 1) != arena + tc) return 7;
    if (eqFind__FPci(n7, 0) != nullptr) return 8;
    if (eqFind__FPci(n8, 0) != nullptr) return 9;  // group boots has no -1
    if (eqFind__FPci(n11, 1) != nullptr) return 10;
    if (eqFind__FPci(n9, 0) != nullptr || eqFind__FPci(n10, 0) != nullptr) return 11;
    char n12[] = "boots7.tex";
    if (eqFind__FPci(n12, 0) != arena + tb) return 12;
    std::puts("eq find passed");
    return 0;
}
