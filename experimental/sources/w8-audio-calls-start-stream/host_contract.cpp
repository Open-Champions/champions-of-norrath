#include "start_stream.cpp"
#include <cstdio>
#include <cstring>
extern "C" unsigned char stream[3 * 452] = {};
static int lookup_ok = 1, lookup_size = 1000, lookup_calls = 0, print_calls = 0, printed = -1;
static char* lookup_name; static int lookup_flag = -1;
extern "C" int cdDirectoryLookup__FPcPiT1i(char* n, int* a, int* b, int f) {
    ++lookup_calls; lookup_name = n; lookup_flag = f; *a = 0x5a5a; *b = lookup_size; return lookup_ok;
}
extern "C" char* strcpy(char* d, const char* s);
extern "C" void soundPrintStream__Fi(int i) { ++print_calls; printed = i; }
static int w(int rec, int block, int word) { int v; std::memcpy(&v, stream + rec * 452 + 64 + block * 48 + word * 4, 4); return v; }
static int cnt(int rec) { int v; std::memcpy(&v, stream + rec * 452 + 448, 4); return v; }
int main() {
    char name[] = "a.vat";
    soundStartStream__FiPciiiiii(1, name, 0x22, 5000, -1, 11, 12, 13);
    if (lookup_calls != 1 || lookup_name != name || lookup_flag != 0) return 1;
    if (cnt(1) != 1 || print_calls != 1 || printed != 1) return 2;
    if (std::strcmp(reinterpret_cast<char*>(stream + 452 + 64), "a.vat") != 0) return 3;
    if (w(1, 0, 4) != 5000 || w(1, 0, 6) != 5000) return 4;
    if (w(1, 0, 5) != 1008 || w(1, 0, 7) != 1008) return 5;        // size 1000 rounded up to 16
    if (w(1, 0, 8) != 11 || w(1, 0, 9) != 12 || w(1, 0, 10) != 13 || w(1, 0, 11) != 0x22) return 6;
    // explicit length, zero allowed; second block
    soundStartStream__FiPciiiiii(1, name, 1, 7, 0, 1, 2, 3);
    if (cnt(1) != 2 || w(1, 1, 5) != 0 || w(1, 1, 7) != 0 || w(1, 1, 4) != 7) return 7;
    lookup_size = 32;                                                // exact multiple stays
    soundStartStream__FiPciiiiii(1, name, 1, 7, -5, 1, 2, 3);
    if (w(1, 2, 5) != 32) return 8;
    // lookup failure changes nothing
    lookup_ok = 0; int before = print_calls;
    soundStartStream__FiPciiiiii(1, name, 1, 7, 9, 1, 2, 3);
    if (cnt(1) != 3 || print_calls != before) return 9;
    lookup_ok = 1;
    // full queue: no lookup at all
    std::memset(stream + 2 * 452, 0, 452);
    int eight = 8; std::memcpy(stream + 2 * 452 + 448, &eight, 4);
    int calls = lookup_calls;
    soundStartStream__FiPciiiiii(2, name, 1, 7, 9, 1, 2, 3);
    if (lookup_calls != calls || cnt(2) != 8) return 10;
    // untouched neighbors: stream 0 and the bytes after block 2 of stream 1
    for (int i = 0; i < 452; ++i) if (stream[i] != 0) return 11;
    for (int i = 64 + 3 * 48; i < 448; ++i) if (stream[452 + i] != 0) return 12;
    // seventh block of a stream with seven queued
    int seven = 7; std::memcpy(stream + 448, &seven, 4);
    soundStartStream__FiPciiiiii(0, name, 3, 4, 8, 1, 2, 3);
    if (cnt(0) != 8 || w(0, 7, 11) != 3) return 13;
    std::puts("start_stream passed");
    return 0;
}
