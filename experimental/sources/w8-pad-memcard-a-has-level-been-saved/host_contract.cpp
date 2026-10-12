#include "mc_has_this_level_been_saved.cpp"

#include <cstdio>

char ramSlot[112144 + 624 + 64];
static const char* level = "act1/forest";
static int strcmps;
extern "C" void gameGetFullLevelName__FPc(char* name) {
    int i = 0;
    for (; level[i]; ++i) name[i] = level[i];
    name[i] = 0;
}
extern "C" int strcmp(const char* a, const char* b) {
    ++strcmps;
    while (*a && *a == *b) { ++a; ++b; }
    return static_cast<unsigned char>(*a) - static_cast<unsigned char>(*b);
}
static void put(int index, const char* text) {
    char* r = ramSlot + 112144 + 24 * index;
    int i = 0;
    for (; text[i]; ++i) r[i] = text[i];
    r[i] = 0;
}

int main() {
    if (MC_HasThisLevelBeenSaved__Fv() != 0 || strcmps != 0) return 1;  // empty first record
    put(0, "other");
    put(1, "act1/forest");
    if (MC_HasThisLevelBeenSaved__Fv() != 1 || strcmps != 2) return 2;
    ramSlot[112144 + 24 * 1] = 0;
    put(2, "act1/forest");  // hidden behind an empty record
    strcmps = 0;
    if (MC_HasThisLevelBeenSaved__Fv() != 0 || strcmps != 1) return 3;
    put(1, "x");
    ramSlot[112144 + 24 * 2] = 'a';
    for (int i = 2; i < 26; ++i) put(i, "y");
    put(25, "act1/forest");
    strcmps = 0;
    if (MC_HasThisLevelBeenSaved__Fv() != 1 || strcmps != 26) return 4;
    put(25, "y");
    put(26, "act1/forest");  // the 27th record is outside the table
    strcmps = 0;
    if (MC_HasThisLevelBeenSaved__Fv() != 0 || strcmps != 26) return 5;
    std::puts("mc has this level been saved passed");
    return 0;
}
