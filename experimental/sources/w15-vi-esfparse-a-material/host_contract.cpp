#include <stdint.h>
#include <string.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_material.cpp"

static std::string tr; static int count_in, init_ret, end_ret, find_ret, version, cb_calls, ru_n, rf_n, rc_n, ri_n; static unsigned flags_in[4], tex_in[4];
static unsigned cb_arg; static int matrix_dtors, matrix_ctors, wrap_in[4], blend_in[4];
static float lod_seen[4], rate_seen[4][2], xf_seen[4][9]; static unsigned char col_seen[4][4], emi_seen[4]; static int tex_handle_seen[4], fill_seen[4], uv_seen[4], tess_seen;
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
extern "C" int ObjectVersion__9VIObjFile(void*) { return version; }
extern "C" int Read__9VIObjFileRi(void*, int* v) { if (ri_n == 0) *v = count_in; else if (ri_n % 2 == 1) *v = wrap_in[(ri_n - 1) / 2 % 4]; else *v = blend_in[(ri_n - 1) / 2 % 4]; ++ri_n; tr += "i;"; return 0; }
static unsigned mat_flags = 1; static unsigned uq[16]; static unsigned un;
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = uq[un++]; ++ru_n; tr += "u;"; return 0; }
extern "C" int Read__9VIObjFileRUc(void*, unsigned char* v) { *v = (unsigned char)(0x40 + rc_n++); tr += "c;"; return 0; }
extern "C" int Read__9VIObjFileRf(void*, float* v) { *v = 1.0f + rf_n++; tr += "f;"; return 0; }
extern "C" int FindTyped__12VIDictionaryUi14VIResourceTypeRi(void* d, unsigned id, int type, int* h) {
    if (d != reinterpret_cast<void*>(0x30) || type != 1) tr += "BADFIND"; *h = 800 + (int)id; tr += "F" + std::to_string(id) + ";"; return find_ret; }
extern "C" void __10VIMatrix33(void*) { ++matrix_ctors; tr += "K;"; }
extern "C" void _$_10VIMatrix33(void*, int f) { ++matrix_dtors; if (f != 2) tr += "BADDTOR"; tr += "D;"; }
extern "C" int Init__10VIMateriali(void* m, int c) { if (m != reinterpret_cast<void*>(0x70) || c != count_in) tr += "BADINIT"; tr += "In;"; return init_ret; }
extern "C" void SetTessellate__10VIMateriali(void*, int e) { tess_seen = e; tr += "Te;"; }
extern "C" void SetEmissiveColor__10VIMaterialRC9VIColor32(void*, const unsigned char* c) { memcpy(emi_seen, c, 4); tr += "Em;"; }
extern "C" void SetLayerFillType__10VIMateriali18VIMaterialFillType(void*, int l, int t) { fill_seen[l] = t; tr += "Fi;"; }
extern "C" void SetLayerUVSet__10VIMaterialii(void*, int l, int s) { uv_seen[l] = s; tr += "Uv;"; }
extern "C" void SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(void*, int l, int h, void* r, void* d) {
    if (r != reinterpret_cast<void*>(0x90) || d != reinterpret_cast<void*>(0x30)) tr += "BADTEX"; tex_handle_seen[l] = h; tr += "Tx;"; }
