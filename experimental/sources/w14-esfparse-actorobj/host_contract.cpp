#include <stdint.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_actor_obj.cpp"

static std::string tr; static int version, find_ret, create_ret, end_ret, find_handle = 77;
static float floats[7]; static int float_calls; static unsigned id_value = 5, extra_value = 9;
static ActorVect3 got_first, got_second; static float got_scalar; static int got_handle, uc_calls, u_calls;
extern "C" int Read__9VIObjFileRUi(void* f, unsigned* v) { (void)f; *v = (u_calls++ & 1) ? extra_value : id_value; tr += "U;"; return 0; }
extern "C" int Read__9VIObjFileRf(void* f, float* v) { (void)f; *v = floats[float_calls++]; tr += "F;"; return 0; }
extern "C" int Read__9VIObjFileRUc(void* f, unsigned char* v) { (void)f; *v = static_cast<unsigned char>(++uc_calls); tr += "C;"; return 0; }
extern "C" unsigned ObjectVersion__9VIObjFile(void* f) { (void)f; return static_cast<unsigned>(version); }
extern "C" int ReadEnd__9VIObjFile(void* f) { (void)f; tr += "E;"; return end_ret; }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* d, unsigned id, int* t, int* h) {
    if (d != reinterpret_cast<void*>(0x30) || id != id_value) tr += "BADFIND"; *t = 1; *h = find_handle; tr += "Find;"; return find_ret; }
extern "C" int CreateActor__7VISceneRC7VIVect3T1fi(void* s, const ActorVect3* a, const ActorVect3* b, float sc, int h) {
    if (s != reinterpret_cast<void*>(0x20)) tr += "BADSCENE"; got_first = *a; got_second = *b; got_scalar = sc; got_handle = h; tr += "Create;"; return create_ret; }

static int run() {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[6] = 0x20; w[8] = 0x30; w[9] = 0x55; tr.clear(); float_calls = 0; uc_calls = 0; u_calls = 0;
    for (int i = 0; i < 7; ++i) floats[i] = 1.5f + i;
    return ParseActorObj__10VIESFParse(parse);
}
int main() {
    find_ret = 1; create_ret = 12; end_ret = 0; version = 0;
    // version 0: one unsigned id, seven floats, lookup, create, close
    if (run() != 12 || tr != "U;F;F;F;F;F;F;F;Find;Create;E;") return 1;
    if (got_first.x != 1.5f || got_first.y != 2.5f || got_first.z != 3.5f || got_second.x != 4.5f || got_second.y != 5.5f || got_second.z != 6.5f || got_scalar != 7.5f || got_handle != 77) return 2;
    // version 1 adds the extra unsigned word, version 2 adds four bytes
    version = 1; if (run() != 12 || tr != "U;U;F;F;F;F;F;F;F;Find;Create;E;") return 3;
    version = 2; if (run() != 12 || tr != "U;U;F;F;F;F;F;F;F;C;C;C;C;Find;Create;E;") return 4;
    // failures
    end_ret = -1; if (run() != -1) return 5;
    end_ret = 0; create_ret = -2; if (run() != -1 || tr.find("E;") != std::string::npos) return 6;
    create_ret = 0; if (run() != 0) return 7;
    find_ret = 0; if (run() != -1 || tr.find("Create") != std::string::npos) return 8;
    return 0;
}
