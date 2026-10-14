#include <stdint.h>
#include <string.h>
#include <string>
alignas(16) static unsigned char arena[0x4000];  // words 0x1000..0x3fff index this arena
static void* host_address(unsigned word) { return (word >= 0x1000 && word < 0x4000) ? static_cast<void*>(arena + word) : reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_particle_sprite_obj.cpp"

static std::string tr; static int id_in, extra_in, find_ret, find_type, find_handle, create_ret, add_ret, end_rets[5], end_calls, begin_rets[3], begin_calls, def_ret, share_ret, int_calls;
static unsigned scene_word; static unsigned short tags[3], second_in; static void* sphere_seen; static const void* sphere_box; static int shared_def, pdef_arg; static void* emitter_seen; static void* emitter_arg; static void* record_seen; static void* system_seen;
alignas(16) static unsigned char sprite_mem[2048];
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tags[begin_calls]; *second = begin_calls == 0 ? second_in : 0; tr += "B;"; return begin_rets[begin_calls++]; }
extern "C" int Read__9VIObjFileRi(void*, int* v) { *v = (int_calls++ & 1) ? extra_in : id_in; tr += "I;"; return 0; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_rets[end_calls++]; }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* d, unsigned id, int* t, int* h) {
    if (d != reinterpret_cast<void*>(0x30) || static_cast<int>(id) != id_in) tr += "BADFIND"; *t = find_type; *h = find_handle; tr += "Find;"; return find_ret; }
extern "C" int CreateSprite__7VIScene12VISpriteTypei(void* s, int t, int a) { if (s != host_address(scene_word) || t != 10 || a != 0x99) tr += "BADCREATE"; tr += "C;"; return create_ret; }
extern "C" void* Sprite__7VIScenei(void*, int) { tr += "Sp;"; return sprite_mem; }
extern "C" int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void*, void* r, unsigned id, int t, int h) {
    if (r != sprite_mem || static_cast<int>(id) != id_in || t != 19 || h != create_ret) tr += "BADADD"; tr += "A;"; return add_ret; }
extern "C" void Init__8VISphereRC6VIBBox(void* s, const void* b) { sphere_seen = s; sphere_box = b; tr += "Init;"; }
extern "C" int ParseParticleDefinitionObj__10VIESFParse(void*) { tr += "PDef;"; return def_ret; }
extern "C" int ShareParticleDefinition__16VIParticleSystemi(void* s, int d) { system_seen = s; shared_def = d; tr += "Share;"; return share_ret; }
extern "C" void* ParticleDefinition__16VIParticleSystemi(void* s, int d) { if (s != system_seen) tr += "BADSYS"; pdef_arg = d; tr += "PD;"; return arena + 0x3800; }
extern "C" void Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition(void* e, void* s, void* d) { emitter_arg = e; if (s != system_seen) tr += "BADSYS2"; record_seen = d; tr += "Em;"; }
extern "C" void StaticBounds__C17VIParticleEmitter(ParticleBounds* r, void* e) { emitter_seen = e; for (int i = 0; i < 6; ++i) r->v[i] = 10.0f + i; tr += "SB;"; }

