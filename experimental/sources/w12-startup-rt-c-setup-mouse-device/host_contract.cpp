// Host check only; not an EE build input. Build the source with -Dsetup_mouse_device=w12_smd.
#include <cstdio>
#include <cstring>
extern "C" {
alignas(16) unsigned char GLibData[1640];
int w12_smd(void*);
}
static void set_count(int n) { std::memcpy(GLibData + 68, &n, 4); }
static void set_id(int slot, short id) { std::memcpy(GLibData + 1228 + 104 * slot, &id, 2); }
static short get_id(int slot) { short v; std::memcpy(&v, GLibData + 1228 + 104 * slot, 2); return v; }
static int word(int off) { int v; std::memcpy(&v, GLibData + off, 4); return v; }
int main() {
    unsigned char rec[17];
    for (int i = 0; i < 17; ++i) rec[i] = (unsigned char)(0xa0 + i);
    short id = 0x1234; std::memcpy(rec + 5, &id, 2);                      // odd alignment: record = rec + 1
    unsigned char* r = rec + 1;
    for (unsigned i = 0; i < sizeof GLibData; ++i) GLibData[i] = 0x5a;
    set_count(0);
    if (w12_smd(r) != 1 || GLibData[1000] != 0x5a) return 1;               // no slots
    set_count(3); set_id(0, 7); set_id(1, -1); set_id(2, -1);
    if (w12_smd(r) != 0) return 2;                                         // goes to slot 1
    unsigned char* s = GLibData + 1224 + 104;
    if (get_id(1) != 0x1234 || std::memcmp(s, r, 16) != 0) return 3;
    if (word(1224 + 104 + 16) || word(1224 + 104 + 32) || word(1224 + 104 + 36) || s[60] || s[61]) return 4;
    if (word(1224 + 104 + 28) != 1 || word(104) != 1) return 5;
    if (s[20] != 0x5a || s[40] != 0x5a || s[62] != 0x5a || s[59] != 0x5a || word(100) != 0x5a5a5a5a) return 6;
    if (get_id(2) != -1 || get_id(0) != 7) return 7;
    int flag = 0; std::memcpy(GLibData + 104, &flag, 4);
    if (w12_smd(r) != 1 || word(104) != 0 || get_id(2) != -1) return 8;    // duplicate identifier
    id = 0x55; std::memcpy(rec + 5, &id, 2);
    if (w12_smd(r) != 0 || get_id(2) != 0x55) return 9;
    id = 0x66; std::memcpy(rec + 5, &id, 2);
    if (w12_smd(r) != 1 || get_id(0) != 7) return 10;                      // table full
    set_id(0, -1); id = -1; std::memcpy(rec + 5, &id, 2);
    if (w12_smd(r) != 1) return 11;                                        // -1 matches a free slot
    std::puts("setup_mouse_device ok");
    return 0;
}
