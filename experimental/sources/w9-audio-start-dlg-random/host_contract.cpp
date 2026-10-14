#include <cstdio>
#include <cstring>
#define strlen audio_strlen
#define strncmp audio_strncmp
#include "start_dlg_random.cpp"
#undef strlen
#undef strncmp
extern "C" signed char language = 2;
extern "C" unsigned eRandState = 0;
extern "C" unsigned long audio_strlen(const char* t) { return std::strlen(t); }
extern "C" int audio_strncmp(const char* a, const char* b, unsigned long n) { return std::strncmp(a, b, n); }
static unsigned char backing[80 * 20];
static unsigned char* table = backing + 80 * 2;
static int starts;
static struct { int stream; char* file; int flags, offset, length, a5, a6, a7; } call;
extern "C" void soundStartStream__FiPciiiiii(int s, char* f, int fl, int o, int l, int a5, int a6, int a7) {
    ++starts; call = {s, f, fl, o, l, a5, a6, a7};
}
static void put(int rec, int off, int v) { std::memcpy(table + rec * 80 + off, &v, 4); }
static void put16(int rec, int off, short v) { std::memcpy(table + rec * 80 + off, &v, 2); }
static void record(int rec, const char* name, short tag, int offset) {
    std::strcpy(reinterpret_cast<char*>(table + rec * 80), name); put16(rec, 74, tag); put(rec, 64, offset);
}
int main() {
    char file[] = "f.vat", prefix[] = "bo";
    record(0, "boss1", 2, 100); record(1, "other", 2, 200); record(2, "boss2", 1, 300);
    record(3, "boss3", 2, 400); record(4, "bo", 2, 500); record(5, "boss4", 2, 600);
    record(6, "end", 2, 700);    // record 7 is empty and ends the table; its offset word is the last end
    put(7, 64, 710);
    put16(4, 72, -2); put(4, 68, 21); put(4, 76, 22);
    // qualifying records: 0, 3, 4, 5 (count 4); record 2 has the wrong tag, record 1 the wrong name
    const unsigned s1 = 0u * 0x19660du + 0x3c6ef35fu;
    int pick = (s1 >> 16) % 4;
    startDlgRandom__FP9dialogDirPciT1i(table, file, 3, prefix, 0x11);
    const int order[4] = {0, 3, 4, 5};
    const int off[8] = {100, 200, 300, 400, 500, 600, 700, 710};
    if (eRandState != s1 || starts != 1) return 1;
    if (call.stream != 3 || call.file != file || call.flags != 0x11) return 2;
    if (call.offset != off[order[pick]] || call.length != off[order[pick] + 1] - off[order[pick]]) return 3;
    // every pick is reachable by choosing the generator state
    for (int want = 0; want < 4; ++want) {
        for (unsigned seed = 1;; ++seed) {
            unsigned next = seed * 0x19660du + 0x3c6ef35fu;
            if ((next >> 16) % 4 == static_cast<unsigned>(want)) { eRandState = seed; break; }
        }
        const int before = starts;
        startDlgRandom__FP9dialogDirPciT1i(table, file, 0, prefix, 0);
        if (starts != before + 1 || call.offset != off[order[want]]) return 4 + want;
    }
    // record 4 carries the extra words
    for (unsigned seed = 1;; ++seed) { unsigned next = seed * 0x19660du + 0x3c6ef35fu; if ((next >> 16) % 4 == 2) { eRandState = seed; break; } }
    startDlgRandom__FP9dialogDirPciT1i(table, file, 0, prefix, 0);
    if (call.a5 != 21 || call.a6 != -2 || call.a7 != 22) return 8;
    // no qualifying record: nothing starts and the generator is left alone
    eRandState = 77;
    const int before = starts;
    char none[] = "zz";
    startDlgRandom__FP9dialogDirPciT1i(table, file, 0, none, 0);
    if (starts != before || eRandState != 77) return 9;
    // an empty table
    unsigned char empty[80] = {};
    startDlgRandom__FP9dialogDirPciT1i(empty, file, 0, prefix, 0);
    if (starts != before || eRandState != 77) return 10;
    // chosen record 5 whose end word (record 6's offset) equals its start: the generator moves, nothing starts
    put(5, 64, 0); put(6, 64, 0);
    for (unsigned seed = 1;; ++seed) { unsigned next = seed * 0x19660du + 0x3c6ef35fu; if ((next >> 16) % 4 == 3) { eRandState = seed; break; } }
    const unsigned seed3 = eRandState;
    startDlgRandom__FP9dialogDirPciT1i(table, file, 0, prefix, 0);
    if (starts != before || eRandState == seed3) return 11;
    // a different language
    language = 1;
    eRandState = 5;
    startDlgRandom__FP9dialogDirPciT1i(table, file, 0, prefix, 0);   // only record 2 qualifies
    if (starts != before + 1 || call.offset != 300) return 12;
    std::puts("start_dlg_random passed");
    return 0;
}
