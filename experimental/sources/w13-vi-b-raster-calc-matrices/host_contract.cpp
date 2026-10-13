#include <initializer_list>
#define NORRATH_HOST_CONTRACT 1
#include <string.h>
static unsigned char scratch[1024];
extern "C" volatile unsigned char* norrath_host_scratch_byte(unsigned address) { return &scratch[address - 0x70000000u]; }

static int calls; static int ids[8]; static unsigned long args[8][3];
static void note(int id, const void* a, const void* b, const void* c) {
    ids[calls] = id; args[calls][0] = (unsigned long)a; args[calls][1] = (unsigned long)b; args[calls][2] = (unsigned long)c; ++calls;
}
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(void* a, const void* b, const void* c) { note(1, a, b, c); }
extern "C" void InvertAffine__10VIMatrix44RC10VIMatrix44(void* a, const void* b) { note(2, a, b, 0); }
extern "C" void SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3(const void* a, const void* b, void* c) { note(3, a, b, c); }
extern "C" void Normalize__7VIVect3(void* a) { note(4, a, 0, 0); }
extern "C" void Mul__C10VIMatrix44RC7VIVect3R7VIVect3(const void* a, const void* b, void* c) { note(5, a, b, c); }
#include "calc_matrices.cpp"

#define S(off) ((unsigned long)(scratch + (off)))
int main() {
    for (int enable : {0, 1, 255}) {
        memset(scratch, 0x77, sizeof scratch);
        scratch[0x222] = static_cast<unsigned char>(enable);
        unsigned char before[1024]; memcpy(before, scratch, sizeof scratch);
        calls = 0;
        if (CalcMatrices__8VIRaster(0) != 0) return 1;
        int expect = enable ? 6 : 5;
        if (calls != expect) return 2;
        int wantId[6] = {1, 1, 2, 3, 4, 5};
        unsigned long want[6][3] = {{S(0x80), S(0x00), S(0x40)}, {S(0x140), S(0x80), S(0xc0)}, {S(0x100), S(0x00), 0},
                                    {S(0x100), S(0x1a0), S(0x180)}, {S(0x180), 0, 0}, {S(0x100), S(0x1c0), S(0x190)}};
        for (int i = 0; i < expect; ++i) {
            if (ids[i] != wantId[i]) return 3;
            for (int j = 0; j < 3; ++j) if (args[i][j] != want[i][j]) return 4;
        }
        if (memcmp(before, scratch, sizeof scratch) != 0) return 5;
    }
    return 0;
}
