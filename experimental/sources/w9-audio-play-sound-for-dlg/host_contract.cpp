#define NORRATH_HOST_CONTRACT 1
#include "play_sound_for_dlg.cpp"
#include <cstdarg>
#include <cstdio>
#include <cstring>
signed char animLoadingComplete = 0, animLoadBuffer = 1, waitForSound = 0;
unsigned char animUseBuffer = 9, animStarted = 1;
int anim[8] = {};
static int cd_calls, find_result_present, size_value;
static char started_name[64], found_name[64], sized_name[64];
static int started_stream, started_flags;
static unsigned char record[160];
static struct { char* file; void* dest; void* flag; int length, offset; } load;
static int loads, order[8], steps;
void cdProcess__Fv() { order[steps++] = 1; if (++cd_calls == 3) animLoadingComplete = 1; }
int sprintf(char* d, const char* f, ...) {
    va_list ap; va_start(ap, f);
    const char* s = va_arg(ap, const char*); va_end(ap);
    std::strcpy(d, s);
    std::strcat(d, std::strcmp(f, "%s.vag") == 0 ? ".vag" : std::strcmp(f, "%s.anm") == 0 ? ".anm" : "?");
    return static_cast<int>(std::strlen(d));
}
void soundStartDialogStream__FiPci(int s, char* n, int f) { order[steps++] = 2; started_stream = s; started_flags = f; std::strcpy(started_name, n); }
int soundGetDialogSize__FPc(char* n) { order[steps++] = 3; std::strcpy(sized_name, n); return size_value; }
unsigned char* soundFindDialog__FPc(char* n) { order[steps++] = 4; std::strcpy(found_name, n); return find_result_present ? record : nullptr; }
char* soundGetDialogFilename__Fv() { return const_cast<char*>("file"); }
void cdLoadAsync__FPcPvT0ii(char* f, void* d, void* flag, int l, int o) { order[steps++] = 5; ++loads; load = {f, d, flag, l, o}; }
static void put(int off, int v) { std::memcpy(record + off, &v, 4); }
int main() {
    unsigned char packet[80] = {};
    std::strcpy(reinterpret_cast<char*>(packet + 49), "hello");
    anim[4] = 0x1234; anim[0] = 0x4321;
    size_value = 777;
    put(64, 100); put(144, 612);
    find_result_present = 1;
    playSoundForDlg__FP9DlgPacket(packet);
    if (cd_calls != 3) return 1;      // the loop runs until the flag is set
    if (started_stream != 2 || started_flags != 42 || std::strcmp(started_name, "hello.vag") != 0) return 2;
    if (std::strcmp(sized_name, "hello.vag") != 0 || anim[4 + 3] != 777 || !waitForSound) return 3;
    if (std::strcmp(found_name, "hello.anm") != 0) return 4;
    if (loads != 1 || std::strcmp(load.file, "file") != 0 || reinterpret_cast<long>(load.dest) != 0x1234 || load.flag != &animLoadingComplete) return 5;
    if (load.length != 512 || load.offset != 100 || anim[4 + 2] != 512 || anim[4 + 1] != 1) return 6;
    if (animUseBuffer != 1 || animLoadBuffer != 0 || animStarted != 0) return 7;
    // order of events: 1 1 1 2 3 4 5
    const int expect[7] = {1, 1, 1, 2, 3, 4, 5};
    for (int i = 0; i < 7; ++i) if (order[i] != expect[i]) return 8;
    // not found: the flag is set, slot 0 is marked absent and the buffers swap back
    steps = 0; anim[1] = 5; anim[2] = 6; find_result_present = 0; animLoadingComplete = 0; cd_calls = 0;
    playSoundForDlg__FP9DlgPacket(packet);
    if (loads != 1 || animLoadingComplete != 1 || anim[1] != 0 || anim[2] != 6 || animUseBuffer != 0 || animLoadBuffer != 1) return 9;
    if (anim[3] != 777) return 10;
    std::puts("play_sound_for_dlg passed");
    return 0;
}
