#include "clear_stream_que.cpp"
#include <cstdio>
#include <cstring>
extern "C" unsigned char stream[3 * 452];
static int calls = 0;
static int shrink_after = -1;  // lower the count inside memset to model the re-read
unsigned char stream[3 * 452];
static void setw(int r, int off, int v) { std::memcpy(stream + r * 452 + off, &v, 4); }
static int getw(int r, int off) { int v; std::memcpy(&v, stream + r * 452 + off, 4); return v; }
extern "C" void* memset(void* d, int v, unsigned long n) {
    ++calls;
    unsigned char* p = static_cast<unsigned char*>(d);
    for (unsigned long i = 0; i < n; ++i) p[i] = static_cast<unsigned char>(v);
    if (calls == shrink_after) setw(1, 448, 2);
    return d;
}
int main() {
    for (unsigned char& b : stream) b = 0xaa;
    setw(1, 448, 3); setw(1, 36, 77);
    soundClearStreamQue__Fi(1);
    if (calls != 3) return 1;
    for (int i = 0; i < 452; ++i) {
        bool cleared = (i >= 64 && i < 64 + 144) || (i >= 448 && i < 452) || (i >= 36 && i < 40);
        if (cleared != (stream[452 + i] == 0)) return 2;      // only those bytes change
    }
    if (stream[0] != 0xaa || stream[904] != 0xaa || stream[451] != 0xaa) return 3;
    // count of zero or less: no memset, offsets 36 and 448 still cleared
    calls = 0;
    setw(2, 448, 0); setw(2, 36, 5);
    soundClearStreamQue__Fi(2);
    if (calls != 0 || getw(2, 36) != 0 || getw(2, 448) != 0) return 4;
    setw(0, 448, -9); setw(0, 36, 5);
    soundClearStreamQue__Fi(0);
    if (calls != 0 || getw(0, 448) != 0) return 5;
    // the count is re-read after each clear
    setw(1, 448, 5);
    calls = 0; shrink_after = 2;
    soundClearStreamQue__Fi(1);
    if (calls != 2) return 6;
    std::puts("clear_stream_que passed");
    return 0;
}
