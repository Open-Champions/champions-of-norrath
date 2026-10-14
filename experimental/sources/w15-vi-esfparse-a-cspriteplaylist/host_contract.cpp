#include <stdint.h>
#include <string.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_csprite_play_list.cpp"

static std::string tr; static int begin_ret, end_ret, count_in, find_calls, fail_find, slot_in, val_in; static unsigned short tag_in, ver_in;
static unsigned ids[8]; static unsigned id_n; static float fl[8]; static unsigned fl_n;
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tag_in; *second = ver_in; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
static int ri_n;
extern "C" int Read__9VIObjFileRi(void*, int* v) { if (ri_n == 0) *v = count_in; else if (ri_n % 2 == 1) *v = slot_in; else *v = val_in; ++ri_n; tr += "I;"; return 0; }
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = ids[id_n++ % 8]; tr += "U;"; return 0; }
extern "C" int Read__9VIObjFileRf(void*, float* v) { *v = fl[fl_n++ % 8]; tr += "F;"; return 0; }
extern "C" int FindTyped__12VIDictionaryUi14VIResourceTypeRi(void* d, unsigned id, int type, int* h) {
    if (d != reinterpret_cast<void*>(0x30)) tr += "BADDICT"; *h = 500 + (int)id; tr += "T" + std::to_string(type) + "i" + std::to_string(id) + ";";
    return find_calls++ == fail_find ? 0 : 1; }
alignas(8) static unsigned char sprite[52 * 3 + 480];
static int run(unsigned dev) {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[4] = dev; w[8] = 0x30; w[9] = 0x55; tr.clear(); ri_n = 0; id_n = 0; fl_n = 0; find_calls = 0; memset(sprite, 0xee, sizeof sprite);
    return ParseCSpritePlayList__10VIESFParseR9VICSprite(parse, sprite); }
static int word(int off) { return *reinterpret_cast<int*>(sprite + off); }
static float fword(int off) { return *reinterpret_cast<float*>(sprite + off); }
int main() {
    begin_ret = end_ret = 0; fail_find = -1; tag_in = 0x2910; ver_in = 3; count_in = 1; slot_in = 1; val_in = 77;
    ids[0] = 11; ids[1] = 12; ids[2] = 13; ids[3] = 14; for (int i = 0; i < 8; ++i) fl[i] = 1.5f + i;
    if (run(1) != 0 || tr != "B;I;U;U;I;F;I;U;F;F;U;F;F;T6i11;T6i12;T14i13;T14i14;E;") return 1;
    int b = 52;
    if (word(b + 424) != 511 || word(b + 428) != 512 || fword(b + 432) != 1.5f || word(b + 436) != 77 || word(b + 448) != 513 || word(b + 452) != 514) return 2;
    if (fword(b + 456) != 2.5f || fword(b + 460) != 4.5f || fword(b + 464) != 3.5f || fword(b + 468) != 5.5f) return 3;
    if (word(424) != (int)0xeeeeeeee) return 4;
    ver_in = 2; if (run(1) != 0 || tr != "B;I;U;U;I;F;I;U;F;U;F;T6i11;T6i12;T14i13;T14i14;E;") return 5;
    if (fword(b + 464) != 0.0f || fword(b + 468) != 0.0f || fword(b + 456) != 2.5f || fword(b + 460) != 3.5f) return 6;
    ver_in = 1; if (run(1) != 0 || tr != "B;I;U;U;I;F;I;T6i11;T6i12;E;") return 7;
    if (word(b + 448) != -1 || word(b + 452) != -1 || fword(b + 456) != 1.0f || fword(b + 460) != 1.0f || fword(b + 464) != 0.0f) return 8;
    ver_in = 0; if (run(1) != 0 || tr != "B;I;U;I;F;I;T6i11;E;" || word(b + 428) != -1) return 9;
    ver_in = 3; if (run(0) != 0 || tr.find("T14") != std::string::npos || word(b + 448) != -1 || word(b + 452) != -1) return 10;
    ids[0] = 0; ids[1] = 0; if (run(1) != 0 || tr.find("T6") != std::string::npos || word(b + 424) != -1) return 11;
    ids[0] = 11; ids[1] = 12; ids[2] = 0; if (run(1) != 0 || tr.find("T14i13") != std::string::npos || word(b + 448) != -1) return 12;
    ids[2] = 13; fail_find = 3; if (run(1) != -1 || tr.find("E;") != std::string::npos) return 13;
    fail_find = 0; if (run(1) != -1 || tr != "B;I;U;U;I;F;I;U;F;F;U;F;F;T6i11;") return 14;
    fail_find = -1; end_ret = -1; if (run(1) != -1) return 15;
    end_ret = 0; count_in = 0; if (run(1) != 0 || tr != "B;I;E;") return 16;
    count_in = -1; if (run(1) != 0 || tr != "B;I;E;") return 17;
    count_in = 1; tag_in = 0x2911; if (run(1) != -1 || tr != "B;") return 18;
    tag_in = 0x2910; begin_ret = -2; if (run(1) != -1 || tr != "B;") return 19;
    return 0;
}