static int run(unsigned flag, unsigned scene) {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[0] = 0x99; w[3] = flag; w[6] = scene; scene_word = scene; w[8] = 0x30; w[9] = 0x55; tr.clear(); end_calls = 0; begin_calls = 0; int_calls = 0; sphere_seen = nullptr;
    memset(sprite_mem, 0xee, sizeof sprite_mem);
    return ParseParticleSpriteObj__10VIESFParse(parse);
}
static float fl(int off) { return *reinterpret_cast<float*>(sprite_mem + off); }
static int slot(int off) { return *reinterpret_cast<int*>(sprite_mem + off); }
int main() {
    id_in = 7; extra_in = 21; find_ret = 0; find_type = 19; find_handle = 11; create_ret = 6; add_ret = 0; def_ret = 4; share_ret = 0; second_in = 1;
    for (int& e : end_rets) e = 0; for (int& b : begin_rets) b = 0; tags[0] = 0x3000; tags[1] = 0xc000; tags[2] = 0;
    // note: Read ints alternate id/extra from a static counter, so every run reads (id, extra)
    const char* head = "B;I;I;E;Find;C;Sp;A;";
    int r = run(0x77, 0x2000);
    if (r != 6 || tr != std::string(head) + "B;PDef;Share;PD;Em;SB;Init;E;") return 1;
    if (slot(1072) != 21 || slot(64) != 4 || shared_def != 4 || pdef_arg != 4 || emitter_arg != sprite_mem + 80 || emitter_seen != sprite_mem + 80 || record_seen != arena + 0x3800) return 2;
    if (system_seen != arena + 0x2000 + 1352 || sphere_seen != sprite_mem + 32 || sphere_box != sprite_mem + 8) return 3;
    for (int i = 0; i < 6; ++i) if (fl(8 + 4 * i) != 10.0f + i) return 4;
    if (fl(44) != 1.0f || fl(32) != 0 || fl(36) != 0 || fl(40) != 0) return 5;
    // flag zero: only one sub-object is skipped; the unit box around the centre stays
    r = run(0, 0x2000);
    if (r != 6 || tr != std::string(head) + "B;E;E;" || fl(8) != -1.0f || fl(12) != -1.0f || fl(16) != -1.0f || fl(20) != 1.0f || fl(24) != 1.0f || fl(28) != 1.0f || slot(1072) != 21) return 6;
    // second output zero: extra is 0
    second_in = 0; r = run(0, 0x2000);
    if (r != 6 || tr != std::string("B;I;E;Find;C;Sp;A;B;E;E;") || slot(1072) != 0) return 7;
    second_in = 1;
    // zero system address: the same skip
    r = run(0x77, static_cast<unsigned>(-1352)); if (r != 6 || tr != std::string(head) + "B;E;E;") return 8;
    // other tag: ReadEnd is called, definition is -1, stored, fails
    tags[1] = 0xc001; r = run(0x77, 0x2000); if (r != -1 || tr != std::string(head) + "B;E;" || slot(64) != -1) return 9;
    tags[1] = 0xc000; begin_rets[1] = -1; r = run(0x77, 0x2000); if (r != -1 || tr != std::string(head) + "B;" || slot(64) != -1) return 10;
    begin_rets[1] = 0; def_ret = -3; r = run(0x77, 0x2000); if (r != -1 || slot(64) != -1 || tr.find("Share") != std::string::npos) return 11;
    def_ret = 4; end_rets[1] = -1; r = run(0x77, 0x2000); if (r != -1) return 12;
    end_rets[1] = 0;
    find_ret = 1; r = run(0x77, 0x2000); if (r != 11 || tr != "B;I;I;E;Find;E;") return 13;
    find_type = 3; r = run(0x77, 0x2000); if (r != -1 || tr != "B;I;I;E;Find;") return 14;
    find_type = 19; find_ret = 0;
    end_rets[0] = -1; r = run(0x77, 0x2000); if (r != -1 || tr != "B;I;I;E;") return 15;
    end_rets[0] = 0; add_ret = -1; r = run(0x77, 0x2000); if (r != -1 || tr.find("B;", 20) != std::string::npos) return 16;
    add_ret = 0; create_ret = -3; r = run(0x77, 0x2000); if (r != -1 || tr.find("Sp;") != std::string::npos) return 17;
    create_ret = 6; begin_rets[0] = -1; r = run(0x77, 0x2000); if (r != -1 || tr != "B;") return 18;
    begin_rets[0] = 0; id_in = 0; r = run(0x77, 0x2000); if (r != 6 || tr.find("Find") != std::string::npos || tr.find("A;") != std::string::npos) return 19;
    return 0;
}
