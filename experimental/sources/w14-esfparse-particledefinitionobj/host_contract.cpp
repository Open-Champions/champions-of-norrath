#include <stdint.h>
#include <string.h>
#include <string>
alignas(16) static unsigned char arena[0x20000];  // words 0x1000..0x1ffff index this arena
static unsigned char* scene_mem = arena + 0x2000;
static void* host_address(unsigned word) { return (word >= 0x1000 && word < 0x20000) ? static_cast<void*>(arena + word) : reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
alignas(16) static unsigned char definition_mem[1024];
static unsigned scene_word() { return 0x2000; }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_particle_definition_obj.cpp"

static std::string tr; static int id_in, find_ret, find_type, find_handle, create_ret, add_ret, end_rets[4], end_calls, begin_rets[4], begin_calls, surf_ret, pdef_ret, null_definition;
static unsigned short tags[4]; static void* system_seen; static void* added_resource; static void* pdef_definition;
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tags[begin_calls]; *second = 0; tr += "B;"; return begin_rets[begin_calls++]; }
extern "C" int Read__9VIObjFileRi(void*, int* v) { *v = id_in; tr += "I;"; return 0; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_rets[end_calls++]; }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* d, unsigned id, int* t, int* h) {
    if (d != reinterpret_cast<void*>(0x30) || static_cast<int>(id) != id_in) tr += "BADFIND"; *t = find_type; *h = find_handle; tr += "Find;"; return find_ret; }
extern "C" int CreateParticleDefinition__16VIParticleSystemi(void* s, int a) { system_seen = s; if (a != 0x99) tr += "BADARG"; tr += "C;"; return create_ret; }
extern "C" unsigned char* ParticleDefinition__16VIParticleSystemi(void* s, int h) { if (s != system_seen || h != create_ret) tr += "BADDEF"; tr += "D;"; return null_definition ? nullptr : definition_mem; }
extern "C" int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void*, void* r, unsigned id, int t, int h) {
    added_resource = r; if (static_cast<int>(id) != id_in || t != 18 || h != create_ret) tr += "BADADD"; tr += "A;"; return add_ret; }
extern "C" int ParseSurfaceObj__10VIESFParse(void*) { tr += "PS;"; return surf_ret; }
extern "C" int ParseParticleDefinition__10VIESFParseR9VIObjFileR20VIParticleDefinition(void*, void* f, void* d) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE2"; pdef_definition = d; tr += "PD;"; return pdef_ret; }

static int run(unsigned flag, unsigned scene_flag) {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[0] = 0x99; w[3] = flag; w[6] = scene_word(); w[8] = 0x30; w[9] = 0x55; tr.clear(); end_calls = 0; begin_calls = 0; added_resource = nullptr; pdef_definition = nullptr;
    *reinterpret_cast<unsigned*>(scene_mem + 19356) = scene_flag;
    return ParseParticleDefinitionObj__10VIESFParse(parse);
}
int main() {
    id_in = 7; find_ret = 0; find_type = 18; find_handle = 11; create_ret = 6; add_ret = 0; surf_ret = 0; pdef_ret = 0; null_definition = 0;
    for (int& e : end_rets) e = 0; for (int& b : begin_rets) b = 0; tags[0] = 0x1111; tags[1] = 0x1000; tags[2] = 0x1000; tags[3] = 0;
    // scene flag clear: the object is only closed
    if (run(1, 0) != 0 || tr != "E;") return 1;
    end_rets[0] = -1; if (run(1, 0) != -1) return 2;
    end_rets[0] = 0;
    // surface present, flag set
    if (run(1, 1) != 6 || tr != "B;I;E;Find;C;D;A;B;PS;B;PD;E;" || added_resource != definition_mem + 764 || pdef_definition != definition_mem) return 3;
    // surface present, flag clear: the sub-object is skipped
    if (run(0, 1) != 6 || tr != "B;I;E;Find;C;D;A;B;B;E;B;PD;E;") return 4;
    // any other tag goes straight to the definition parse
    tags[1] = 0x1002; if (run(1, 1) != 6 || tr != "B;I;E;Find;C;D;A;B;PD;E;") return 5;
    tags[1] = 0x1000;
    null_definition = 1; if (run(1, 1) != 6 || added_resource != nullptr || pdef_definition != nullptr) return 6;
    null_definition = 0;
    // dictionary hit
    find_ret = 1; if (run(1, 1) != 11 || tr != "B;I;E;Find;E;") return 7;
    find_type = 3; if (run(1, 1) != -1 || tr != "B;I;E;Find;") return 8;
    find_type = 18; find_ret = 0;
    id_in = 0; if (run(1, 1) != 6 || tr.find("Find") != std::string::npos || tr.find("A;") != std::string::npos) return 9;
    id_in = 7;
    surf_ret = -1; if (run(1, 1) != -1 || tr.find("PD") != std::string::npos) return 10;
    surf_ret = 0; pdef_ret = -1; if (run(1, 1) != -1 || tr.back() != ';' || tr.find("E;", tr.find("PD;")) != std::string::npos) return 11;
    pdef_ret = 0; add_ret = -1; if (run(1, 1) != -1 || tr != "B;I;E;Find;C;D;A;") return 12;
    add_ret = 0; create_ret = -4; if (run(1, 1) != -1 || tr != "B;I;E;Find;C;") return 13;
    create_ret = 6; end_rets[0] = -1; if (run(1, 1) != -1 || tr != "B;I;E;") return 14;
    end_rets[0] = 0; begin_rets[0] = -1; if (run(1, 1) != -1 || tr != "B;") return 15;
    begin_rets[0] = 0; begin_rets[1] = -1; if (run(1, 1) != -1 || tr.back() != ';' || tr.find("PS") != std::string::npos) return 16;
    begin_rets[1] = 0; begin_rets[2] = -1; if (run(1, 1) != -1 || tr.find("PD") != std::string::npos) return 17;
    begin_rets[2] = 0; end_rets[1] = -1; if (run(1, 1) != -1) return 18;
    return 0;
}
