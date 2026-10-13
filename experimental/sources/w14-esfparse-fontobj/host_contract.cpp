#include <stdint.h>
#include <string.h>
#include <string>
static void host_callback(void* arg);
static void* host_address(unsigned word) { return word == 1 ? reinterpret_cast<void*>(host_callback) : reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_font_obj.cpp"

static std::string tr; static unsigned id_in;
static int find_ret, find_type, find_handle, create_ret, add_ret, init_ret, end_ret, lock_ret, unlock_ret, endchar_ret, begin_ret, readbuf_ret, begin_char_ret, conv_ret, ispal, palette_ret, locked_count;
static unsigned short second_out; static int us_calls; static int ints[16], int_calls, pitch_in, conv_fail_at, conv_calls, vec_null, begin_fail_at, begin_calls, end_fail_at, end_calls; static void* cb_arg; static int cb_calls;
alignas(16) static unsigned char font_mem[64];
alignas(16) static unsigned char table_mem[64];
alignas(16) static unsigned char dest_mem[4096];
alignas(16) static unsigned char palette_dest[256];
static std::string num(long v) { return std::to_string(v); }
extern "C" int ReadBegin__9VIObjFileRUsT1(void*, unsigned short* tag, unsigned short* second) { *tag = 0; *second = second_out; tr += "B;"; return begin_calls++ == begin_fail_at ? -1 : begin_ret; }
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = id_in; tr += "U;"; return 0; }
extern "C" int Read__9VIObjFileRUs(void*, unsigned short* v) { *v = static_cast<unsigned short>(0x41 + (us_calls++ % 3)); tr += "S;"; return 0; }
extern "C" int Read__9VIObjFileRi(void*, int* v) { *v = ints[int_calls++]; tr += "I;"; return 0; }
extern "C" int Read__9VIObjFilePUci(void*, unsigned char* b, int n) { tr += "R" + num(n) + ";"; if (b && n > 0 && n < 64 && b != table_mem) memset(b, 0x5a, n); return readbuf_ret; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_calls++ == end_fail_at ? -1 : end_ret; }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void*, unsigned id, int* t, int* h) { if (id != id_in) tr += "BADFIND"; *t = find_type; *h = find_handle; tr += "Find;"; return find_ret; }
extern "C" int CreateFont__8VIRasteri(void* r, int a) { if (r != reinterpret_cast<void*>(0x77) || a != 0x99) tr += "BADCREATE"; tr += "C;"; return create_ret; }
extern "C" void* Font__8VIRasteri(void*, int h) { if (h != create_ret) tr += "BADFONT"; tr += "F;"; return font_mem; }
extern "C" void* __builtin_vec_new(unsigned n) { tr += "N" + num(n) + ";"; return vec_null ? nullptr : table_mem; }
extern "C" void __builtin_vec_delete(void* p) { if (p != table_mem) tr += "BADDEL"; tr += "D;"; }
extern "C" int Init__7VIRFontR8VIRasterR12VIDictionaryiPUci10VIBitDepthi(void* f, void* r, void* d, int count, unsigned char* tab, int rows, int depth, int flags) {
    if (f != font_mem || r != reinterpret_cast<void*>(0x77) || d != reinterpret_cast<void*>(0x30) || tab != table_mem) tr += "BADINIT";
    tr += "Init" + num(count) + "," + num(rows) + "," + num(depth) + "," + num(flags) + ";"; return init_ret; }
extern "C" int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void*, void* r, unsigned id, int t, int h) { if (r != font_mem || id != id_in || t != 12 || h != create_ret) tr += "BADADD"; tr += "A;"; return add_ret; }
extern "C" int Lock__7VIRFontR8VIRaster(void*, void*) { tr += "L;"; return lock_ret; }
extern "C" int Unlock__7VIRFontR8VIRaster(void*, void*) { tr += "UL;"; return unlock_ret; }
extern "C" int IsPalettized__7VIRFont(void*) { tr += "IP;"; return ispal; }
extern "C" int Palette__7VIRFontRiR12VIPixelStorePPUc(void*, int* c, FontPixelStore* s, unsigned char** d) { *c = locked_count; memset(s, 0, sizeof *s); *d = palette_dest; tr += "P;"; return palette_ret; }
extern "C" FontPixelStore PixelStore__7VIRFont(void*) { FontPixelStore s = {{1, 2, 3, 4}}; tr += "PS;"; return s; }
extern "C" int BeginChar__7VIRFontUsRiPPUc(void*, unsigned short code, int* p, unsigned char** d) { *p = pitch_in; *d = dest_mem; tr += "BC" + num(code) + ";"; return begin_char_ret; }
extern "C" int EndChar__7VIRFont(void*) { tr += "EC;"; return endchar_ret; }
static int convert(const char* name, FontPixelStore* s, unsigned char* src, unsigned char* dst, int count) {
    tr += std::string(name) + ":" + num(count) + "@" + num(dst - dest_mem) + (s->bytes[3] == 4 ? "" : "?") + (src[0] == 0x5a || count == 0 ? "" : "!") + ";"; ++conv_calls; return conv_calls == conv_fail_at ? -1 : conv_ret; }
