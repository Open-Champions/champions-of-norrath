// Host check only; not a build input. Pointer words hold arena offsets (NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "draw_vunc.cpp"
#include <initializer_list>
#include <string.h>

alignas(16) static unsigned char arena[0x8000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

static char order[32];
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

static const unsigned R = 0x1000, ENT = 0x3000, BUF = 0x3100, REC = 0x3200, SETS = 0x3300, SETOBJ = 0x3400, MATARR = 0x3500, MAT = 0x3600, DATA = 0x3800, DMA = 0x4000;
static void w32(unsigned off, unsigned v) { memcpy(arena + off, &v, 4); }
static unsigned r32(unsigned off) { unsigned v; memcpy(&v, arena + off, 4); return v; }

int main() {
    for (int flags = 0; flags < 4; ++flags) {
        for (int dirty : {0, 1}) {
            memset(arena, 0, sizeof arena);
            w32(R + 19328, ENT);
            w32(ENT + 12 * 2, BUF);                       // entry a = 2
            w32(BUF + 64, 3); w32(BUF + 88, REC); w32(BUF + 96, DATA);
            // three material records: ids 1, 0, 2; materials 0 and 2 opaque (word 60 == 0), 1 flagged
            unsigned ids[3] = {1, 0, 2}, off[3] = {5, 7, 9}, len[3] = {0x10003, 20, 30};
            for (int i = 0; i < 3; ++i) { w32(REC + 16 * i, ids[i]); w32(REC + 16 * i + 8, off[i]); w32(REC + 16 * i + 12, len[i]); }
            w32(R + 19436, SETS);
            w32(SETS + 12 * 1, SETOBJ);                   // entry b = 1
            w32(SETOBJ + 12, MATARR);
            for (int i = 0; i < 3; ++i) w32(MATARR + 4 * i, MAT + 0x40 * i);
            w32(MAT + 0x40 * 1 + 60, 9);
            w32(R + 17888, dirty);
            w32(R + 19492, DMA);
            norder = nadded = 0;
            int r = DrawPrimBufferVUNC__8VIRasteriiUi(arena + R, 2, 1, flags);
            // material k is drawn iff (flagged ? flags & 2 : flags & 1)
            bool drawn[3] = {(flags & 2) != 0, (flags & 1) != 0, (flags & 1) != 0};
            int tags = 0;
            char want[32]; int nw = 0;
            want[nw++] = 'F';
            if (dirty) { want[nw++] = 'C'; want[nw++] = 'U'; }
            for (int i = 0; i < 3; ++i) if (drawn[i]) { want[nw++] = 'A'; ++tags; }
            want[nw++] = 'F';
            if (norder != nw || memcmp(order, want, nw) != 0) return 1;
            if (r != 1) return 2;
            if (r32(R + 19492) != DMA + 32 * tags) return 3;
            int t = 0;
            for (int i = 0; i < 3; ++i) {
                if (!drawn[i]) continue;
                unsigned base = DMA + 32 * t++;
                if (added[t - 1] != MAT + 0x40 * ids[i]) return 4;
                // second 16 bytes of the pair: tag written by the draw step
                if (arena[base + 16 + 3] != 48) return 5;
                if (r32(base + 16 + 4) != DATA + off[i] * 16) return 6;
                unsigned short count;
                memcpy(&count, arena + base + 16, 2);
                if (count != (len[i] & 0xffff)) return 7;
            }
        }
    }
    // Only opaque materials, no flagged ones: result 0.
    memset(arena, 0, sizeof arena);
    w32(R + 19328, ENT); w32(ENT, BUF); w32(BUF + 64, 1); w32(BUF + 88, REC); w32(BUF + 96, DATA);
    w32(R + 19436, SETS); w32(SETS, SETOBJ); w32(SETOBJ + 12, MATARR); w32(MATARR, MAT); w32(R + 19492, DMA);
    norder = nadded = 0;
    if (DrawPrimBufferVUNC__8VIRasteriiUi(arena + R, 0, 0, 3) != 0) return 8;
    return 0;
}
