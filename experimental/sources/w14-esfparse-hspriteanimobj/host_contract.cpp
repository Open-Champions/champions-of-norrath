#include <stdint.h>
#include <string.h>
#include <string>
static void host_callback(void* arg);
static void* host_address(unsigned word) { return word == 1 ? reinterpret_cast<void*>(host_callback) : reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_hsprite_anim_obj.cpp"

static std::string tr; static unsigned id_in;
static int find_ret, find_type, find_handle, create_ret, add_ret, init_ret, ref_ret, frame_ret, end_ret, version;
static int ints[16], int_calls, shorts_calls, floats_calls; static void* cb_arg;
static int init_args[6]; static float init_rate, init_scale; static int frame_calls, frame_fail_at;
static float float_records[8][8]; static short pack_records[8][8]; static int node_seen[8], frame_seen[8], keys_idx[4]; static float keys_val[4]; static int key_calls, key_numbers[4];
alignas(16) static unsigned char anim_mem[64];
static std::string num(long v) { return std::to_string(v); }
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = id_in; tr += "U;"; return 0; }
extern "C" int Read__9VIObjFileRi(void*, int* v) { *v = ints[int_calls++]; tr += "I;"; return 0; }
extern "C" int Read__9VIObjFileRs(void*, short* v) { *v = static_cast<short>(shorts_calls % 2 ? -(shorts_calls + 1) * 100 : (shorts_calls + 1) * 100); ++shorts_calls; tr += "S;"; return 0; }
extern "C" int Read__9VIObjFileRf(void*, float* v) { *v = 0.5f * (1 + floats_calls++); tr += "F;"; return 0; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
extern "C" unsigned ObjectVersion__9VIObjFile(void*) { return static_cast<unsigned>(version); }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* d, unsigned id, int* t, int* h) {
    if (d != reinterpret_cast<void*>(0x30) || id != id_in) tr += "BADFIND"; *t = find_type; *h = find_handle; tr += "Find;"; return find_ret; }
extern "C" int CreateAnimation__7VIScenei(void* s, int a) { if (s != reinterpret_cast<void*>(0x20) || a != 0x99) tr += "BADCREATE"; tr += "C;"; return create_ret; }
extern "C" unsigned char* Animation__7VIScenei(void*, int h) { if (h != create_ret) tr += "BADANIM"; tr += "An;"; return anim_mem; }
extern "C" int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void*, void* r, unsigned id, int t, int h) { if (r != anim_mem || id != id_in || t != 6 || h != create_ret) tr += "BADADD"; tr += "A;"; return add_ret; }
extern "C" int Init__13VIHSpriteAnim19VIHSpriteAnimFormatiiiff14VIPlaybackType(void* a, int f, int n, int fr, int k, float rate, float scale, int pb) {
    if (a != anim_mem) tr += "BADINIT"; init_args[0] = f; init_args[1] = n; init_args[2] = fr; init_args[3] = k; init_args[4] = pb; init_rate = rate; init_scale = scale; tr += "Init;"; return init_ret; }
extern "C" int SetNodeRefID__13VIHSpriteAnimii(void*, int node, int id) { tr += "R" + num(node) + "=" + num(id) + ";"; return ref_ret; }
extern "C" int SetNodeFrame__13VIHSpriteAnimiiR14VIHSpriteFrame(void*, int node, int frame, const AnimFrame* r) {
    node_seen[frame_calls & 7] = node; frame_seen[frame_calls & 7] = frame; memcpy(float_records[frame_calls & 7], r->v, sizeof r->v); tr += "F" + num(node) + "." + num(frame) + ";"; return ++frame_calls == frame_fail_at ? -1 : frame_ret; }