extern "C" int Convert4__10VIESFParseR12VIPixelStorePUcT2i(void*, FontPixelStore* st, unsigned char* s, unsigned char* d, int c) { return convert("c4", st, s, d, c); }
extern "C" int Convert8__10VIESFParseR12VIPixelStorePUcT2i(void*, FontPixelStore* st, unsigned char* s, unsigned char* d, int c) { return convert("c8", st, s, d, c); }
extern "C" int Convert16__10VIESFParseR12VIPixelStorePUcT2i(void*, FontPixelStore* st, unsigned char* s, unsigned char* d, int c) { return convert("c16", st, s, d, c); }
extern "C" int Convert24__10VIESFParseR12VIPixelStorePUcT2i(void*, FontPixelStore* st, unsigned char* s, unsigned char* d, int c) { return convert("c24", st, s, d, c); }
extern "C" int Convert32__10VIESFParseR12VIPixelStorePUcT2i(void*, FontPixelStore* st, unsigned char*, unsigned char* d, int c) {
    tr += "c32:" + num(c) + "@" + num(d == palette_dest ? -1 : d - dest_mem) + (st->bytes[3] == 4 || d == palette_dest ? "" : "?") + ";"; ++conv_calls; return conv_calls == conv_fail_at ? -1 : conv_ret; }
static void host_callback(void* a) { cb_arg = a; ++cb_calls; tr += "cb;"; }

