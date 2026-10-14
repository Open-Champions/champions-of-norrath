// Host check only; not an EE build input. Build the source with -Dprocess_mouse_data=w12_pmd.
#include <cstdio>
#include <cstring>
extern "C" {
alignas(16) unsigned char GLibData[1800];
static int waits, signals, wait_arg, signal_arg, order[8], n_order;
int WaitSema(int s) { ++waits; wait_arg = s; order[n_order++] = 1; return 0; }
int SignalSema(int s) { ++signals; signal_arg = s; order[n_order++] = 2; return 1; }
void w12_pmd(void*);
}
static void put32(int off, int v) { std::memcpy(GLibData + off, &v, 4); }
static void put16(int off, int v) { short s = (short)v; std::memcpy(GLibData + off, &s, 2); }
static int get32(int off) { int v; std::memcpy(&v, GLibData + off, 4); return v; }
static int get16(int off) { unsigned short v; std::memcpy(&v, GLibData + off, 2); return v; }
static const int SLOT1 = 1224 + 104;
static unsigned char rec[28];
static void reset(int flags, int pos, int rd) {
    std::memset(GLibData, 0x5a, sizeof GLibData);
    put32(68, 2); put16(1228, 7); put16(1228 + 104, 0x1234);
    put16(SLOT1 + 14, flags); put32(SLOT1 + 36, pos); put32(SLOT1 + 32, rd); put16(SLOT1 + 60, 9);
    put32(44, 77);
    waits = signals = n_order = 0;
}
int main() {
    std::memset(rec, 0, sizeof rec);
    for (int i = 0; i < 4; ++i) rec[i] = (unsigned char)(0x10 + i);
    rec[6] = 0x34; rec[7] = 0x12;
    for (int i = 20; i < 26; ++i) rec[i] = (unsigned char)(0xa0 + i);
    unsigned char* e = GLibData + SLOT1 + 64;
    reset(0x4000, 2, 0);                                                      // bit 14: fourth byte
    w12_pmd(rec);
    if (waits != 1 || signals != 1 || wait_arg != 77 || signal_arg != 77 || order[0] != 1 || order[1] != 2) return 1;
    if (std::memcmp(e + 16, rec, 4) || e[16 + 4] != rec[20] || e[16 + 5] != rec[21] || e[16 + 6] != rec[22] ||
        e[16 + 7] != rec[23]) return 2;
    if (get32(SLOT1 + 36) != 3 || get32(SLOT1 + 32) != 0 || get16(SLOT1 + 60) != 10) return 3;
    reset(0x0000, 4, 0);                                                      // no fourth byte, wraps
    w12_pmd(rec);
    if (e[32 + 7] != 0x5a || e[32 + 4] != rec[20] || get32(SLOT1 + 36) != 0) return 4;
    if (get32(SLOT1 + 32) != 1 || get16(SLOT1 + 60) != 9) return 5;           // caught up: read position moves
    reset(0x0000, 3, 4);                                                      // read position wraps past 4
    w12_pmd(rec);
    if (get32(SLOT1 + 36) != 4 || get32(SLOT1 + 32) != 0 || get16(SLOT1 + 60) != 9) return 15;
    reset(0x8000, 1, 3);                                                      // bit 15: leading byte must be 1
    rec[20] = 1;
    w12_pmd(rec);
    if (e[8 + 4] != rec[21] || e[8 + 5] != rec[22] || e[8 + 6] != rec[23] || e[8 + 7] != 0x5a) return 6;
    if (get32(SLOT1 + 36) != 2 || get16(SLOT1 + 60) != 10) return 7;
    reset(0x8000, 1, 3);
    rec[20] = 2;
    w12_pmd(rec);
    if (waits || get32(SLOT1 + 36) != 1 || e[8 + 4] != 0x5a) return 8;
    rec[20] = 0xa0 + 20;
    reset(0x4000, 1, 3);
    rec[12] = 0x78; rec[13] = 0x56; rec[14] = 0x34; rec[15] = 0x12;           // nonzero word at +12
    w12_pmd(rec);
    if (waits || get32(SLOT1 + 16) != 0x12345678 || get32(SLOT1 + 36) != 1) return 9;
    rec[12] = rec[13] = rec[14] = rec[15] = 0;
    reset(0x4000, 6, 0);                                                      // 6 + 1 = 7 -> 2 (remainder)
    w12_pmd(rec);
    if (get32(SLOT1 + 36) != 2) return 10;
    reset(0x4000, -2, 1);                                                     // signed remainder: (-2 + 1) % 5 = -1
    w12_pmd(rec);
    if (get32(SLOT1 + 36) != -1) return 11;
    reset(0x4000, 2, 0);
    rec[6] = 0x35;                                                            // unknown identifier
    w12_pmd(rec);
    if (waits || get32(SLOT1 + 36) != 2) return 12;
    rec[6] = 0x07; rec[7] = 0x00;                                             // first slot matches
    put32(1224 + 36, 3); put32(1224 + 32, 0); put16(1224 + 14, 0);
    w12_pmd(rec);
    if (get32(1224 + 36) != 4 || get32(SLOT1 + 36) != 2) return 13;
    put32(68, 0);
    waits = 0;
    w12_pmd(rec);
    if (waits) return 14;
    std::puts("process_mouse_data ok");
    return 0;
}
