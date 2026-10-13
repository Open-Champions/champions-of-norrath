// Host check only; not an EE build input. Build the source with
//   -Dmemcpy=w12_memcpy -Dsprintf=w12_sprintf -Dstrcat=w12_strcat -Dprocess_kbd_data=w12_pkd
#include <cstdio>
#include <string>
extern "C" {
alignas(16) unsigned char GLibData[1700];
static int log_n; static char log_[16][24]; static std::string text;
static unsigned char* seen_slot[4]; static unsigned char* seen_rec[4];
static int rel_ret = 0x11, pre_ret = 0x22, copy_len; static unsigned char copied[16], rec_at_release[8];
int WaitSema(int s) { std::snprintf(log_[log_n++], 24, "W%d", s); return 0; }
int SignalSema(int s) { std::snprintf(log_[log_n++], 24, "S%d", s); return 0; }
void CheckforModifierKeyEvents(unsigned char* slot, unsigned char* rec) { seen_slot[0] = slot; seen_rec[0] = rec; std::snprintf(log_[log_n++], 24, "M"); }
int generate_key_release_events(unsigned char* slot, unsigned char* rec) {
    seen_slot[1] = slot; seen_rec[1] = rec; __builtin_memcpy(rec_at_release, rec + 20, 8); std::snprintf(log_[log_n++], 24, "R");
    __builtin_memset(slot + 224, 0, 32); slot[224] = 0x05; slot[225] = 0x80; slot[255] = 0x01;        // keys 0, 2, 15, 248
    return rel_ret;
}
int generate_key_press_events(unsigned char* slot, unsigned char* rec) { seen_slot[2] = slot; seen_rec[2] = rec; std::snprintf(log_[log_n++], 24, "P"); return pre_ret; }
void* w12_memcpy(void* d, const void* s, unsigned long n) { copy_len = (int)n; __builtin_memcpy(copied, s, n < 16 ? n : 16); __builtin_memcpy(d, s, n); std::snprintf(log_[log_n++], 24, "C"); return d; }
int w12_sprintf(char* b, const char* f, ...) {
    __builtin_va_list ap; __builtin_va_start(ap, f); int v = __builtin_va_arg(ap, int); __builtin_va_end(ap);
    return std::snprintf(b, 112, f, v);
}
char* w12_strcat(char* d, const char* s) { text += s; return d; }
int w12_pkd(void*);
}
static void put32(int off, int v) { __builtin_memcpy(GLibData + off, &v, 4); }
static void put16(int off, int v) { short s = (short)v; __builtin_memcpy(GLibData + off, &s, 2); }
static int get32(int off) { int v; __builtin_memcpy(&v, GLibData + off, 4); return v; }
static const int SLOT1 = 136 + 272;
static unsigned char rec[28];
static void reset() {
    __builtin_memset(GLibData, 0x5a, sizeof GLibData);
    put32(64, 2); put16(140, 7); put16(140 + 272, 0x1234); put32(44, 77);
    __builtin_memset(rec, 0, sizeof rec); rec[6] = 0x34; rec[7] = 0x12; rec[10] = 0xb1; rec[11] = 0xb2;
    log_n = 0; text.clear(); copy_len = -1;
}
int main() {
    reset();
    rec[6] = 0x35;
    if (w12_pkd(rec) != (int)0x80000000 || log_n) return 1;               // unknown identifier
    reset(); put32(64, 0);
    if (w12_pkd(rec) != (int)0x80000000 || log_n) return 2;               // empty table
    reset(); rec[12] = 0x78; rec[13] = 0x56; rec[14] = 0x34; rec[15] = 0x12;
    if (w12_pkd(rec) != 0x12345678 || get32(SLOT1 + 16) != 0x12345678 || log_n) return 3;
    reset(); rec[22] = 1; put32(0, 0);
    __builtin_memcpy(rec + 16, "\xff\xff\xff\xff", 4);
    if (w12_pkd(rec) != 0x8000000c || get32(SLOT1 + 16) != 0x8000000c || log_n) return 4;
    if (*(int*)(rec + 16) != 0) return 5;
    reset();
    rec[22] = 0x41; rec[23] = 0; rec[24] = 0x42; rec[25] = 0; rec[26] = 0; rec[27] = 0x43;
    int r = w12_pkd(rec);
    if (r != pre_ret || get32(SLOT1 + 16) != pre_ret) return 6;
    if (__builtin_strcmp(log_[0], "W77") || __builtin_strcmp(log_[1], "M") || __builtin_strcmp(log_[2], "R") ||
        __builtin_strcmp(log_[3], "P") || __builtin_strcmp(log_[4], "C") || __builtin_strcmp(log_[5], "S77") || log_n != 6) return 7;
    if (seen_slot[0] != GLibData + SLOT1 || seen_rec[0] != rec || seen_slot[1] != seen_slot[0] || seen_slot[2] != seen_slot[0]) return 8;
    if (GLibData[SLOT1 + 32] != 0xb1 || GLibData[SLOT1 + 33] != 0xb2) return 9;
    if (rec[20] != 0x41 || rec[21] != 0x42 || rec[22] != 0x43 || *(int*)(rec + 16) != 3) return 10;
    if (rec_at_release[0] != 0x41 || copy_len != 3 || copied[2] != 0x43 || get32(SLOT1 + 256) != 3) return 11;
    if (__builtin_memcmp(GLibData + SLOT1 + 260, rec + 20, 3)) return 12;
    if (text != "0 2 f f8 ") { std::printf("text=[%s]\n", text.c_str()); return 13; }
    // the list ends at the 6-byte limit and never grows beyond it
    reset();
    for (int i = 22; i < 28; ++i) rec[i] = (unsigned char)(0x30 + i);
    w12_pkd(rec);
    if (*(int*)(rec + 16) != 6 || copy_len != 6) return 14;
    std::puts("process_kbd_data ok");
    return 0;
}