extern "C" void SetLayerWrapMode__10VIMateriali18VIMaterialWrapMode(void*, int l, int m) { if (m != wrap_in[l]) tr += "BADWRAP"; tr += "Wr;"; }
extern "C" void SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(void*, int l, int m) { if (m != blend_in[l]) tr += "BADBLEND"; tr += "Bl;"; }
extern "C" void SetLayerColor__10VIMaterialiRC9VIColor32(void*, int l, const unsigned char* c) { memcpy(col_seen[l], c, 4); tr += "Co;"; }
extern "C" void SetLayerUVTransform__10VIMaterialiRC10VIMatrix33(void*, int l, const float* m) { memcpy(xf_seen[l], m, 36); tr += "Xf;"; }
extern "C" void SetLayerTextureLODBias__10VIMaterialif(void*, int l, float b) { lod_seen[l] = b; tr += "Lo;"; }
extern "C" void SetLayerUVRate__10VIMaterialiRC7VIVect2(void*, int l, const float* r) { rate_seen[l][0] = r[0]; rate_seen[l][1] = r[1]; tr += "Ra;"; }
static void callback(void* a) { cb_arg = static_cast<unsigned>(reinterpret_cast<uintptr_t>(a)); ++cb_calls; tr += "CB;"; }
static int run(bool with_cb) {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[1] = with_cb ? static_cast<unsigned>(reinterpret_cast<uintptr_t>(&callback)) : 0; w[2] = 0x1234; w[3] = 0x90; w[8] = 0x30; w[9] = 0x55;
    un = 0; { unsigned n = 0; if (version >= 2) uq[n++] = mat_flags; for (int l = 0; l < 4; ++l) { uq[n++] = flags_in[l]; uq[n++] = tex_in[l]; } }
    tr.clear(); ru_n = rf_n = rc_n = ri_n = 0; cb_calls = 0; matrix_ctors = matrix_dtors = 0;
    return ParseMaterial__10VIESFParseP10VIMaterial(parse, reinterpret_cast<void*>(0x70)); }
int main() {
    count_in = 2; init_ret = end_ret = 0; find_ret = 1; version = 3;
    flags_in[0] = 3; flags_in[1] = 0; flags_in[2] = 2; tex_in[0] = 9; tex_in[1] = 0; wrap_in[0] = 5; wrap_in[1] = 6; blend_in[0] = 7; blend_in[1] = 8;
    // the library classes are hosted in this process, so the callback pointer must be below 4 GiB: skipped when it is not
    bool cb_ok = reinterpret_cast<uintptr_t>(&callback) < 0x100000000ull;
    if (run(cb_ok) != 0 || matrix_ctors != 1 || matrix_dtors != 1) return 1;
    if (cb_ok && (cb_calls != 1 || cb_arg != 0x1234 || tr.substr(0, 5) != "K;CB;")) return 2;
    if (!cb_ok && tr.substr(0, 2) != "K;") return 2;
    if (tess_seen != 1 || emi_seen[0] != 0x40 || emi_seen[3] != 0x43) return 3;
    if (fill_seen[0] != 1 || uv_seen[0] != 1 || tex_handle_seen[0] != 809 || col_seen[0][0] != 0x44 || col_seen[0][3] != 0x47) return 4;
    if (fill_seen[1] != 0 || uv_seen[1] != 0 || tr.find("Ra;u;Fi;Uv;u;i;Wr;i;Bl;") == std::string::npos) return 5;
    if (xf_seen[0][0] != 1.0f || xf_seen[0][8] != 9.0f || lod_seen[0] != 10.0f || rate_seen[0][0] != 11.0f || rate_seen[0][1] != 12.0f || xf_seen[1][0] != 13.0f) return 6;
    version = 0; if (run(false) != 0 || lod_seen[0] != 0.8f || lod_seen[1] != 0.8f || tr.find("Em;") != std::string::npos || tr.find("Te;") != std::string::npos) return 7;
    version = 2; if (run(false) != 0 || tr.find("Te;") == std::string::npos || tr.find("Em;") != std::string::npos || lod_seen[0] != 10.0f) return 8;
    version = 3; mat_flags = 2; if (run(false) != 0 || tr.find("Te;") != std::string::npos) return 9;
    mat_flags = 1; find_ret = 0; if (run(false) != -1 || matrix_dtors != 1 || tr.find("Tx;") != std::string::npos || tr.find("E;") != std::string::npos) return 10;
    find_ret = 1; tex_in[0] = 0; tex_in[1] = 0; if (run(false) != 0 || tr.find("F9;") != std::string::npos) return 11;
    tex_in[0] = 9; end_ret = -1; if (run(false) != -1 || matrix_dtors != 1) return 12;
    end_ret = 0; init_ret = -1; if (run(false) != -1 || tr != "K;i;In;D;") return 13;
    init_ret = 0; count_in = 0; if (run(false) != 0 || tr.find("Fi;") != std::string::npos) return 14;
    count_in = -1; if (run(false) != 0 || tr.find("Fi;") != std::string::npos) return 15;
    return 0;
}
