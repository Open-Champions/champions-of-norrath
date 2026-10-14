// Host check only; not a build input. Pointer words hold arena offsets (NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "draw_packvuncg.cpp"
#include <initializer_list>
#include <math.h>
#include <string.h>

alignas(16) static unsigned char arena[0x8000];
alignas(16) static unsigned char scratchpad[0x1000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
extern "C" unsigned char* norrath_scratchpad(unsigned offset) { return scratchpad + offset; }

static char order[64];
static int norder;
static unsigned added[8];
static int nadded;
extern "C" void FlushDMABufferIfFull__8VIRaster(unsigned char*) { order[norder++] = 'F'; }
extern "C" void CalcMatrices__8VIRaster(unsigned char*) { order[norder++] = 'C'; }
extern "C" void UploadMatrices__8VIRasterPP10_sceDmaTag(unsigned char* r, unsigned char* cursor) {
    order[norder++] = 'U';
    if (cursor != r + 19492) order[norder++] = '!';
}
extern "C" void AddMaterial__8VIRasteriPP10_sceDmaTag(unsigned char* r, unsigned char* material, unsigned char* cursor) {
    order[norder++] = 'A';
    if (cursor != r + 19492) order[norder++] = '!';
    added[nadded++] = static_cast<unsigned>(material - arena);
    unsigned p;
    memcpy(&p, cursor, 4);
    memset(arena + p, 0x44, 16);
    p += 16;
    memcpy(cursor, &p, 4);
}
static char tail_marker;
static int tess_result, tess_calls;
static unsigned tess_args[3];
extern "C" void UploadPreTrans__8VIRasterPP10_sceDmaTag(unsigned char* r, unsigned char* cursor) {
    order[norder++] = 'T';
    if (cursor != r + 19492) order[norder++] = '!';
}
extern "C" int DrawPrimBufferTessPackVUNCG__8VIRasteriiUi(unsigned char* r, int a, int b, unsigned flags) {
    order[norder++] = 'X';
    ++tess_calls;
    tess_args[0] = a; tess_args[1] = b; tess_args[2] = flags;
    return tess_result;
}

static const unsigned R = 0x1000, ENT = 0x3000, BUF = 0x3100, REC = 0x3200, SETS = 0x3300, SETOBJ = 0x3400, MATARR = 0x3500, MAT = 0x3600, DATA = 0x3800, DMA = 0x4000, QOBJ = 0x5000, QREC = 0x5200, QENT = 0x5400;
static void w32(unsigned off, unsigned v) { memcpy(arena + off, &v, 4); }
static unsigned r32(unsigned off) { unsigned v; memcpy(&v, arena + off, 4); return v; }
static void wf(unsigned char* base, unsigned off, float f) { memcpy(base + off, &f, 4); }

int main() {
    for (int flags = 0; flags < 4; ++flags) {
        for (int dirty : {0, 1}) {
            for (int pre : {0, 1}) {
                for (int tess : {0, 1, 5, -1}) {
            memset(arena, 0, sizeof arena);
            memset(scratchpad, 0, sizeof scratchpad);
            w32(R + 19328, ENT);
            w32(ENT + 12 * 2, BUF);
            w32(BUF + 64, 3); w32(BUF + 88, REC); w32(BUF + 96, DATA);
            for (int i = 0; i < 4; ++i) { float f = 10.0f + i; memcpy(arena + BUF + 48 + 4 * i, &f, 4); }
            unsigned ids[3] = {1, 0, 2}, off[3] = {5, 7, 9}, len[3] = {0x10003, 20, 30};
            for (int i = 0; i < 3; ++i) { w32(REC + 16 * i, ids[i]); w32(REC + 16 * i + 8, off[i]); w32(REC + 16 * i + 12, len[i]); }
            w32(R + 19436, SETS);
            w32(SETS + 12 * 1, SETOBJ);
            w32(SETOBJ + 12, MATARR);
            for (int i = 0; i < 3; ++i) w32(MATARR + 4 * i, MAT + 0x40 * i);
            w32(MAT + 0x40 * 1 + 60, 9);
            w32(R + 17888, dirty);
            w32(R + 19492, DMA);
            norder = nadded = 0;
                    w32(R + 17896, pre);
                    tess_result = tess; tess_calls = 0;
                    int r = DrawPrimBufferPackVUNCG__8VIRasteriiUi(arena + R, 2, 1, flags);
                    bool drawn[3] = {(flags & 2) != 0, (flags & 1) != 0, (flags & 1) != 0};
                    char want[32]; int nw = 0;
                    want[nw++] = 'F';
                    if (dirty) want[nw++] = 'C';
                    want[nw++] = 'U';
                    if (pre) want[nw++] = 'T';
                    for (int i = 0; i < 3; ++i) if (drawn[i]) want[nw++] = 'A';
                    want[nw++] = 'F';
                    want[nw++] = 'X';
                    if (norder != nw || memcmp(order, want, nw) != 0) return 1;
                    if (tess_args[0] != 2 || tess_args[1] != 1 || tess_args[2] != (unsigned)flags) return 2;
                    if (r != (tess ? 1 : 1)) return 3;
                    for (int i = 0; i < 4; ++i) { float f = 10.0f + i; if (memcmp(scratchpad + 0x210 + 4 * i, &f, 4) != 0) return 8; }
                    // result is 1 anyway here (the flagged material); check the opaque-only case below.
                    // material k is drawn iff (flagged ? flags & 2 : flags & 1)
                    int tags = 0;
                    for (int i = 0; i < 3; ++i) tags += drawn[i];
                    if (r32(R + 19492) != DMA + 32 * tags) return 3;
                    int t = 0;
                    for (int i = 0; i < 3; ++i) {
                        if (!drawn[i]) continue;
                        unsigned base = DMA + 32 * t++;
                        if (added[t - 1] != MAT + 0x40 * ids[i]) return 4;
                        if (arena[base + 16 + 3] != 48) return 5;
                        if (r32(base + 16 + 4) != DATA + off[i] * 16) return 6;
                        unsigned short count;
                        memcpy(&count, arena + base + 16, 2);
                        if (count != (len[i] & 0xffff)) return 7;
                    }
                }
            }
        }
    }
    // Opaque material only: result comes from the tess call alone.
    for (int tess : {0, 1, 7}) {
        memset(arena, 0, sizeof arena);
        w32(R + 19328, ENT); w32(ENT, BUF); w32(BUF + 64, 1); w32(BUF + 88, REC); w32(BUF + 96, DATA);
        w32(R + 19436, SETS); w32(SETS, SETOBJ); w32(SETOBJ + 12, MATARR); w32(MATARR, MAT); w32(R + 19492, DMA);
        tess_result = tess; norder = nadded = 0;
        if (DrawPrimBufferPackVUNCG__8VIRasteriiUi(arena + R, 0, 0, 3) != (tess ? 1 : 0)) return 10;
    }
    return 0;
}
