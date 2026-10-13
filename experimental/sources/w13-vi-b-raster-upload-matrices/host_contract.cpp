#define NORRATH_HOST_CONTRACT 1
#include <initializer_list>
#include <string.h>
#include <stdint.h>
alignas(16) static unsigned char arena[1024];
alignas(16) static unsigned char scratch[1024];
extern "C" unsigned char* norrath_host_load_pointer(const void* field) { uint32_t v; memcpy(&v, field, 4); return arena + v; }
extern "C" void norrath_host_store_pointer(void* field, unsigned char* value) { uint32_t v = static_cast<uint32_t>(value - arena); memcpy(field, &v, 4); }
extern "C" volatile unsigned char* norrath_host_scratch_byte(unsigned address) { return &scratch[address - 0x70000000u]; }
static int copies; static void* copyDst[3]; static const void* copySrc[3];
extern "C" void sceVu0CopyMatrix(void* destination, const void* source) { copyDst[copies] = destination; copySrc[copies] = source; ++copies; memmove(destination, source, 64); }
#include "upload_matrices.cpp"
unsigned int VIRasterInitOffset __asm__("_8VIRaster$RInitOffset") = 0x00ab1234u;

static float S(int off) { float f; memcpy(&f, scratch + off, 4); return f; }
static void putf(unsigned char* e, int index, float v) { memcpy(e + index * 4, &v, 4); }
static void putw(unsigned char* e, int index, unsigned v) { memcpy(e + index * 4, &v, 4); }

int main() {
    alignas(16) static unsigned char object[20000], want[20000];
    for (int s220 : {0, 1}) for (int s221 : {0, 1}) for (int s222 : {0, 1}) {
        memset(arena, 0x5a, sizeof arena);
        memset(object, 0x6b, sizeof object);
        for (int i = 0; i < 0x300; i += 4) { float f = 100.0f + i * 0.25f; memcpy(scratch + i, &f, 4); }
        scratch[0x220] = s220; scratch[0x221] = s221; scratch[0x222] = s222;
        scratch[0x224] = scratch[0x225] = scratch[0x226] = scratch[0x227] = 0x99;
        unsigned char* P = arena + 32;
        norrath_host_store_pointer(object + 19492, P);
        unsigned word17864 = 0x7f8123ab; memcpy(object + 17864, &word17864, 4);
        int flag = 7; memcpy(object + 17888, &flag, 4);
        memcpy(want, object, sizeof object);
        unsigned char expect[1024]; memcpy(expect, arena, sizeof arena);
        unsigned char scratchBefore[1024]; memcpy(scratchBefore, scratch, sizeof scratch);
        unsigned char* E = expect + 32;
        E[3] = 0x10; putw(E, 1, 0); E[0] = 23; E[1] = 0;
        putw(E + 16, 0, 0); putw(E + 16, 1, 0); putw(E + 16, 2, 0x13000000); putw(E + 16, 3, 0x6c150000);
        memcpy(E + 32, scratch + 0x140, 64); memcpy(E + 96, scratch + 0x80, 64); memcpy(E + 160, scratch + 0xc0, 64);
        unsigned char* e = E + 224;
        putf(e, 0, S(384)); putf(e, 1, -S(392)); putf(e, 2, S(388)); putw(e, 3, 0);
        e = E + 240;
        putf(e, 0, S(400)); putf(e, 1, S(404)); putf(e, 2, S(408)); putf(e, 3, s222 ? 1.0f / S(460) : 1.0f);
        e = E + 256;
        putw(e, 0, word17864); putf(e, 1, S(556)); putw(e, 2, 0); putf(e, 3, S(552));
        e = E + 272;
        if (s221) { putf(e, 0, S(428)); putf(e, 1, S(432)); putf(e, 2, S(436)); putf(e, 3, S(440)); } else for (int i = 0; i < 4; ++i) putw(e, i, 0);
        e = E + 288;
        if (s222) { putf(e, 0, S(464)); putf(e, 1, S(468)); putf(e, 2, S(472)); putw(e, 3, 0); } else for (int i = 0; i < 4; ++i) putw(e, i, 0);
        e = E + 304;
        if (s220) { putf(e, 0, S(480)); putf(e, 1, S(484)); putf(e, 2, S(488)); putf(e, 3, S(492)); } else for (int i = 0; i < 4; ++i) putw(e, i, 0);
        e = E + 320; putf(e, 0, S(528)); putf(e, 1, S(532)); putf(e, 2, S(536)); putf(e, 3, S(540));
        e = E + 336; putf(e, 0, S(496)); putf(e, 1, S(500)); putf(e, 2, S(504)); putf(e, 3, S(508));
        e = E + 352; putf(e, 0, S(512)); putf(e, 1, S(516)); putf(e, 2, S(520)); putf(e, 3, S(524));
        e = E + 368; putw(e, 0, 0); putw(e, 1, 0); putw(e, 2, 0); putw(e, 3, 0x14000000u | 0x00ab1234u);
        norrath_host_store_pointer(want + 19492, P + 384);
        int zero = 0; memcpy(want + 17888, &zero, 4);
        copies = 0;
        if (UploadMatrices__8VIRasterPP10_sceDmaTag(object, object + 19492) != 0) return 1;
        if (copies != 3 || copyDst[0] != P + 32 || copyDst[1] != P + 96 || copyDst[2] != P + 160) return 2;
        if (copySrc[0] != scratch + 0x140 || copySrc[1] != scratch + 0x80 || copySrc[2] != scratch + 0xc0) return 3;
        if (memcmp(object, want, sizeof object) != 0) return 4;
        if (memcmp(arena, expect, sizeof arena) != 0) return 5;
        for (int i = 0; i < 1024; ++i) {
            unsigned char want_byte = (i >= 0x224 && i < 0x228) ? 0 : scratchBefore[i];
            if (scratch[i] != want_byte) return 6;
        }
    }
    return 0;
}