extern "C" int SetNodeFrame__13VIHSpriteAnimiiR18VIHSpritePackFrame(void*, int node, int frame, const AnimPackFrame* r) {
    node_seen[frame_calls & 7] = node; frame_seen[frame_calls & 7] = frame; memcpy(pack_records[frame_calls & 7], r->v, sizeof r->v); tr += "P" + num(node) + "." + num(frame) + ";"; return ++frame_calls == frame_fail_at ? -1 : frame_ret; }
extern "C" void SetKeyframe__13VIHSpriteAnimiif(void*, int key, int index, float value) { key_numbers[key_calls & 3] = key; keys_idx[key_calls & 3] = index; keys_val[key_calls & 3] = value; ++key_calls; tr += "K" + num(key) + ";"; }
static void host_callback(void* a) { cb_arg = a; tr += "cb;"; }

static int run(unsigned cb, int fmt, int nodes, int frames, int keys, int enc) {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[0] = 0x99; w[1] = cb; w[2] = 0x1234; w[6] = 0x20; w[8] = 0x30; w[9] = 0x55;
    tr.clear(); int_calls = 0; shorts_calls = 0; floats_calls = 0; frame_calls = 0; key_calls = 0; cb_arg = nullptr;
    memset(anim_mem, 0, sizeof anim_mem); *reinterpret_cast<int*>(anim_mem + 8) = enc;
    int k = 0; if (version >= 2) ints[k++] = fmt; ints[k++] = nodes; ints[k++] = frames; if (version >= 3) ints[k++] = keys; if (version != 0) ints[k++] = 9;
    ints[k++] = 21; ints[k++] = 22; ints[k++] = 23; ints[k++] = 24; ints[k++] = 25; ints[k++] = 26; ints[k++] = 27; ints[k++] = 28;
    return ParseHSpriteAnimObj__10VIESFParse(parse);
}
static std::string frames_trace(int node, int frames, char kind, const char* reader) {
    std::string s;
    for (int f = 0; f < frames; ++f) { for (int i = 0; i < 8; ++i) s += reader; s += std::string(1, kind) + num(node) + "." + num(f) + ";"; }
    return s;
}
int main() {
    id_in = 7; find_ret = 0; find_type = 6; find_handle = 11; create_ret = 6; add_ret = 0; init_ret = 0; ref_ret = 0; frame_ret = 0; end_ret = 0; frame_fail_at = 0; version = 3;
    unsigned scale_bits; float constant = 0x1.0002p-15f; memcpy(&scale_bits, &constant, 4); if (scale_bits != 0x38000100u) return 90;
    const std::string pre = "U;I;I;I;I;F;F;I;Find;C;An;A;Init;";
    // version 3, format 0: eight floats per frame; the header floats come first
    if (run(1, 0, 2, 2, 1, 0) != 6 || cb_arg != reinterpret_cast<void*>(0x1234)) return 1;
    if (tr != "cb;" + pre + "I;R0=21;" + frames_trace(0, 2, 'F', "F;") + "I;R1=22;" + frames_trace(1, 2, 'F', "F;") + "I;F;K0;E;") return 2;
    if (init_args[0] != 0 || init_args[1] != 2 || init_args[2] != 2 || init_args[3] != 1 || init_args[4] != 9 || init_rate != 0.5f || init_scale != 1.0f) return 3;
    for (int i = 0; i < 8; ++i) if (float_records[0][i] != 0.5f * (3 + i)) return 4;
    if (node_seen[3] != 1 || frame_seen[3] != 1 || keys_idx[0] != 23 || keys_val[0] != 0.5f * 35 || key_numbers[0] != 0) return 5;
    // format 1 with word 1: eight raw 16-bit values per frame
    if (run(0, 1, 1, 2, 0, 1) != 6 || tr != pre + "I;R0=21;" + frames_trace(0, 2, 'P', "S;") + "E;") return 6;
    { const short expected[8] = {100, -200, 300, -400, 500, -600, 700, -800}; for (int i = 0; i < 8; ++i) if (pack_records[0][i] != expected[i]) return 7; }
    // format 1 with word 0: converted to floats, rotation lanes scaled by the odd constant, translation by 1/512
    if (run(0, 1, 1, 1, 0, 0) != 6 || tr != pre + "I;R0=21;" + frames_trace(0, 1, 'F', "S;") + "E;") return 8;
    { const short raw[8] = {100, -200, 300, -400, 500, -600, 700, -800};
      for (int i = 0; i < 4; ++i) if (float_records[0][i] != static_cast<float>(raw[i]) * constant) return 9;
      for (int i = 4; i < 8; ++i) if (float_records[0][i] != static_cast<float>(raw[i]) * 0.001953125f) return 10; }
    // other format and word combinations are refused after Init
    if (run(0, 2, 1, 1, 0, 0) != -1 || tr.substr(tr.size() - 5) != "Init;") return 11;
    if (run(0, 0, 1, 1, 0, 1) != -1 || tr.substr(tr.size() - 5) != "Init;") return 12;
    if (run(0, 1, 1, 1, 0, 2) != -1 || tr.substr(tr.size() - 5) != "Init;") return 13;
    // no nodes, no frames, no keys
    if (run(0, 0, 0, 3, 2, 0) != 6 || tr != pre + "I;F;K0;I;F;K1;E;" || keys_idx[1] != 22) return 13 + 100;
    if (run(0, 0, 2, 0, 0, 0) != 6 || tr != "U;I;I;I;I;F;F;I;Find;C;An;A;Init;I;R0=21;I;R1=22;E;") return 14;
    // versions below 3 read fewer header fields
    version = 2; if (run(0, 0, 0, 0, 0, 0) != 6 || tr != "U;I;I;I;F;F;I;Find;C;An;A;Init;E;" || init_args[3] != 0) return 15;
    version = 1; if (run(0, 0, 0, 0, 0, 0) != 6 || tr != "U;I;I;F;F;I;Find;C;An;A;Init;E;" || init_args[0] != 0) return 16;
    version = 0; if (run(0, 0, 0, 0, 0, 0) != 6 || tr != "U;I;I;F;Find;C;An;A;Init;E;" || init_scale != 1.0f || init_args[4] != 0 || init_rate != 0.5f) return 17;
    version = 3;
    // failures
    frame_fail_at = 3; if (run(0, 0, 2, 2, 0, 0) != -1 || tr.find("F1.0;") == std::string::npos || tr.find("I;R1") == std::string::npos || tr.find("F1.1;") != std::string::npos) return 18;
    frame_fail_at = 0; ref_ret = -1; if (run(0, 0, 2, 2, 0, 0) != -1 || tr.find("F0.0") != std::string::npos) return 19;
    ref_ret = 0; init_ret = -1; if (run(0, 0, 2, 2, 0, 0) != -1 || tr.find("I;R0") != std::string::npos) return 20;
    init_ret = 0; end_ret = -1; if (run(0, 0, 0, 0, 0, 0) != -1) return 21;
    end_ret = 0;
    find_ret = 1; if (run(0, 0, 2, 2, 0, 0) != 11 || tr != "U;I;I;I;I;F;F;I;Find;E;") return 22;
    find_type = 3; if (run(0, 0, 2, 2, 0, 0) != -1 || tr != "U;I;I;I;I;F;F;I;Find;") return 23;
    find_type = 6; find_ret = 0; add_ret = -1; if (run(0, 0, 2, 2, 0, 0) != -1 || tr.find("Init") != std::string::npos) return 24;
    add_ret = 0; create_ret = -3; if (run(0, 0, 2, 2, 0, 0) != -1 || tr.find("An;") != std::string::npos) return 25;
    create_ret = 6; id_in = 0; if (run(0, 0, 0, 0, 0, 0) != 6 || tr.find("Find;") != std::string::npos || tr.find("A;") != std::string::npos) return 26;
    return 0;
}
