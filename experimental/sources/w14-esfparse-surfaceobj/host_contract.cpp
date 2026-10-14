#include <stdint.h>
#include <string.h>
#include <string>
static void host_callback(void* arg);
static void* host_address(unsigned word) { return word == 1 ? reinterpret_cast<void*>(host_callback) : reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_surface_obj.cpp"

static std::string tr; static unsigned id_in; static int find_ret, find_type, find_handle, create_ret, add_ret, init_ret, end_ret, lock_ret, lockp_ret, unlockp_ret, unlockm_ret, conv_ret, read_ret;
static int ints[16], int_calls, locked_count, pitch_in; static int conv_fail_at, conv_calls; static void* cb_arg;
alignas(16) static unsigned char surface_mem[128];
alignas(16) static unsigned char dest_mem[4096];
alignas(16) static unsigned char palette_dest[1024];
static std::string num(long v) { return std::to_string(v); }
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = id_in; tr += "U;"; return 0; }
extern "C" int Read__9VIObjFileRi(void*, int* v) { *v = ints[int_calls++]; tr += "I;"; return 0; }
extern "C" int Read__9VIObjFilePUci(void*, unsigned char* b, int n) { tr += "R" + num(n) + ";"; if (b) memset(b, 0x5a, n > 0 && n < 64 ? n : 0); return read_ret; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void*, unsigned id, int* t, int* h) { if (id != id_in) tr += "BADFIND"; *t = find_type; *h = find_handle; tr += "Find;"; return find_ret; }
extern "C" int CreateSurface__8VIRasteri(void* r, int a) { if (r != reinterpret_cast<void*>(0x77) || a != 0x99) tr += "BADCREATE"; tr += "C;"; return create_ret; }
extern "C" void* Surface__8VIRasteri(void*, int h) { if (h != create_ret) tr += "BADSURF"; tr += "S;"; return surface_mem; }
extern "C" int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void*, void* r, unsigned id, int t, int h) { if (r != surface_mem || id != id_in || t != 1 || h != create_ret) tr += "BADADD"; tr += "A;"; return add_ret; }
extern "C" int Init__9VISurfaceR8VIRasterii10VIBitDepthi(void* s, void* r, int w, int h, int d, int m) { if (s != surface_mem || r != reinterpret_cast<void*>(0x77)) tr += "BADINIT"; tr += "Init" + num(w) + "x" + num(h) + "d" + num(d) + "m" + num(m) + ";"; return init_ret; }
extern "C" int LockPalette__9VISurfaceRiR12VIPixelStorePPUc(void*, int* c, SurfacePixelStore* s, unsigned char** d) { *c = locked_count; memset(s, 0, sizeof *s); *d = palette_dest; tr += "LP;"; return lockp_ret; }
extern "C" int UnlockPalette__9VISurface(void*) { tr += "UP;"; return unlockp_ret; }
extern "C" int LockMipLevel__9VISurfaceiRiR12VIPixelStorePPUc(void*, int level, int* p, SurfacePixelStore* s, unsigned char** d) { *p = pitch_in; memset(s, 0, sizeof *s); *d = dest_mem; tr += "LM" + num(level) + ";"; return lock_ret; }
extern "C" int UnlockMipLevel__9VISurfacei(void*, int level) { tr += "UM" + num(level) + ";"; return unlockm_ret; }
static int convert(const char* name, unsigned char* src, unsigned char* dst, int count) {
    tr += std::string(name) + ":" + num(count) + "@" + num(dst - dest_mem) + (src[0] == 0x5a || count == 0 ? "" : "?") + ";"; ++conv_calls; return conv_calls == conv_fail_at ? -1 : conv_ret; }
