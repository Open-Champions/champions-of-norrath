#include <stdint.h>
#include <string.h>
#include <string>
#include <vector>
alignas(16) static unsigned char arena[0x8000];  // words 0x1000..0x7fff index this arena
static void* host_address(unsigned word) { return (word >= 0x1000 && word < 0x8000) ? static_cast<void*>(arena + word) : reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_spell_effect_obj.cpp"

static const unsigned kScene = 0x1000;
static const unsigned kRecords = 0x6000;  // list n uses kRecords + 0x400 * n
static std::string tr; static std::string num(long v) { return std::to_string(v); }
static int version, find_ret, find_type, find_handle, create_ret, effect_null, ecreate_ret, add_ret, end_rets[8], end_calls, begin_rets[8], begin_calls, count_in, obj_ret[4], obj_calls;
static unsigned short tags[8]; static std::vector<int> ri_values; static size_t ri_pos; static std::vector<unsigned> ui_values; static size_t ui_pos;
static int event_find_ret, event_handle_next; static int shared_particles, shared_sprites, particle_calls, sprite_calls; static void* system_seen;
static unsigned flag_word;
alignas(16) static unsigned char effect_mem[64];
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tags[begin_calls]; *second = 0; tr += "B;"; return begin_rets[begin_calls++]; }
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = ui_values[ui_pos++]; tr += "U;"; return 0; }
extern "C" int Read__9VIObjFileRi(void*, int* v) { *v = ri_values[ri_pos++]; tr += "I;"; return 0; }
extern "C" int Read__9VIObjFileRf(void*, float* v) { *v = 1.0f; tr += "F;"; return 0; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_rets[end_calls++]; }
extern "C" int NumSubObjects__9VIObjFile(void*) { tr += "N;"; return count_in; }
extern "C" unsigned ObjectVersion__9VIObjFile(void*) { return static_cast<unsigned>(version); }
extern "C" int ParseObject__10VIESFParse(void*) { tr += "O;"; return obj_ret[obj_calls++]; }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* d, unsigned id, int* t, int* h) {
    if (d != reinterpret_cast<void*>(0x30)) tr += "BADFIND";
    if (tr.find("CE;") == std::string::npos) { *t = find_type; *h = find_handle; tr += "Find;"; return find_ret; }
    *t = 77; *h = event_handle_next; tr += "Fd" + num(static_cast<int>(id)) + ";"; return event_find_ret; }
extern "C" int CreateSpellEffect__19VISpellEffectSystemi(void* s, int a) { system_seen = s; if (a != 0x99) tr += "BADARG"; tr += "CE;"; return create_ret; }
extern "C" void* SpellEffect__19VISpellEffectSystemi(void* s, int h) { if (s != system_seen || h != create_ret) tr += "BADSE"; tr += "SE;"; return effect_null ? nullptr : effect_mem; }
extern "C" int Create__13VISpellEffectP7VIScene(void* e, void* sc) { if (e != effect_mem || sc != host_address(kScene)) tr += "BADEC"; tr += "EC;"; return ecreate_ret; }
extern "C" int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void*, void* r, unsigned id, int t, int h) { if (r != effect_mem || id != 7 || t != 21 || h != create_ret) tr += "BADADD"; tr += "A;"; return add_ret; }
extern "C" void Create__16VISpellEventListiP7VIScene(void* list, int n, void* sc) {
    int idx = static_cast<int>((static_cast<unsigned char*>(list) - effect_mem - 12) / 12);
    if (sc != host_address(kScene)) tr += "BADSC";
    *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(list) + 8) = kRecords + 0x400 * idx; tr += "L" + num(idx) + ":" + num(n) + ";"; }
extern "C" void ShareParticleDefinition__16VIParticleSystemi(void* s, int d) { if (s != host_address(kScene + 1352)) tr += "BADPS"; shared_particles = d; ++particle_calls; tr += "SPD" + num(d) + ";"; }
extern "C" void ShareSprite__7VIScenei(void* s, int d) { if (s != host_address(kScene)) tr += "BADSS"; shared_sprites = d; ++sprite_calls; tr += "SS" + num(d) + ";"; }

