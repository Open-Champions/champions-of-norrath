#include <stdint.h>
#include <string.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_csprite_part_emitters.cpp"

static std::string tr; static int begin_ret, end_ret, init_ret, find_ret, count_in, rd, find_calls, fail_find; static unsigned short tag_in; static unsigned id_in;
static int init_count; static int ivals[32]; static unsigned char elems[1056 * 3];
static unsigned found_type, found_id;
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tag_in; *second = 0; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
extern "C" int Read__9VIObjFileRi(void*, int* v) { *v = (rd == 0) ? count_in : ivals[(rd - 1) % 7]; ++rd; tr += "I;"; return 0; }
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = id_in; tr += "U;"; return 0; }
extern "C" int Init__t7VIArray1Z20VICSpritePartEmitteri(void* a, int c) { init_count = c; tr += "In;";
    *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(a) + 4) = static_cast<unsigned>(reinterpret_cast<uintptr_t>(elems)); return init_ret; }
extern "C" int FindTyped__12VIDictionaryUi14VIResourceTypeRi(void* d, unsigned id, int type, int* h) {
    if (d != reinterpret_cast<void*>(0x30)) tr += "BADDICT"; found_id = id; found_type = type; *h = 700 + (int)id; tr += "F;";
    return find_calls++ == fail_find ? 0 : find_ret; }
alignas(8) static unsigned char sprite_mem[4616 + 16];
static int run(unsigned scene, unsigned flag) {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[3] = flag; w[6] = scene; w[8] = 0x30; w[9] = 0x55; tr.clear(); rd = 0; find_calls = 0; memset(elems, 0xee, sizeof elems);
    return ParseCSpritePartEmitters__10VIESFParseR9VICSprite(parse, sprite_mem); }
int main() {
    // the sprite must live below 4 GiB for the element pointer words
    begin_ret = end_ret = init_ret = 0; find_ret = 1; fail_find = -1; tag_in = 0x2960; count_in = 2; id_in = 9;
    for (int i = 0; i < 7; ++i) ivals[i] = 11 + i;
    if (run(0x20, 1) != 0 || tr != "B;I;In;U;F;I;I;I;I;I;I;I;U;F;I;I;I;I;I;I;I;E;" || init_count != 2) return 1;
    if (found_id != 9 || found_type != 18) return 2;
    int* e0 = reinterpret_cast<int*>(elems);
    if (e0[0] != 709 || e0[252] != 11 || e0[253] != (int)0xeeeeeeee || e0[254] != 13 || e0[255] != 14 || e0[256] != 15 || e0[257] != 16 || e0[258] != 17) return 3;
    int* e1 = reinterpret_cast<int*>(elems + 1056);
    if (e1[0] != 709 || e1[252] != 11 || e1[254] != 13) return 4;
    ivals[1] = 3; if (run(0x20, 1) != 0) return 5;
    if (e0[253] != (int)0xeeeeeeef) return 6;
    ivals[1] = 2; if (run(0x20, 1) != 0 || e0[253] != (int)0xeeeeeeee) return 7;
    if (run(0x20, 0) != 0 || tr.find("F;") != std::string::npos || e0[0] != -1) return 8;
    if (run(0, 1) != 0 || tr.find("F;") == std::string::npos) return 9;
    if (run(static_cast<unsigned>(-1352), 1) != 0 || tr.find("F;") != std::string::npos || e0[0] != -1) return 10;
    fail_find = 1; if (run(0x20, 1) != -1 || tr.find("E;") != std::string::npos || find_calls != 2) return 11;
    fail_find = -1; end_ret = -1; if (run(0x20, 1) != -1) return 12;
    end_ret = 0; count_in = 0; if (run(0x20, 1) != 0 || tr != "B;I;In;E;") return 13;
    count_in = -2; if (run(0x20, 1) != 0 || tr != "B;I;In;E;") return 14;
    count_in = 1; init_ret = -1; if (run(0x20, 1) != -1 || tr != "B;I;In;") return 15;
    init_ret = 0; tag_in = 0x2961; if (run(0x20, 1) != -1 || tr != "B;") return 16;
    tag_in = 0x2960; begin_ret = -1; if (run(0x20, 1) != -1 || tr != "B;") return 17;
    return 0;
}