extern "C" int Convert4__10VIESFParseR12VIPixelStorePUcT2i(void*, SurfacePixelStore*, unsigned char* s, unsigned char* d, int c) { return convert("c4", s, d, c); }
extern "C" int Convert8__10VIESFParseR12VIPixelStorePUcT2i(void*, SurfacePixelStore*, unsigned char* s, unsigned char* d, int c) { return convert("c8", s, d, c); }
extern "C" int Convert16__10VIESFParseR12VIPixelStorePUcT2i(void*, SurfacePixelStore*, unsigned char* s, unsigned char* d, int c) { return convert("c16", s, d, c); }
extern "C" int Convert24__10VIESFParseR12VIPixelStorePUcT2i(void*, SurfacePixelStore*, unsigned char* s, unsigned char* d, int c) { return convert("c24", s, d, c); }
extern "C" int Convert32__10VIESFParseR12VIPixelStorePUcT2i(void*, SurfacePixelStore*, unsigned char*, unsigned char* d, int c) {
    tr += "c32:" + num(c) + "@" + num(d == palette_dest ? -1 : d - dest_mem) + ";"; ++conv_calls; return conv_calls == conv_fail_at ? -1 : conv_ret; }
static void host_callback(void* a) { cb_arg = a; tr += "cb;"; }

