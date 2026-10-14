// Host check only; not a build input. Words hold low 32 bits of arena addresses (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "stretch_blit.cpp"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
alignas(16) static unsigned char arena[0x8000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }

static std::string trace;
static int add_result, begin_result, end_result;
extern "C" int UploadRasterMicro__8VIRasterPP10_sceDmaTag(unsigned char*, unsigned char*) { trace += "U;"; return 0; }
extern "C" void SetDither__10VIMateriali(unsigned char*, int e) { trace += e ? "D1;" : "D0;"; }
extern "C" int AddMaterial__8VIRasteriPP10_sceDmaTag(unsigned char*, int, unsigned char*) { trace += "A;"; return add_result; }
extern "C" int BlitBegin__8VIRasterPP10_sceDmaTag(unsigned char*, unsigned char*) { trace += "B;"; return begin_result; }
extern "C" int BlitEnd__8VIRasterPP10_sceDmaTag(unsigned char*, unsigned char*) { trace += "E;"; return end_result; }
extern "C" int FlushDMABufferIfFull__8VIRaster(unsigned char*) { trace += "F;"; return 0; }

static unsigned& W(unsigned char* b, int off) { return *reinterpret_cast<unsigned*>(b + off); }
static float& F(unsigned char* b, int off) { return *reinterpret_cast<float*>(b + off); }

int main() {
    bool ok = true;
    for (unsigned flags : {0u, 1u, 2u, 3u, 4u, 7u}) {
        for (int variant = 0; variant < 4; ++variant) {
            std::memset(arena, 0, sizeof arena);
            trace.clear();
            add_result = variant == 1 ? -1 : 0;
            begin_result = variant == 2 ? -1 : 0;
            end_result = variant == 3 ? -1 : 0;
            unsigned char* raster = arena;
            unsigned char* out = arena + 0x5000;
            unsigned char* table = arena + 0x5800;
            unsigned char* tex = arena + 0x5900;
            unsigned char* mat = arena + 0x5a00;
            W(raster, 19388) = word_of(table);
            W(raster, 17848) = word_of(mat);
            W(raster, 52) = word_of(out);
            W(raster, 48) = 10;
            W(raster, 19532) = 0;
            W(raster, 17852) = 5;
            W(raster, 17856) = 1;
            F(raster, 17732) = 1.0f; F(raster, 17736) = 2.0f; F(raster, 17740) = 0.5f; F(raster, 17744) = 0.25f;
            W(table, 12 * 3) = word_of(tex);
            W(tex, 8) = 64; W(tex, 12) = 32;
            int a[2] = {10, 20}, b[2] = {30, 40}, c[2] = {8, 4}, d[2] = {16, 8};
            int r = StretchBlit__8VIRasteriUiRC8VIVect2IN3316VIAlphaBlendModef(raster, 3, flags, (const Vect2I*)a, (const Vect2I*)b, (const Vect2I*)c, (const Vect2I*)d, 2, 0.75f);
            std::string want = "U;";
            want += (flags & 4) ? "D0;" : "D1;";
            want += "A;";
            if (variant != 1) want += "B;";
            if (variant == 0 || variant == 3) want += "E;";
            if (variant == 0) want += "F;";
            if (trace != want) ok = false;
            if (r != (variant == 0 ? 0 : -1)) ok = false;
            if (W(mat, 48) != (variant == 0 ? 0xffffffffu : 3u) || W(mat, 60) != 2u || W(mat, 32) != 1u) ok = false;
            if (W(raster, 17852) != 3u || W(raster, 17856) != 2u) ok = false;
            if (variant == 1 || variant == 2) {
                if (W(raster, 48) != 10u || W(raster, 52) != word_of(out)) ok = false;
                continue;
            }
            if (W(raster, 48) != 14u || W(raster, 52) != word_of(out) + 256) ok = false;
            // expected texture corners
            float l = 8, rt = 24, t = 4, bt = 12;
            if (flags & 1) { float s = l; l = rt; rt = s; }
            if (flags & 2) { float s = t; t = bt; bt = s; }
            const float xs[4] = {10, 40, 40, 10}, ys[4] = {20, 20, 60, 60};
            const float us[4] = {l, rt, rt, l}, vs[4] = {t, t, bt, bt};
            for (int i = 0; i < 4; ++i) {
                float* v = reinterpret_cast<float*>(out + 64 * i);
                if (v[0] != (xs[i] - 1.0f + 0.5f) * 0.5f || v[1] != (ys[i] - 2.0f + 0.5f) * -0.25f) ok = false;
                if (v[4] != us[i] * (1.0f / 64.0f) || v[5] != vs[i] * (1.0f / 32.0f)) ok = false;
                if (v[2] != 0 || v[3] != 1 || v[6] != 1 || v[7] != 0 || v[12] != 1 || v[13] != 1 || v[14] != 1 || v[15] != 0.75f) ok = false;
                for (int k = 8; k < 12; ++k) if (v[k] != 0) ok = false;
            }
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
