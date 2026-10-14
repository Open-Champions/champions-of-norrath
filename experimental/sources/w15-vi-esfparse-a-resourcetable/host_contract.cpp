#include <stdint.h>
#include <string.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_resource_table.cpp"

static std::string tr; static int begin_ret, end_ret, init_ret, count_in, init_count; static unsigned short tag_in; static unsigned rd_n;
alignas(16) static unsigned char elems[16 * 8];
static uintptr_t table_seen;
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tag_in; *second = 0; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
extern "C" int Read__9VIObjFileRi(void*, int* v) { *v = count_in; tr += "I;"; return 0; }
extern "C" int Init__t7VIArray1Z14VIResourceElemi(void* t, int c) { table_seen = reinterpret_cast<uintptr_t>(t); init_count = c; tr += "In;";
    *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(t) + 4) = static_cast<unsigned>(reinterpret_cast<uintptr_t>(elems)); return init_ret; }
extern "C" int Read__9VIObjFileRUl(void*, unsigned long* v) { *v = 1000 + rd_n++; tr += "L;"; return 0; }
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = 1000 + rd_n++; tr += "U;"; return 0; }
static unsigned char table_obj[16];
static int run() { alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse); w[9] = 0x55; tr.clear(); rd_n = 0; memset(elems, 0, sizeof elems);
    return ParseResourceTable__10VIESFParseRt7VIArray1Z14VIResourceElem(parse, table_obj); }
int main() {
    begin_ret = end_ret = init_ret = 0; tag_in = 0x9000; count_in = 2;
    if (run() != 0 || tr != "B;I;In;L;U;U;L;U;U;E;" || init_count != 2 || table_seen != reinterpret_cast<uintptr_t>(table_obj)) return 1;
    unsigned* e = reinterpret_cast<unsigned*>(elems);
    if (e[0] != 1000 || e[2] != 1001 || e[3] != 1002 || e[4] != 1003 || e[6] != 1004 || e[7] != 1005 || e[1] != 0 || e[5] != 0) return 2;
    count_in = 0; if (run() != 0 || tr != "B;I;In;E;") return 3;
    count_in = -3; if (run() != 0 || tr != "B;I;In;E;" || init_count != -3) return 4;
    count_in = 1; end_ret = -1; if (run() != -1 || tr != "B;I;In;L;U;U;E;") return 5;
    end_ret = 0; init_ret = -1; if (run() != -1 || tr != "B;I;In;") return 6;
    init_ret = 2; if (run() != 0) return 7;
    tag_in = 0x9001; if (run() != -1 || tr != "B;") return 8;
    tag_in = 0x9000; begin_ret = -1; if (run() != -1 || tr != "B;") return 9;
    return 0;
}
