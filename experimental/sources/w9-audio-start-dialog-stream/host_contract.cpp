#include "start_dialog_stream.cpp"
#include <cstdio>
#include <cstring>
static unsigned char backing[80 * 16];
static unsigned char* table = backing + 80 * 4;   // room before the table for a negative index
static unsigned char* dir = table;
static char file_name[] = "level.vat";
static int dir_calls, name_calls, starts;
static struct { int stream; char* file; int flags, offset, length, a5, a6, a7; } call;
extern "C" unsigned char* soundGetDialogDir__Fv() { ++dir_calls; return dir; }
extern "C" char* soundGetDialogFilename__Fv() { ++name_calls; return file_name; }
extern "C" void soundStartStream__FiPciiiiii(int s, char* f, int fl, int o, int l, int a5, int a6, int a7) {
    ++starts; call = {s, f, fl, o, l, a5, a6, a7};
}
static void put(int rec, int off, int v) { std::memcpy(table + rec * 80 + off, &v, 4); }
int main() {
    for (int k = 0; k < 4; ++k) { table[k * 80] = 'a' + k; put(k, 64, 100 * k); put(k, 68, 7 + k); put(k, 72, k == 2 ? 0xfffd : k); put(k, 76, 9 + k); }
    // word 144 of record k is word 64 of record k + 1
    soundStartDialogStream__Fiii(1, 2, 0x8);
    if (starts != 1 || call.stream != 1 || call.file != file_name || call.flags != 8) return 1;
    if (call.offset != 200 || call.length != 100 || call.a5 != 9 || call.a6 != -3 || call.a7 != 11) return 2;
    soundStartDialogStream__Fiii(0, 3, 0);                 // last populated record
    if (starts != 2 || call.offset != 300 || call.length != -300) return 3;
    soundStartDialogStream__Fiii(0, 4, 0);                 // record 4 has an empty name
    if (starts != 2) return 4;
    soundStartDialogStream__Fiii(0, 9, 0);                 // far past the end: stops at record 4
    if (starts != 2) return 5;
    table[1 * 80] = 0;                                     // a hole before the index
    soundStartDialogStream__Fiii(0, 2, 0);
    if (starts != 2) return 6;
    soundStartDialogStream__Fiii(0, 0, 0);                 // an index before the hole is fine
    if (starts != 3 || call.offset != 0) return 7;
    soundStartDialogStream__Fiii(0, -1, 0);                // no check; reads the record before the table
    if (starts != 4 || call.offset != 0 || call.length != 0) return 8;
    dir = nullptr;
    soundStartDialogStream__Fiii(0, 0, 0);
    if (starts != 4) return 9;
    std::puts("start_dialog_stream passed");
    return 0;
}
