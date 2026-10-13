// Host check only; not an EE build input. Build the source with
//   -Dmemset=w12_memset -Dsetup_kbd_device=w12_skd
#include <cstdio>
#include <cstring>
extern "C" {
alignas(16) unsigned char GLibData[1640];
static int memsets;
void* w12_memset(void* d, int v, unsigned long n) { ++memsets; unsigned char* p = (unsigned char*)d; for (unsigned long i = 0; i < n; ++i) p[i] = (unsigned char)v; return d; }
int w12_skd(void*);
}
static void set_count(int n) { std::memcpy(GLibData + 64, &n, 4); }
static void set_id(int slot, short id) { std::memcpy(GLibData + 140 + 272 * slot, &id, 2); }
static short get_id(int slot) { short v; std::memcpy(&v, GLibData + 140 + 272 * slot, 2); return v; }
static int word(int off) { int v; std::memcpy(&v, GLibData + off, 4); return v; }
int main() {
    unsigned char rec[17];
    for (int i = 0; i < 17; ++i) rec[i] = (unsigned char)(0xa0 + i);
    short id = 0x1234; std::memcpy(rec + 4 + 1, &id, 2);                  // odd alignment: record = rec + 1
    unsigned char* r = rec + 1;
    for (unsigned i = 0; i < sizeof GLibData; ++i) GLibData[i] = 0x5a;
    set_count(0);
    if (w12_skd(r) != 1 || GLibData[1000] != 0x5a) return 1;               // no slots
    set_count(3); set_id(0, 7); set_id(1, -1); set_id(2, -1);
    if (w12_skd(r) != 0) return 2;                                         // goes to slot 1
    if (get_id(1) != 0x1234 || std::memcmp(GLibData + 136 + 272, r, 16) != 0) return 3;
    unsigned char* s = GLibData + 136 + 272;
    for (int i = 0; i < 8; ++i) if (s[260 + i]) return 4;
    for (int i = 0; i < 32; ++i) if (s[224 + i]) return 5;
    if (word(136 + 272 + 256) || word(136 + 272 + 16) || s[36] || s[37] || s[38] || s[39]) return 6;
    if (word(136 + 272 + 28) != 1 || word(100) != 1 || memsets != 2) return 7;
    if (s[20] != 0x5a || s[40] != 0x5a || s[268] != 0x5a || s[223] != 0x5a) return 8;   // neighbours untouched
    if (get_id(2) != -1 || get_id(0) != 7) return 9;
    int flag = 0; std::memcpy(GLibData + 100, &flag, 4);
    if (w12_skd(r) != 1 || word(100) != 0 || get_id(2) != -1) return 10;  // duplicate identifier
    id = 0x55; std::memcpy(rec + 5, &id, 2);
    if (w12_skd(r) != 0 || get_id(2) != 0x55) return 11;
    id = 0x66; std::memcpy(rec + 5, &id, 2);
    if (w12_skd(r) != 1 || get_id(0) != 7) return 12;                      // table full
    set_id(0, -1); id = -1; std::memcpy(rec + 5, &id, 2);
    if (w12_skd(r) != 1) return 13;                                        // identifier -1 matches a free slot
    std::puts("setup_kbd_device ok");
    return 0;
}