static const char* kEvent = "U;I;I;I;I;F;F;F;F;F;F;";
static int run() {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[0] = 0x99; w[6] = kScene; w[8] = 0x30; w[9] = 0x55; tr.clear(); end_calls = 0; begin_calls = 0; obj_calls = 0; ri_pos = 0; ui_pos = 0; particle_calls = 0; sprite_calls = 0;
    *reinterpret_cast<unsigned*>(arena + kScene + 19356) = flag_word; memset(arena + kRecords, 0xee, 0x1000); memset(effect_mem, 0, sizeof effect_mem);
    return ParseSpellEffectObj__10VIESFParse(parse);
}
static int rec_int(int list, int event, int off) { return *reinterpret_cast<int*>(arena + kRecords + 0x400 * list + 44 * event + off); }
int main() {
    find_ret = 0; find_type = 21; find_handle = 11; create_ret = 6; effect_null = 0; ecreate_ret = 0; add_ret = 0; count_in = 2; version = 2; flag_word = 1;
    for (int& e : end_rets) e = 0; for (int& b : begin_rets) b = 0; for (int& o : obj_ret) o = 0;
    tags[0] = 0x2000; tags[1] = 0xc230; tags[2] = 0x2001; event_find_ret = 1; event_handle_next = 60;
    // counts 1,0,2: list 0 has one particle reference, list 2 has a sprite reference and an unrelated record
    ri_values = {7, 1, 21, 5, 22, 23, 0, 2, 21, 9, 22, 23, 21, 4, 22, 23}; ui_values = {3, 9, 2};
    int r = run();
    const std::string head = "B;I;E;Find;B;N;O;O;E;CE;SE;EC;A;B;";
    if (r != 60 && r != 60) { return 1; }
    if (tr != head + "I;L0:1;" + kEvent + "Fd5;SPD60;" + "I;L1:0;" + "I;L2:2;" + kEvent + "Fd9;SS60;" + kEvent + "E;E;") return 2;
    if (rec_int(0, 0, 8) != 60 || rec_int(2, 0, 8) != 60 || rec_int(2, 1, 8) != 4 || rec_int(0, 0, 12) != 22) return 3;
    if (shared_particles != 60 || shared_sprites != 60 || particle_calls != 1 || sprite_calls != 1) return 4;
    // the return value is the last event lookup's handle when there was one, else the effect handle
    ri_values = {7, 0, 0, 0}; ui_values = {}; if (run() != 6) return 5;
    // flag word clear: references become -1 without lookups
    flag_word = 0; ri_values = {7, 1, 21, 5, 22, 23, 0, 2, 21, 9, 22, 23, 21, 4, 22, 23}; ui_values = {3, 9, 2};
    if (run() != 6 || rec_int(0, 0, 8) != -1 || rec_int(2, 0, 8) != -1 || rec_int(2, 1, 8) != 4 || tr.find("Fd") != std::string::npos || tr.find("O;") != std::string::npos) return 6;
    flag_word = 1;
    // -1 and 0 references become -1 without a lookup
    ri_values = {7, 2, 21, -1, 22, 23, 21, 0, 22, 23, 0, 0}; ui_values = {8, 3}; if (run() != 6 || rec_int(0, 0, 8) != -1 || rec_int(0, 1, 8) != -1 || tr.find("Fd") != std::string::npos) return 7;
    // type 11 is a sprite reference, type 1 gets 5000 below version 2 and nothing at or above it
    ri_values = {7, 1, 21, 3, 22, 23, 0, 0}; ui_values = {11}; if (run() != 60 || sprite_calls != 1 || shared_sprites != 60) return 8;
    version = 1; ri_values = {7, 1, 21, 3, 22, 23, 0, 0}; ui_values = {1};
    if (run() != 6 || rec_int(0, 0, 12) != 5000) return 9;
    version = 2; if (run() != 6 || rec_int(0, 0, 12) != 22) return 10;
    // version 0 has no nested block header; version 1 and above do
    version = 0; ri_values = {7, 0, 0, 0}; ui_values = {}; if (run() != 6 || tr != "B;I;E;Find;CE;SE;EC;A;B;I;L0:0;I;L1:0;I;L2:0;E;E;") return 11;
    version = 3; tags[1] = 0xc230; if (run() != 6 || tr != "B;I;E;Find;B;N;O;O;E;CE;SE;EC;A;B;I;L0:0;I;L1:0;I;L2:0;E;E;") return 12;
    // another tag after the id: that header is the one already consumed, so none is read after the effect is made
    tags[1] = 0xc231; if (run() != 6 || tr != "B;I;E;Find;B;CE;SE;EC;A;I;L0:0;I;L1:0;I;L2:0;E;E;") return 13;
    tags[1] = 0xc230; flag_word = 0; if (run() != 6 || tr != "B;I;E;Find;B;E;CE;SE;EC;A;B;I;L0:0;I;L1:0;I;L2:0;E;E;") return 14;
    flag_word = 1; version = 2;
    // failures
    ri_values = {7, 1, 21, 5, 22, 23, 0, 0}; ui_values = {3};
    event_find_ret = 0; if (run() != -1 || tr.find("SPD") != std::string::npos) return 15;
    event_find_ret = 1; end_rets[2] = -1; if (run() != -1 || tr.back() != ';' || tr.substr(tr.size() - 2) != "E;") return 16;
    end_rets[2] = 0; end_rets[3] = -1; if (run() != -1) return 17;
    end_rets[3] = 0; begin_rets[2] = -1; if (run() != -1 || tr.find("L0") != std::string::npos) return 18;
    begin_rets[2] = 0; add_ret = -1; if (run() != -1 || tr.find("L0") != std::string::npos) return 19;
    add_ret = 0; ecreate_ret = -1; if (run() != -1 || tr.find("A;") != std::string::npos) return 20;
    ecreate_ret = 0; effect_null = 1; if (run() != -1 || tr.find("EC;") != std::string::npos) return 21;
    effect_null = 0; create_ret = -1; if (run() != -1 || tr.find("SE;") != std::string::npos) return 22;
    create_ret = -2; if (run() == -1) return 23;   // only exactly -1 is treated as failure
    create_ret = 6; obj_ret[1] = -1; if (run() != -1 || tr.find("CE;") != std::string::npos) return 24;
    obj_ret[1] = 0; end_rets[1] = -1; if (run() != -1 || tr.find("CE;") != std::string::npos) return 25;
    end_rets[1] = 0; begin_rets[1] = -1; if (run() != -1 || tr.find("N;") != std::string::npos) return 26;
    begin_rets[1] = 0;
    // dictionary hit, ReadEnd after the id, ReadBegin first
    find_ret = 1;  if (run() != 11 || tr != "B;I;E;Find;E;") return 27;
    find_type = 3; if (run() != -1 || tr != "B;I;E;Find;") return 28;
    find_type = 21; find_ret = 0; end_rets[0] = -1; if (run() != -1 || tr != "B;I;E;") return 29;
    end_rets[0] = 0; begin_rets[0] = -1; if (run() != -1 || tr != "B;") return 30;
    begin_rets[0] = 0; ri_values = {0, 0, 0, 0}; if (run() != 6 || tr.find("Find;") != std::string::npos || tr.find("A;") != std::string::npos) return 31;
    return 0;
}
