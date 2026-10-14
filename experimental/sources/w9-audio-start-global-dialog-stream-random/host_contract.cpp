#include "start_global_dialog_stream_random.cpp"
#include <cstdio>
#include <cstring>
extern "C" unsigned char norrath_sound_global_dialog_map_view[12] = {};
static unsigned char table[80];
static unsigned char* answer = table;
static void* map_seen;
static int calls;
static struct { unsigned char* table; char file[16]; int stream; char* name; int flags; } seen;
extern "C" unsigned char* get__11ResourceMap(void* m) { map_seen = m; return answer; }
extern "C" void startDlgRandom__FP9dialogDirPciT1i(unsigned char* t, char* f, int s, char* n, int fl) {
    ++calls; seen.table = t; std::strncpy(seen.file, f, 15); seen.stream = s; seen.name = n; seen.flags = fl;
}
int main() {
    char name[] = "hail";
    soundStartGlobalDialogStreamRandom__FiPci(2, name, 0x40);
    if (calls != 1 || map_seen != norrath_sound_global_dialog_map_view) return 1;
    if (seen.table != table || std::strcmp(seen.file, "mouth.vat") != 0) return 2;
    if (seen.stream != 2 || seen.name != name || seen.flags != 0x40) return 3;
    answer = nullptr;
    soundStartGlobalDialogStreamRandom__FiPci(1, name, 0);
    if (calls != 1) return 4;
    std::puts("start_global_dialog_stream_random passed");
    return 0;
}
