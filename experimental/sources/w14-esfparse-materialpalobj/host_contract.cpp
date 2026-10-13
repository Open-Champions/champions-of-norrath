#include <stdint.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_material_pal_obj.cpp"

static std::string tr; static unsigned id_in; static int find_ret, find_type, find_handle, create_ret, add_ret, end_rets[4], end_calls, begin_rets[16], begin_calls, count_in, version, surf_ret, mat_ret, null_entry, entries_seen, surface_calls;
static unsigned short tags[16]; static int created_count, created_arg, entry_index[4]; static void* record_added;
alignas(16) static unsigned char record_mem[16];
alignas(16) static unsigned char entry_mem[16];
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tags[begin_calls]; *second = 0; tr += "B;"; return begin_rets[begin_calls++]; }
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = id_in; tr += "U;"; return 0; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_rets[end_calls++]; }
extern "C" int NumSubObjects__9VIObjFile(void*) { tr += "N;"; return count_in; }
extern "C" int ObjectVersion__9VIObjFile(void*) { tr += "V;"; return version; }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* d, unsigned id, int* t, int* h) {
    if (d != reinterpret_cast<void*>(0x30) || id != id_in) tr += "BADFIND"; *t = find_type; *h = find_handle; tr += "Find;"; return find_ret; }
extern "C" int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void*, void* r, unsigned id, int t, int h) {
    record_added = r; if (id != id_in || t != 3 || h != create_ret) tr += "BADADD"; tr += "A;"; return add_ret; }
extern "C" int ParseSurfaceObj__10VIESFParse(void*) { ++surface_calls; tr += "PS;"; return surf_ret; }
extern "C" int CreateMaterialPal__8VIRasterii(void* r, int n, int a) { if (r != reinterpret_cast<void*>(0x77)) tr += "BADRASTER"; created_count = n; created_arg = a; tr += "C;"; return create_ret; }
extern "C" void* MaterialPal__8VIRasteri(void* r, int h) { if (r != reinterpret_cast<void*>(0x77) || h != create_ret) tr += "BADPAL"; tr += "MP;"; return record_mem; }
extern "C" void* GetMaterialPalEntry__8VIRasterii(void* r, int h, int i) { if (r != reinterpret_cast<void*>(0x77) || h != create_ret) tr += "BADENTRY"; entry_index[entries_seen++ & 3] = i; tr += "G;"; return null_entry ? nullptr : entry_mem; }
extern "C" int ParseMaterial__10VIESFParseP10VIMaterial(void*, void* m) { if (m != entry_mem) tr += "BADMAT"; tr += "PM;"; return mat_ret; }

static int run() {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[0] = 0x99; w[3] = 0x77; w[8] = 0x30; w[9] = 0x55; tr.clear(); end_calls = 0; begin_calls = 0; entries_seen = 0; surface_calls = 0; record_added = nullptr;
    return ParseMaterialPalObj__10VIESFParse(parse);
}
int main() {
    id_in = 7; find_ret = 0; find_type = 3; find_handle = 11; create_ret = 6; add_ret = 0; count_in = 2; version = 0; surf_ret = 0; mat_ret = 0; null_entry = 0;
    for (int& e : end_rets) e = 0; for (int& b : begin_rets) b = 0;
    tags[0] = 0x1111; tags[1] = 0x1101; tags[2] = 0x1100; tags[3] = 0x1100;
    const std::string head = "B;U;E;Find;V;";
    // version 0: entry list only
    if (run() != 6 || tr != head + "B;N;C;MP;A;B;G;PM;B;G;PM;E;E;" || created_count != 2 || created_arg != 0x99 || record_added != record_mem || entry_index[0] != 0 || entry_index[1] != 1) return 1;
    // version 1: surface array first, with the count re-read each pass
    version = 1; tags[1] = 0x1001; tags[2] = 0x1000; tags[3] = 0x1000; tags[4] = 0x1101; tags[5] = 0x1100; tags[6] = 0x1100;
    if (run() != 6 || tr != head + "B;N;B;PS;N;B;PS;N;E;B;N;C;MP;A;B;G;PM;B;G;PM;E;E;" || surface_calls != 2) return 2;
    tags[1] = 0x1002; if (run() != -1 || tr != head + "B;") return 3;
    tags[1] = 0x1001; tags[2] = 0x1002; if (run() != -1 || tr != head + "B;N;B;") return 4;
    tags[2] = 0x1000; surf_ret = -1; if (run() != -1 || tr != head + "B;N;B;PS;") return 5;
    surf_ret = 0; end_rets[1] = -1; if (run() != -1 || tr != head + "B;N;B;PS;N;B;PS;N;E;") return 6;
    end_rets[1] = 0; count_in = 0; tags[1] = 0x1001; tags[2] = 0x1101; if (run() != 6 || tr != head + "B;N;E;B;N;C;MP;A;E;E;") return 7;
    count_in = 2; version = 0; tags[1] = 0x1101; tags[2] = 0x1100;
    // entries
    null_entry = 1; if (run() != -1 || tr != head + "B;N;C;MP;A;B;G;") return 8;
    null_entry = 0; mat_ret = -1; if (run() != -1 || tr != head + "B;N;C;MP;A;B;G;PM;") return 9;
    mat_ret = 0; tags[3] = 0x1102; if (run() != -1 || tr.find("B;G;PM;B;") == std::string::npos || tr.substr(tr.size() - 2) != "B;") return 10;
    tags[3] = 0x1100; tags[2] = 0x1102; if (run() != -1 || tr != head + "B;N;C;MP;A;B;") return 11;
    tags[2] = 0x1100; tags[1] = 0x1102; if (run() != -1 || tr != head + "B;") return 12;
    tags[1] = 0x1101; add_ret = -1; if (run() != -1 || tr != head + "B;N;C;MP;A;") return 13;
    add_ret = 0; create_ret = -2; if (run() != -1 || tr != head + "B;N;C;") return 14;
    create_ret = 6; end_rets[0] = -1; if (run() != -1 || tr != "B;U;E;") return 15;
    end_rets[0] = 0; end_rets[1] = -1; if (run() != -1) return 16;
    end_rets[1] = 0; end_rets[2] = -1; if (run() != -1) return 17;
    end_rets[2] = 0;
    // dictionary hit and the id-zero shortcut
    find_ret = 1; if (run() != 11 || tr != "B;U;E;Find;E;") return 18;
    find_type = 4; if (run() != -1 || tr != "B;U;E;Find;") return 19;
    find_type = 3; find_ret = 0; id_in = 0;
    if (run() != 6 || tr.find("Find") != std::string::npos || tr.find("A;") != std::string::npos) return 20;
    id_in = 7; tags[0] = 0x1110; if (run() != -1 || tr != "B;") return 21;
    tags[0] = 0x1111; begin_rets[0] = -1; if (run() != -1 || tr != "B;") return 22;
    return 0;
}
