#include "../detail.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int stage;
static unsigned char object[1300];
static unsigned char model[16];
static unsigned char header_pool[16][16];
static int header_next;
static bool creature_sets_flag;
static unsigned rolls[1];
static float seen_scale; static void* seen_map; static char** seen_args; static short seen_type;
static float params[5]; static int param_flag;
static float scanned_float;
static char sscanf_text[32]; static int sscanf_calls;
static char lump_names[16][48], resource_names[16][48]; static int lump_calls;
static void* chunk_arg; static int chunk_class;
static int set_state_calls; static int set_state_values[4];
static int run_list_calls, slow_list_calls, anim_add_calls, anim_flag, randomize_calls;
static void* anim_header;
static int path_calls; static void* path_state; static char* path_tag; static void* path_point;
static int specific_calls; static char specific_first[48], specific_second[48];
static bool tag_present[16]; static bool bit_present[32]; static int oneshot;
static const char* tag_text[16];
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static unsigned fb(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
#define VIEW(name, size) unsigned char norrath_ctors_ant_lantern_view_##name[size]
VIEW(44c080, 4); VIEW(480d28, 4);
VIEW(46dc90, 8); VIEW(485160, 8); VIEW(485170, 8); VIEW(46dca0, 8); VIEW(46dcc0, 8); VIEW(46dce0, 8); VIEW(46dd00, 8); VIEW(46dd20, 8); VIEW(46dd40, 8); VIEW(46dd60, 8);
VIEW(485190, 8); VIEW(485198, 8);
VIEW(485158, 16) = "%s.lmp"; VIEW(485168, 16) = "%s.vif"; VIEW(485178, 16) = "%s.tex"; VIEW(485180, 16) = "%s.anm"; VIEW(485188, 16) = "%f";
VIEW(46dcb0, 16) = "%s_i"; VIEW(46dcd0, 16) = "%s_m"; VIEW(46dcf0, 16) = "%s_f"; VIEW(46dd10, 16) = "%s_l"; VIEW(46dd30, 16) = "%s_o"; VIEW(46dd50, 16) = "%s_p";
VIEW(46dd70, 16) = "chg%d";
unsigned char norrath_actor_view_4844a4[4] = {4, 3, 2, 1};
unsigned char norrath_factory_ctype_minimum_view[384];
static unsigned char* const tag_views[10] = {norrath_ctors_ant_lantern_view_46dc90, norrath_ctors_ant_lantern_view_485160, norrath_ctors_ant_lantern_view_485170, norrath_ctors_ant_lantern_view_46dca0,
    norrath_ctors_ant_lantern_view_46dcc0, norrath_ctors_ant_lantern_view_46dce0, norrath_ctors_ant_lantern_view_46dd00, norrath_ctors_ant_lantern_view_46dd20,
    norrath_ctors_ant_lantern_view_46dd40, norrath_ctors_ant_lantern_view_46dd60};
extern "C" unsigned norrath_actor_random(void*) { return rolls[0]; }
extern "C" unsigned norrath_creature_construct(void*, const void* point, short type, void* map, float scale, char** args) {
    expect(stage == 0 && point != nullptr, "creature first");
    stage = 1; seen_scale = scale; seen_map = map; seen_args = args; seen_type = type;
    const float f = 7.9f;
    std::memcpy(object + 788, &f, 4);
    std::memset(object + 12, 0, 4);
    std::memset(object + 980, 0, 4);
    if (creature_sets_flag) { const unsigned one = 1u; std::memcpy(object + 980, &one, 4); }
    return 0;
}
extern "C" unsigned norrath_actor_set_parameters(void*, float a, float b, float c, float d, float e, int f) {
    expect(stage == 1, "parameters after creature");
    stage = 2;
    params[0] = a; params[1] = b; params[2] = c; params[3] = d; params[4] = e; param_flag = f;
    return 0;
}
extern "C" char* norrath_actor_find_tag_string(void*, char* tag, int) {
    expect(stage >= 2, "tags after parameters");
    for (int i = 0; i < 10; ++i) if (tag == reinterpret_cast<char*>(tag_views[i])) return tag_present[i] ? const_cast<char*>(tag_text[i]) : nullptr;
    if (tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_485190)) return const_cast<char*>("pathtext");
    for (int bit = 0; bit < 32; ++bit) {
        char expected[16];
        std::snprintf(expected, sizeof expected, "chg%d", bit);
        if (std::strcmp(tag, expected) == 0) return bit_present[bit] ? tag : nullptr;
    }
    expect(false, "unknown string tag");
    return nullptr;
}
extern "C" int norrath_game_object_find_tag_int(void*, char* tag, int) { expect(tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_485198), "oneshot tag"); return oneshot; }
extern "C" int norrath_factory_sprintf(char* dst, const char* fmt, ...) {
    va_list ap; va_start(ap, fmt);
    const int n = std::vsnprintf(dst, 48, fmt, ap);
    va_end(ap);
    return n;
}
extern "C" int sscanf(const char* text, const char* format, ...) {
    va_list ap; va_start(ap, format);
    float* out = va_arg(ap, float*);
    va_end(ap);
    ++sscanf_calls; std::strcpy(sscanf_text, text);
    expect(format == reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_485188), "scan format");
    *out = scanned_float;
    return 1;
}
extern "C" void* norrath_lump_find_resource(char* lump, char* res) {
    std::strcpy(lump_names[lump_calls], lump); std::strcpy(resource_names[lump_calls], res);
    ++lump_calls;
    if (res[0] == 'F') return nullptr;     // lookups whose text starts with 'F' fail
    return header_pool[header_next++];
}
extern "C" void norrath_actor_animation_initialize(void*) { expect(stage == 2, "animation init after lookups"); stage = 3; }
extern "C" void gameLineInitPathState__FP17gameLinePathStatePcP6Point3(void* s, char* tag, void* p) { ++path_calls; path_state = s; path_tag = tag; path_point = p; expect(stage == 3, "path state after init"); }
extern "C" void norrath_actor_animation_add(void*, void* h, int f) { ++anim_add_calls; anim_header = h; anim_flag = f; }
extern "C" void norrath_actor_animation_randomize_time(void*) { ++randomize_calls; }
extern "C" void setState__7Critteri(void*, int s) {
    set_state_values[set_state_calls++] = s;
    if (s == 12) { object[572] = 12; object[573] = 0; }
}
extern "C" void specificInit__7CritterPcT1(void*, char* a, char* b) { ++specific_calls; std::strcpy(specific_first, a); std::strcpy(specific_second, b); expect(stage == 3, "specific init after the bit loop"); }
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int c) { expect(stage == 3 && specific_calls == 1, "chunks after specific init"); chunk_arg = m; chunk_class = c; return 0xaabbccdd11223344ull; }
extern "C" unsigned norrath_actor_add_run_list(void*) { ++run_list_calls; return 0; }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { ++slow_list_calls; return 0; }
extern "C" unsigned norrath_actor_finish_construction(void*) { stage = 7; return 0xf1f1u; }
static void reset_tags() {
    std::memset(tag_present, 0, sizeof tag_present);
    std::memset(bit_present, 0, sizeof bit_present);
    oneshot = 0; creature_sets_flag = false;
}
static void run(const char* name_text) {
    stage = 0; lump_calls = 0; header_next = 0; set_state_calls = 0; run_list_calls = slow_list_calls = anim_add_calls = randomize_calls = path_calls = specific_calls = sscanf_calls = 0;
    std::memset(object, 0x5a, sizeof object);
    unsigned char point[12] = {0};
    char* args[] = {nullptr};
    static char name[32];
    std::strcpy(name, name_text);
    expect(norrath_construct_Critter(object, name, point, static_cast<short>(-3), args) == 0xf1f1u && stage == 7, "finish result");
    expect(seen_map == nullptr && seen_type == -3 && seen_args == args, "creature arguments");
}
int main() {
    for (auto& h : header_pool) norrath_ctors_ant_lantern::host_note(h);
    for (int c = 'A'; c <= 'Z'; ++c) norrath_factory_ctype_minimum_view[128 + c] = 1;
    const float lead = 2.5f;
    std::memcpy(norrath_ctors_ant_lantern_view_480d28, &lead, 4);
    rolls[0] = 30;                       // 30 % 14 + 4 = 6
    scanned_float = 4.25f;
    // A: no tags, a mixed-case name
    reset_tags();
    run("Ab_C");
    expect(seen_scale == 6.0f, "scale");
    expect(std::strcmp(specific_first, "ab_c") == 0 && std::strcmp(specific_second, "ab_c.lmp") == 0, "lowercased name and lump name");
    expect(lump_calls == 8, "eight lookups");
    const char* want_res[8] = {"ab_c.vif", "ab_c.tex", "ab_c_i", "ab_c_m", "ab_c_f", "ab_c_l", "ab_c_o", "ab_c_p"};
    for (int i = 0; i < 8; ++i) expect(std::strcmp(lump_names[i], "ab_c.lmp") == 0 && std::strcmp(resource_names[i], want_res[i]) == 0, "lookup texts");
    expect(word_at(object + 1036) == bits(header_pool[0]) && word_at(object + 1040) == bits(header_pool[1]) && word_at(object + 1044) == bits(header_pool[2]) &&
           word_at(object + 1056) == bits(header_pool[3]) && word_at(object + 1060) == bits(header_pool[4]) && word_at(object + 1064) == bits(header_pool[5]) &&
           word_at(object + 1068) == bits(header_pool[6]) && word_at(object + 1072) == bits(header_pool[7]), "lookup results stored");
    expect(object[40] == 10 && word_at(object + 1084) == 7u && word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_44c080) && word_at(object + 12) == 8u, "fixed stores");
    expect(word_at(object + 168) == fb(lead) && word_at(object + 28) == fb(10.0f) && word_at(object + 248) == 0x01020304u && object[172] == 18 && word_at(object + 1076) == 2u, "singles");
    expect(word_at(object + 176) == 0 && word_at(object + 192) == 0 && word_at(object + 236) == 0 && half_at(object + 572) == 0 && word_at(object + 1080) == 0, "clears");
    expect(params[0] == 1.0f && params[1] == 1.0f && params[2] == 1.0f && params[3] == 1.0f && params[4] == 1.0f && param_flag == 1, "parameters");
    expect(word_at(object + 1028) == 180 && word_at(object + 1024) == 60 && word_at(object + 1020) == 180 && word_at(object + 1032) == 0 && word_at(object + 1048) == 0 && word_at(object + 1052) == 0, "timing words");
    expect(word_at(object + 1016) == fb(1.0f) && sscanf_calls == 0, "no speed tag");
    expect(path_calls == 1 && path_state == object + 960 && path_point == object + 16 && std::strcmp(path_tag, "pathtext") == 0, "path state");
    expect(set_state_calls == 1 && set_state_values[0] == 0 && run_list_calls == 0 && slow_list_calls == 1, "state 0 and the slow list");
    expect(chunk_arg == header_pool[0] && chunk_class == 0 && word_at(object + 552) == 0x11223344u && word_at(object + 556) == 0xaabbccddu, "chunks use the first lookup");
    expect(specific_calls == 1 && anim_add_calls == 0, "specific init");
    // B: tags replace the names; failing lookups use fallbacks
    reset_tags();
    tag_present[0] = true; tag_text[0] = "lumpx";        // lump tag
    tag_present[7] = true; tag_text[7] = "Fx";            // 1068 lookup fails -> falls back to 1044
    tag_present[8] = true; tag_text[8] = "Fy";            // 1072 lookup fails -> falls back to 1064
    tag_present[1] = true; tag_text[1] = "wak";
    run("Name");
    expect(std::strcmp(lump_names[0], "lumpx.lmp") == 0 && std::strcmp(resource_names[0], "wak.vif") == 0, "tag text replaces the name");
    expect(std::strcmp(specific_first, "name") == 0 && std::strcmp(specific_second, "lumpx.lmp") == 0, "name copy and lump name passed on");
    expect(std::strcmp(resource_names[6], "Fx.anm") == 0 && std::strcmp(resource_names[7], "Fy.anm") == 0, "tag-form format for later lookups");
    expect(word_at(object + 1068) == word_at(object + 1044) && word_at(object + 1072) == word_at(object + 1064), "fallback words");
    // C: failing first lookups choose the state
    reset_tags();
    tag_present[3] = true; tag_text[3] = "Fz";            // 1044 fails
    run("name");
    expect(word_at(object + 1044) == 0 && set_state_calls == 1 && set_state_values[0] == 1, "state 1 when only 1056 is set");
    tag_present[4] = true; tag_text[4] = "Fw";            // 1056 fails as well
    run("name");
    expect(word_at(object + 1056) == 0 && set_state_calls == 0, "no state when both are missing");
    // D: the one-shot tag
    reset_tags();
    oneshot = 1;
    run("name");
    expect(set_state_calls == 1 && set_state_values[0] == 13 && slow_list_calls == 1, "state 13");
    // E: the +980 word
    reset_tags();
    creature_sets_flag = true;
    run("name");
    expect(word_at(object + 12) == (8u | 5u) && anim_add_calls == 1 && anim_flag == 2049 && anim_header == header_pool[3] && randomize_calls == 1, "980 path animation");
    expect(set_state_calls == 1 && set_state_values[0] == 12 && run_list_calls == 1 && slow_list_calls == 0, "state 12 and the run list");
    // F: bits and the speed tag
    reset_tags();
    bit_present[3] = true; bit_present[31] = true;
    tag_present[9] = true; tag_text[9] = "2.5";
    run("name");
    expect(word_at(object + 244) == ((1u << 3) | (1u << 31)) && chunk_class == static_cast<int>((1u << 3) | (1u << 31)), "change bits");
    expect(sscanf_calls == 1 && std::strcmp(sscanf_text, "2.5") == 0 && word_at(object + 1016) == fb(4.25f), "speed tag scanned");
    if (fails != 0) return 1;
    std::puts("critter constructor passed");
    return 0;
}
