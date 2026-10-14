#include <stdint.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_resource_dir_obj.cpp"

static std::string tr; static int begin_ret, count, table_ret, obj_ret[8], obj_calls, end_rets[2], end_calls; static unsigned short next_tag;
static void* table_array;
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = next_tag; *second = 1; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void* f) { (void)f; tr += "E;"; return end_rets[end_calls++]; }
extern "C" int NumSubObjects__9VIObjFile(void* f) { (void)f; tr += "N;"; return count; }
extern "C" int ParseObject__10VIESFParse(void* p) { (void)p; tr += "O;"; return obj_ret[obj_calls++]; }
extern "C" int ParseResourceTable__10VIESFParseRt7VIArray1Z14VIResourceElem(void* p, void* a) { (void)p; table_array = a; tr += "T;"; return table_ret; }

static int run(int flag) {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse); w[9] = 0x55;
    tr.clear(); obj_calls = 0; end_calls = 0; table_array = nullptr;
    int array_marker; int r = ParseResourceDirObj__10VIESFParseiR13VIResourceDir(parse, flag, &array_marker);
    if (table_array && table_array != &array_marker) tr += "BADARRAY";
    return r;
}
int main() {
    next_tag = 0xa010; begin_ret = 0; count = 2; table_ret = 0; end_rets[0] = end_rets[1] = 0;
    if (run(0) != 0 || tr != "B;N;O;O;E;T;E;") return 1;
    if (run(1) != 0 || tr != "B;E;T;E;") return 2;
    count = 0; if (run(0) != 0 || tr != "B;N;E;T;E;") return 3;
    count = 3; obj_ret[1] = -1; if (run(0) != -1 || tr != "B;N;O;O;") return 4;
    obj_ret[1] = 0; end_rets[0] = -1; if (run(0) != -1 || tr != "B;N;O;O;O;E;") return 5;
    end_rets[0] = 0; table_ret = -1; if (run(0) != -1 || tr.substr(tr.size() - 4) != "E;T;") return 6;
    table_ret = 0; end_rets[1] = -4; if (run(0) != -1) return 7;
    next_tag = 0xa011; if (run(0) != -1 || tr != "B;") return 8;
    next_tag = 0xa010; begin_ret = -1; if (run(0) != -1 || tr != "B;") return 9;
    return 0;
}
