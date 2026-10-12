#include "start_global_music.cpp"
#include <cstdio>
#include <cstring>
float soundMusicVolume = 1.0f;
float musicDuckAmount = 1.0f;
static unsigned char table[160];
static unsigned char* found = table;
static int vol_calls, start_calls;
static int vol_stream; static float vol;
static int s_stream, s_flags, s_off, s_len, s_a5, s_a6, s_a7; static char s_file[16];
extern "C" unsigned char* soundFindGlobalDialog__FPc(char*) { return found; }
extern "C" void soundSetStreamVolume__Fif(int s, float v) { ++vol_calls; vol_stream = s; vol = v; }
extern "C" void soundStartStream__FiPciiiiii(int s, char* f, int fl, int o, int l, int a5, int a6, int a7) {
    ++start_calls; s_stream = s; std::strcpy(s_file, f); s_flags = fl; s_off = o; s_len = l; s_a5 = a5; s_a6 = a6; s_a7 = a7;
}
static void setw(int off, int v) { std::memcpy(table + off, &v, 4); }
int main() {
    char name[] = "m";
    setw(64, 100); setw(144, 612); setw(68, 7); setw(76, 9);
    table[72] = 0xfe; table[73] = 0xff;    // halfword -2 must sign extend
    musicDuckAmount = 0.5f; soundMusicVolume = 0.8f;
    soundStartGlobalMusic__FPci(name, 0x100);
    if (vol_calls != 1 || vol_stream != 0 || vol != 0.5f * 0.8f * 0.46875f) return 1;
    if (start_calls != 1 || s_stream != 0 || std::strcmp(s_file, "mouth.vat") != 0) return 2;
    if (s_flags != (0x100 | 0x22) || s_off != 100 || s_len != 512) return 3;
    if (s_a5 != 7 || s_a6 != -2 || s_a7 != 9) return 4;
    soundStartGlobalMusic__FPci(name, 0x2);
    if (s_flags != 0x22) return 5;
    found = nullptr;                      // volume is still set before the lookup
    soundStartGlobalMusic__FPci(name, 0);
    if (vol_calls != 3 || start_calls != 2) return 6;
    found = table; setw(144, 100);        // empty span: no stream
    soundStartGlobalMusic__FPci(name, 0);
    if (start_calls != 2) return 7;
    soundMusicVolume = -0.25f;            // negative volume: nothing at all
    int before = vol_calls;
    soundStartGlobalMusic__FPci(name, 0);
    if (vol_calls != before || start_calls != 2) return 8;
    soundMusicVolume = 0.0f;              // zero is allowed
    setw(144, 612);
    soundStartGlobalMusic__FPci(name, 0);
    if (vol_calls != before + 1 || vol != 0.0f || start_calls != 3) return 9;
    std::puts("start_global_music passed");
    return 0;
}