static int run(unsigned raster, unsigned cb, int w, int h, int d, int m, int entries, int rb, int pal_flag, int mip_count) {
    alignas(8) unsigned char parse[2200] = {0}; unsigned* words = reinterpret_cast<unsigned*>(parse);
    words[0] = 0x99; words[1] = cb; words[2] = 0x1234; words[3] = raster; words[8] = 0x30; words[9] = 0x55;
    tr.clear(); int_calls = 0; conv_calls = 0; cb_arg = nullptr; memset(surface_mem, 0, sizeof surface_mem);
    *reinterpret_cast<int*>(surface_mem + 40) = pal_flag; *reinterpret_cast<int*>(surface_mem + 64) = mip_count;
    ints[0] = w; ints[1] = h; ints[2] = d; ints[3] = m; int k = 4; if (d == 0 || d == 1) ints[k++] = entries;
    for (int i = 0; i < 6; ++i) ints[k++] = rb;
    locked_count = 100;
    return ParseSurfaceObj__10VIESFParse(parse);
}
int main() {
    id_in = 7; find_ret = 0; find_type = 1; find_handle = 11; create_ret = 6; add_ret = 0; init_ret = 0; end_ret = 0; lock_ret = 0; lockp_ret = 0; unlockp_ret = 0; unlockm_ret = 0; conv_ret = 0; read_ret = 0; pitch_in = 16; conv_fail_at = 0;
    // no raster: only ReadEnd, result 0
    if (run(0, 0, 4, 4, 2, 1, 0, 8, 0, 1) != 0 || tr != "E;") return 1;
    // 16-bit surface, three levels but the surface only has two: widths 8,4 / heights 4,2
    if (run(0x77, 1, 8, 4, 2, 3, 0, 8, 0, 2) != 6 || cb_arg != reinterpret_cast<void*>(0x1234)) return 2;
    if (tr != "cb;U;Find;C;S;A;I;I;I;I;Init8x4d2m3;LM0;I;R8;c16:8@0;R8;c16:8@16;R8;c16:8@32;R8;c16:8@48;UM0;LM1;I;R8;c16:4@0;R8;c16:4@16;UM1;E;") return 3;
    // each depth selects its own converter; palettised depths read the palette first
    if (run(0x77, 0, 4, 1, 4, 1, 0, 8, 0, 1) != 6 || tr.find("c32:4@0;") == std::string::npos) return 4;
    if (run(0x77, 0, 4, 1, 3, 1, 0, 8, 0, 1) != 6 || tr.find("c24:4@0;") == std::string::npos) return 5;
    if (run(0x77, 0, 4, 1, 0, 1, 3, 8, 0, 1) != 6 || tr.find("I;R12;") == std::string::npos || tr.find("c4:4@0;") == std::string::npos || tr.find("LP;") != std::string::npos) return 6;
    if (run(0x77, 0, 4, 1, 1, 1, 3, 8, 1, 1) != 6 || tr.find("R12;LP;c32:3@-1;UP;LM0;") == std::string::npos || tr.find("c8:4@0;") == std::string::npos) return 7;
    // palette count is clamped to what the surface can hold
    if (run(0x77, 0, 4, 1, 1, 1, 300, 8, 1, 1) != 6 || tr.find("c32:100@-1;") == std::string::npos) return 8;
    // a depth above 4 fails at the first row
    if (run(0x77, 0, 4, 1, 5, 1, 0, 8, 0, 1) != -1 || tr.find("LM0;I;R8;") == std::string::npos || tr.find("UM0") != std::string::npos) return 9;
    // zero height: no rows but the level is still unlocked; zero mips or surface levels: nothing at all
    if (run(0x77, 0, 4, 0, 2, 1, 0, 8, 0, 1) != 6 || tr.find("LM0;I;UM0;E;") == std::string::npos) return 10;
    if (run(0x77, 0, 4, 4, 2, 0, 0, 8, 0, 1) != 6 || tr.find("LM") != std::string::npos) return 11;
    if (run(0x77, 0, 4, 4, 2, 2, 0, 8, 0, 0) != 6 || tr.find("LM") != std::string::npos) return 12;
    // width and height halve down to 1
    if (run(0x77, 0, 1, 1, 2, 3, 0, 8, 0, 3) != 6 || tr.find("c16:1@0;UM0;LM1;I;R8;c16:1@0;UM1;LM2;") == std::string::npos) return 13;
    // failures
    conv_fail_at = 2; if (run(0x77, 0, 8, 4, 2, 1, 0, 8, 0, 1) != -1 || tr.find("c16:8@16;") == std::string::npos || tr.find("c16:8@32") != std::string::npos) return 14;
    conv_fail_at = 0; read_ret = -1; if (run(0x77, 0, 8, 4, 2, 1, 0, 8, 0, 1) != -1 || tr.find("c16") != std::string::npos) return 15;
    read_ret = 0; unlockm_ret = -1; if (run(0x77, 0, 8, 4, 2, 1, 0, 8, 0, 1) != -1 || tr.find("E;") != std::string::npos) return 16;
    unlockm_ret = 0; lock_ret = -1; if (run(0x77, 0, 8, 4, 2, 1, 0, 8, 0, 1) != -1 || tr.find("I;R") != std::string::npos) return 17;
    lock_ret = 0; unlockp_ret = -1; if (run(0x77, 0, 4, 1, 1, 1, 3, 8, 1, 1) != -1 || tr.find("LM0") != std::string::npos) return 18;
    unlockp_ret = 0; lockp_ret = -1; if (run(0x77, 0, 4, 1, 1, 1, 3, 8, 1, 1) != -1 || tr.find("UP;") != std::string::npos) return 19;
    lockp_ret = 0; conv_fail_at = 1; if (run(0x77, 0, 4, 1, 1, 1, 3, 8, 1, 1) != -1 || tr.find("UP;") != std::string::npos) return 20;
    conv_fail_at = 0; init_ret = -1; if (run(0x77, 0, 4, 1, 1, 1, 3, 8, 1, 1) != -1 || tr.find("R12") != std::string::npos) return 21;
    init_ret = 0; end_ret = -1; if (run(0x77, 0, 4, 1, 2, 1, 0, 8, 0, 1) != -1) return 22;
    end_ret = 0;
    // dictionary: hit of type 1 returns the stored handle, other types fail, id 0 skips the lookup
    find_ret = 1; if (run(0x77, 0, 4, 1, 2, 1, 0, 8, 0, 1) != 11 || tr != "U;Find;E;") return 23;
    find_type = 4; if (run(0x77, 0, 4, 1, 2, 1, 0, 8, 0, 1) != -1 || tr != "U;Find;") return 24;
    find_type = 1; find_ret = 0; add_ret = -1; if (run(0x77, 0, 4, 1, 2, 1, 0, 8, 0, 1) != -1 || tr != "U;Find;C;S;A;") return 25;
    add_ret = 0; create_ret = -3; if (run(0x77, 0, 4, 1, 2, 1, 0, 8, 0, 1) != -1 || tr != "U;Find;C;") return 26;
    create_ret = 6; id_in = 0; if (run(0x77, 0, 4, 1, 2, 1, 0, 8, 0, 1) != 6 || tr.find("Find") != std::string::npos || tr.find("A;") != std::string::npos) return 27;
    return 0;
}
