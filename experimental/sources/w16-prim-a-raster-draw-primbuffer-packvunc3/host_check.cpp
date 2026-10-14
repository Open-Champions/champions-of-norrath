// Host check only; not a build input. Pointer words hold arena offsets (NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "draw_packvunc3.cpp"
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
static int isect_result, isect_calls;
static unsigned char* isect_box;
static float isect_sphere[4];
extern "C" int Intersects__C6VIBBoxRC8VISphere(const unsigned char* box, const float* sphere) {
    order[norder++] = 'I';
    ++isect_calls;
    isect_box = const_cast<unsigned char*>(box);
    memcpy(isect_sphere, sphere, 16);
    return isect_result;
}

static const unsigned R = 0x1000, ENT = 0x3000, BUF = 0x3100, REC = 0x3200, SETS = 0x3300, SETOBJ = 0x3400, MATARR = 0x3500, MAT = 0x3600, DATA = 0x3800, DMA = 0x4000, QOBJ = 0x5000, QREC = 0x5200, QENT = 0x5400;
static void w32(unsigned off, unsigned v) { memcpy(arena + off, &v, 4); }
static unsigned r32(unsigned off) { unsigned v; memcpy(&v, arena + off, 4); return v; }
static void wf(unsigned char* base, unsigned off, float f) { memcpy(base + off, &f, 4); }

int main() {
    for (int flags = 0; flags < 4; ++flags) {
        for (int dirty : {0, 1}) {
            for (int vis : {0, 1}) {
                for (int hit : {0, 1}) {
            memset(arena, 0, sizeof arena);
            memset(scratchpad, 0, sizeof scratchpad);
            w32(R + 19328, ENT);
            w32(ENT + 12 * 1, BUF);
            w32(BUF + 64, 3); w32(BUF + 88, REC); w32(BUF + 96, DATA);
            for (int i = 0; i < 4; ++i) { float f = 10.0f + i; memcpy(arena + BUF + 48 + 4 * i, &f, 4); }
            unsigned ids[3] = {1, 0, 2}, off[3] = {5, 7, 9}, len[3] = {0x10003, 20, 30};
            for (int i = 0; i < 3; ++i) { w32(REC + 16 * i, ids[i]); w32(REC + 16 * i + 8, off[i]); w32(REC + 16 * i + 12, len[i]); }
            w32(R + 19436, SETS);
            w32(SETS + 12 * 2, SETOBJ);
            w32(SETOBJ + 12, MATARR);
            for (int i = 0; i < 3; ++i) w32(MATARR + 4 * i, MAT + 0x40 * i);
            w32(MAT + 0x40 * 1 + 60, 9);
            w32(R + 17888, dirty);
            w32(R + 19492, DMA);
            norder = nadded = 0;
                    // primitive buffer a = 1, run list object entry b = 0, material set entry c = 2
                    w32(R + 19364, QENT);
                    w32(QENT + 12 * 0, QOBJ);
                    w32(QOBJ + 12, 1); w32(QOBJ + 16, 3); w32(QOBJ + 36, QREC); w32(QOBJ + 40, DATA);
                    unsigned qid[3] = {1, 0, 2}, qoff[3] = {4, 6, 8};
                    for (int i = 0; i < 3; ++i) { w32(QREC + 8 * i, qid[i]); w32(QREC + 8 * i + 4, qoff[i]); }
                    wf(scratchpad, 0x100, 3.0f); wf(scratchpad, 0x104, 4.0f); wf(scratchpad, 0x108, 0.0f);
                    wf(scratchpad, 0x1cc, 10.0f);
                    wf(scratchpad, 0x190, -1.5f); wf(scratchpad, 0x194, 2.5f); wf(scratchpad, 0x198, 7.0f);
                    scratchpad[0x222] = vis;
                    w32(R + 17864, 0x7777);
                    isect_result = hit; isect_calls = 0;
                    int r = DrawPrimBufferPackVUNC__8VIRasteriiiUi(arena + R, 1, 0, 2, flags);
                    bool drawn[3] = {(flags & 2) != 0, (flags & 1) != 0, (flags & 1) != 0};
                    char want[32]; int nw = 0;
                    want[nw++] = 'F';
                    if (dirty) want[nw++] = 'C';
                    if (vis) want[nw++] = 'I';
                    want[nw++] = 'U';
                    for (int i = 0; i < 3; ++i) if (drawn[i]) want[nw++] = 'A';
                    want[nw++] = 'F';
                    if (norder != nw || memcmp(order, want, nw) != 0) return 1;
                    if (r != 1) return 2;
                    for (int i = 0; i < 4; ++i) { float f = 10.0f + i; if (memcmp(scratchpad + 0x210 + 4 * i, &f, 4) != 0) return 8; }
                    if (vis) {
                        if (isect_calls != 1 || isect_box != arena + BUF + 20) return 11;
                        float radius = 10.0f * (1.0f / sqrtf(25.0f));
                        if (isect_sphere[0] != -1.5f || isect_sphere[1] != 2.5f || isect_sphere[2] != 7.0f) return 12;
                        if (fabsf(isect_sphere[3] - radius) > 1e-5f) return 13;
                        if (r32(R + 17864) != (hit ? 1u : 0u)) return 14;
                    } else {
                        if (isect_calls != 0 || r32(R + 17864) != 0) return 15;
                    }
                    int tags = 0;
                    for (int i = 0; i < 3; ++i) tags += drawn[i];
                    if (r32(R + 19492) != DMA + 32 * tags) return 3;
                    int u = 0;
                    for (int i = 0; i < 3; ++i) {
                        if (!drawn[i]) continue;
                        unsigned base = DMA + 32 * u++;
                        if (added[u - 1] != MAT + 0x40 * qid[i]) return 4;
                        if (arena[base + 16 + 3] != 80) return 5;
                        if (r32(base + 16 + 4) != DATA + qoff[i] * 16) return 6;
                        unsigned short count;
                        memcpy(&count, arena + base + 16, 2);
                        if (count != 0) return 7;
                    }
                }
            }
        }
    }
    // Mismatch between a and the run list's word at +12: returns -1 after the first flush only.
    memset(arena, 0, sizeof arena);
    memset(scratchpad, 0, sizeof scratchpad);
    w32(R + 19328, ENT); w32(ENT, BUF); w32(R + 19364, QENT); w32(QENT, QOBJ); w32(QOBJ + 12, 5);
    norder = nadded = 0;
    if (DrawPrimBufferPackVUNC__8VIRasteriiiUi(arena + R, 0, 0, 0, 3) != -1) return 20;
    if (norder != 1 || order[0] != 'F') return 21;
    return 0;
}
