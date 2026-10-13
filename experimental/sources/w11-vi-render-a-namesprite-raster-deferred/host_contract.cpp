extern "C" {
int _12VINameSprite$NumNameDefers;
unsigned char _12VINameSprite$NameDefers[3456];
}
#include "raster_deferred.cpp"
#include <string.h>

static int order[128]; static int order_n;
static void* q_base; static int q_count, q_size; static int (*q_cmp)(const void*, const void*);
static int q_num_at_call;
static int sort_marker;
static int isect_calls; static const void* isect_a[64]; static const void* isect_b[64];
static int copy_n; static void* copy_dst[24]; static const void* copy_src[24];
static void* mv_raster[24]; static const void* mv_matrix[24]; static int mv_n;
static void* dr_raster[24]; static int dr_id[24]; static float dr_pos[24][3]; static const void* dr_color[24]; static const char* dr_text[24]; static int dr_n;
extern "C" void qsort(void* b, int c, int s, int (*cmp)(const void*, const void*)) { q_base = b; q_count = c; q_size = s; q_cmp = cmp; q_num_at_call = _12VINameSprite$NumNameDefers; order[order_n++] = 1; }
extern "C" int VINameDeferSort__FPCvT0(const void*, const void*) { return sort_marker; }
extern "C" int Intersects__C6VIRectRC6VIRect(const void* a, const void* b) {
    isect_a[isect_calls] = a; isect_b[isect_calls++] = b; order[order_n++] = 2;
    int x, y; memcpy(&x, a, 4); memcpy(&y, b, 4); return x == y;
}
extern "C" void sceVu0CopyMatrix(void* d, const void* s) { copy_dst[copy_n] = d; copy_src[copy_n++] = s; memcpy(d, s, 64); order[order_n++] = 3; }
extern "C" void SetModelView__8VIRasterRC10VIMatrix44(void* r, const void* m) { mv_raster[mv_n] = r; mv_matrix[mv_n++] = m; order[order_n++] = 4; }
extern "C" void DrawCenteredString__8VIRasteriRC7VIVect3RC10VIColor32FPCc(void* r, int id, const void* pos, const void* col, const char* t) {
    dr_raster[dr_n] = r; dr_id[dr_n] = id; memcpy(dr_pos[dr_n], pos, 12); dr_color[dr_n] = col; dr_text[dr_n++] = t; order[order_n++] = 5;
}

static void fill(int n, const int* keys) {
    memset(_12VINameSprite$NameDefers, 0, sizeof _12VINameSprite$NameDefers);
    for (int i = 0; i < n; ++i) {
        unsigned char* r = _12VINameSprite$NameDefers + 144 * i;
        for (int b = 0; b < 144; ++b) r[b] = static_cast<unsigned char>(i * 16 + b + 1);
        memcpy(r + 56, &keys[i], 4);
        int id = 100 + i; memcpy(r + 4, &id, 4);
    }
    _12VINameSprite$NumNameDefers = n;
}

int main() {
    int raster;
    order_n = 0;
    // empty list: sorted, nothing drawn, count cleared
    _12VINameSprite$NumNameDefers = 0;
    if (RasterDeferred__12VINameSpriteR8VIRaster(&raster) != 0) return 1;
    if (q_base != _12VINameSprite$NameDefers || q_count != 0 || q_size != 144 || q_cmp != VINameDeferSort__FPCvT0) return 2;
    if (order_n != 1 || dr_n != 0 || mv_n != 0) return 3;

    // keys 1,2,1,3,2,4: records 2 and 4 intersect earlier kept ones and are dropped
    static const int keys[6] = {1, 2, 1, 3, 2, 4};
    static unsigned char original[6 * 144];
    fill(6, keys);
    memcpy(original, _12VINameSprite$NameDefers, sizeof original);
    order_n = 0; isect_calls = copy_n = mv_n = dr_n = 0;
    if (RasterDeferred__12VINameSpriteR8VIRaster(&raster) != 0) return 4;
    if (q_num_at_call != 6 || q_count != 6) return 5;
    if (_12VINameSprite$NumNameDefers != 0) return 6;
    // kept source records are 0, 1, 3, 5
    static const int kept_src[4] = {0, 1, 3, 5};
    if (copy_n != 4 || dr_n != 4 || mv_n != 4) return 7;
    unsigned char* rec = _12VINameSprite$NameDefers;
    for (int k = 0; k < 4; ++k) {
        const unsigned char* src = original + 144 * kept_src[k];
        unsigned char* dst = rec + 144 * k;
        if (memcmp(dst, src, 72) != 0) return 10 + k;
        if (memcmp(dst + 80, src + 80, 64) != 0) return 20 + k;
        if (copy_dst[k] != dst + 80) return 30 + k;
        if (mv_matrix[k] != dst + 80 || mv_raster[k] != &raster) return 40 + k;
        if (dr_raster[k] != &raster || dr_id[k] != 100 + kept_src[k] || dr_color[k] != dst + 40 || dr_text[k] != reinterpret_cast<const char*>(dst + 8)) return 50 + k;
        if (dr_pos[k][0] != 0.0f || dr_pos[k][1] != __builtin_bit_cast(float, 0x3f199999u) || dr_pos[k][2] != 0.0f) return 60 + k;
    }
    // the eight bytes at 72..79 of each destination record keep their old contents
    for (int k = 0; k < 4; ++k) {
        // slot k for k >= 2 was a source of a later record; compare against what the earlier fill left there
        if (k < 2) { for (int b = 72; b < 80; ++b) if (rec[144 * k + b] != original[144 * k + b]) return 70 + k; }
    }
    // comparisons: record 1 vs kept{0}; 2 vs {0} stop; 3 vs {0,1}; 4 vs {0,1,3}? 4 matches key 2 at kept 1; 5 vs {0,1,3}
    if (isect_calls != 1 + 1 + 2 + 2 + 3) return 80;
    // phase order: sort, then (compare/copy) per record, then draws
    if (order[0] != 1 || order[order_n - 1] != 5) return 81;
    // every record kept: all records copy onto themselves
    static const int distinct[3] = {7, 8, 9};
    fill(3, distinct);
    order_n = 0; isect_calls = copy_n = mv_n = dr_n = 0;
    RasterDeferred__12VINameSpriteR8VIRaster(&raster);
    if (copy_n != 3 || dr_n != 3 || isect_calls != 0 + 1 + 2) return 90;
    return 0;
}