static int run(unsigned cb, int count, int rows, int depth, int flags, unsigned short second, int entries, int rb) {
    alignas(8) unsigned char parse[2200] = {0}; unsigned* words = reinterpret_cast<unsigned*>(parse);
    words[0] = 0x99; words[1] = cb; words[2] = 0x1234; words[3] = 0x77; words[8] = 0x30; words[9] = 0x55;
    tr.clear(); int_calls = 0; conv_calls = 0; begin_calls = 0; end_calls = 0; cb_arg = nullptr; cb_calls = 0; us_calls = 0; second_out = second;
    int k = 0; ints[k++] = count; ints[k++] = rows; ints[k++] = depth; if (second) ints[k++] = flags;
    if (depth == 0 || depth == 1) ints[k++] = entries;
    for (int i = 0; i < 8; ++i) ints[k++] = rb;
    memset(table_mem, 0, sizeof table_mem);
    locked_count = 100;
    return ParseFontObj__10VIESFParse(parse);
}
int main() {
    id_in = 7; find_ret = 0; find_type = 12; find_handle = 11; create_ret = 6; add_ret = 0; init_ret = 0; end_ret = 0; lock_ret = 0; unlock_ret = 0; endchar_ret = 0; begin_ret = 0; readbuf_ret = 0; begin_char_ret = 0; conv_ret = 0; ispal = 1; palette_ret = 0; pitch_in = 16;
    conv_fail_at = 0; vec_null = 0; begin_fail_at = -1; end_fail_at = -1;
    // 16-bit font, two glyphs of two rows; the width table is all zero here, so the converters get 0
    if (run(1, 2, 2, 2, 0, 1, 0, 8) != 0 || cb_calls != 2 || cb_arg != reinterpret_cast<void*>(0x1234)) return 1;
    if (tr != "B;U;I;I;I;I;E;Find;C;F;N2;B;R2;E;Init2,2,2,0;A;L;B;PS;cb;S;I;BC65;R8;c16:0@0;R8;c16:0@16;EC;cb;S;I;BC66;R8;c16:0@0;R8;c16:0@16;EC;E;UL;D;") return 2;
    // flags default to 1 without the extra field; depth selects the converter; table value is the pixel count
    if (run(0, 1, 1, 4, 0, 0, 0, 8) != 0 || tr.find("Init1,1,4,1;") == std::string::npos || tr.find("c32:0@0;") == std::string::npos) return 3;
    if (run(0, 1, 1, 3, 0, 0, 0, 8) != 0 || tr.find("c24:") == std::string::npos) return 4;
    // palettised: entry count, palette bytes, conversion with min(entries, locked)
    if (run(0, 1, 1, 1, 9, 1, 300, 8) != 0 || tr.find("B;I;R1200;IP;P;c32:100@-1;PS;") == std::string::npos || tr.find("c8:0@0;") == std::string::npos) return 5;
    if (run(0, 1, 1, 0, 9, 1, 3, 8) != 0 || tr.find("R12;IP;P;c32:3@-1;PS;") == std::string::npos || tr.find("c4:0@0;") == std::string::npos) return 6;
    ispal = 0; if (run(0, 1, 1, 0, 9, 1, 3, 8) != 0 || tr.find("R12;IP;PS;") == std::string::npos) return 7;
    ispal = 1;
    // zero rows still call BeginChar/EndChar; zero glyphs skip the loop
    if (run(0, 2, 0, 2, 1, 1, 0, 8) != 0 || tr.find("BC65;EC;") == std::string::npos) return 8;
    if (run(0, 0, 2, 2, 1, 1, 0, 8) != 0 || tr.find("BC") != std::string::npos || tr.find("PS;E;UL;") == std::string::npos) return 9;
    // a depth above 4 fails in the first row and leaks the table
    if (run(0, 1, 1, 5, 1, 1, 0, 8) != -1 || tr.find("R8;") == std::string::npos || tr.find("D;") != std::string::npos) return 10;
    // failures
    conv_fail_at = 2; if (run(0, 1, 2, 2, 1, 1, 0, 8) != -1 || tr.find("EC;") != std::string::npos) return 11;
    conv_fail_at = 1; if (run(0, 1, 1, 1, 1, 1, 3, 8) != -1 || tr.find("PS;") != std::string::npos) return 12;
    conv_fail_at = 0; palette_ret = -1; if (run(0, 1, 1, 1, 1, 1, 3, 8) != -1 || tr.find("c32") != std::string::npos) return 13;
    palette_ret = 0; endchar_ret = -1; if (run(0, 2, 1, 2, 1, 1, 0, 8) != -1 || tr.find("BC66") != std::string::npos) return 14;
    endchar_ret = 0; begin_char_ret = -1; if (run(0, 1, 1, 2, 1, 1, 0, 8) != -1 || tr.find("R8") != std::string::npos) return 15;
    begin_char_ret = 0; unlock_ret = -1; if (run(0, 1, 1, 2, 1, 1, 0, 8) != -1 || tr.find("D;") != std::string::npos) return 16;
    unlock_ret = 0; end_fail_at = 2; if (run(0, 1, 1, 2, 1, 1, 0, 8) != -1 || tr.find("UL;") != std::string::npos) return 17;
    end_fail_at = 1; if (run(0, 1, 1, 2, 1, 1, 0, 8) != -1 || tr.find("Init") != std::string::npos) return 18;
    end_fail_at = 0; if (run(0, 1, 1, 2, 1, 1, 0, 8) != -1 || tr.find("Find") != std::string::npos) return 19;
    end_fail_at = -1; begin_fail_at = 2; if (run(0, 1, 1, 2, 1, 1, 0, 8) != -1 || tr.find("PS;") != std::string::npos) return 20;
    begin_fail_at = 1; if (run(0, 1, 1, 2, 1, 1, 0, 8) != -1 || tr.find("R1;") != std::string::npos) return 21;
    begin_fail_at = 0; if (run(0, 1, 1, 2, 1, 1, 0, 8) != -1 || tr != "B;") return 22;
    begin_fail_at = -1; lock_ret = -1; if (run(0, 1, 1, 2, 1, 1, 0, 8) != -1 || tr.back() != ';' || tr.find("L;B;") != std::string::npos) return 23;
    lock_ret = 0; add_ret = -1; if (run(0, 1, 1, 2, 1, 1, 0, 8) != -1 || tr.find("L;") != std::string::npos) return 24;
    add_ret = 0; init_ret = -1; if (run(0, 1, 1, 2, 1, 1, 0, 8) != -1 || tr.find("A;") != std::string::npos) return 25;
    init_ret = 0; readbuf_ret = -1; if (run(0, 1, 1, 2, 1, 1, 0, 8) != -1 || tr.find("Init") != std::string::npos) return 26;
    readbuf_ret = 0; vec_null = 1; if (run(0, 1, 1, 2, 1, 1, 0, 8) != -1 || tr.find("B;R") != std::string::npos) return 27;
    vec_null = 0; create_ret = -3; if (run(0, 1, 1, 2, 1, 1, 0, 8) != -1 || tr.find("N") != std::string::npos) return 28;
    create_ret = 6; find_ret = 1; if (run(0, 1, 1, 2, 1, 1, 0, 8) != 11 || tr != "B;U;I;I;I;I;E;Find;E;") return 29;
    find_type = 3; if (run(0, 1, 1, 2, 1, 1, 0, 8) != -1 || tr != "B;U;I;I;I;I;E;Find;") return 30;
    find_type = 12; find_ret = 0; id_in = 0; if (run(0, 1, 1, 2, 1, 1, 0, 8) != 0 || tr.find("Find") != std::string::npos || tr.find("A;") != std::string::npos) return 31;
    return 0;
}
