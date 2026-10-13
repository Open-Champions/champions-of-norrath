#include "start_global_dialog_stream.cpp"
#include <cstdio>
#include <cstring>
static unsigned char table[160];
static unsigned char* found;
static char* seen_name;
static int finds, starts;
static struct { int stream; char file[16]; int flags, offset, length, a5, a6, a7; } call;
extern "C" unsigned char* soundFindGlobalDialog__FPc(char* n) { ++finds; seen_name = n; return found; }
extern "C" void soundStartStream__FiPciiiiii(int s, char* f, int fl, int o, int l, int a5, int a6, int a7) {
    ++starts; call.stream = s; std::strncpy(call.file, f, 15); call.flags = fl; call.offset = o; call.length = l;
    call.a5 = a5; call.a6 = a6; call.a7 = a7;
}
static void put(int off, int v) { std::memcpy(table + off, &v, 4); }
int main() {
    char name[] = "x";
    found = nullptr;
    soundStartGlobalDialogStream__FiPci(1, name, 5);
    if (finds != 1 || seen_name != name || starts != 0) return 1;
    found = table;
    put(64, 1000); put(144, 1000);
    soundStartGlobalDialogStream__FiPci(1, name, 5);
    if (starts != 0) return 2;
    put(144, 3500); put(68, 11); put(72, 0xfffe); put(76, 33);
    soundStartGlobalDialogStream__FiPci(2, name, 0x40);
    if (starts != 1 || call.stream != 2 || std::strcmp(call.file, "mouth.vat") != 0) return 3;
    if (call.flags != 0x40 || call.offset != 1000 || call.length != 2500) return 4;
    if (call.a5 != 11 || call.a6 != -2 || call.a7 != 33) return 5;
    put(144, 400);   // end below start still starts, with a negative length
    soundStartGlobalDialogStream__FiPci(0, name, 0);
    if (starts != 2 || call.length != -600) return 6;
    std::puts("start_global_dialog_stream passed");
    return 0;
}
