#include <cstdio>
#include <cstring>
extern "C" const unsigned char norrath_archive_animation_extension_view[64] = {
    'a','n','m',0,0,0,0,0,
    'v','i','f',0,0,0,0,0,
    't','e','x',0,0,0,0,0,
    'f','n','t',0,0,0,0,0,
    'w','o','r','l','d',0,0,0,
    'v','a','g',0,0,0,0,0,
    'u','n','i',0,0,0,0,0,
    'e','t','e','x',0,0,0,0
};
extern "C" unsigned norrath_archive_no_new_vags_view = 0;
static int kind;
static void* seen_payload;
static char* seen_name;
static int seen_allow;
static int world_result;
static int compares;
extern "C" char* norrath_archive_last_character(const char* text, int character) {
    const char* found = nullptr;
    for (const char* cursor = text; *cursor != 0; ++cursor)
        if (*cursor == static_cast<char>(character)) found = cursor;
    return const_cast<char*>(found);
}
extern "C" int norrath_string_compare(const char* left, const char* right) {
    ++compares;
    return std::strcmp(left, right);
}
extern "C" void animInitAnimation__FP15AnimationHeader(void* payload) { kind = 1; seen_payload = payload; }
extern "C" void modelInitModel__FPv(void* payload) { kind = 2; seen_payload = payload; }
extern "C" void textureInitTexture__FP8_texture(void* payload) { kind = 3; seen_payload = payload; }
extern "C" void fontInitFont__FP4Font(void* payload) { kind = 4; seen_payload = payload; }
extern "C" int worldInitWorld__FP12_worldHeader(void* payload) { kind = 5; seen_payload = payload; return world_result; }
extern "C" void soundAddSound__FPcP10_vagHeaderb(char* name, void* payload, int allow) {
    kind = 6; seen_name = name; seen_payload = payload; seen_allow = allow;
}
extern "C" void textAddText__FPv(void* payload) { kind = 7; seen_payload = payload; }
extern "C" unsigned norrath_archive_initialize_file(char*, void*, int);
int main() {
    char payload[4] = {};
    compares = 0;
    if (norrath_archive_initialize_file(const_cast<char*>("plain"), payload, 10) != 0u || kind != 0) return 1;
    if (norrath_archive_initialize_file(const_cast<char*>("a.tex.anm"), payload, 10) != 0u || kind != 1) return 2;
    if (seen_payload != payload) return 3;
    if (norrath_archive_initialize_file(const_cast<char*>("m.vif"), payload, 10) != 0u || kind != 2) return 4;
    if (norrath_archive_initialize_file(const_cast<char*>("t.tex"), payload, 10) != 0u || kind != 3) return 5;
    if (norrath_archive_initialize_file(const_cast<char*>("f.fnt"), payload, 10) != 0u || kind != 4) return 6;
    world_result = 0;
    if (norrath_archive_initialize_file(const_cast<char*>("w.world"), payload, 500) != 0u || kind != 5) return 7;
    world_result = 100;
    if (norrath_archive_initialize_file(const_cast<char*>("w.world"), payload, 500) != 400u) return 8;
    world_result = 20;
    if (norrath_archive_initialize_file(const_cast<char*>("w.world"), payload, 10) != 0xfffffff6u) return 9;
    norrath_archive_no_new_vags_view = 1u;
    char vag[] = "sound.vag";
    if (norrath_archive_initialize_file(vag, payload, 100) != 36u || kind != 6) return 10;
    if (seen_name != vag || seen_allow != 1) return 11;
    if (norrath_archive_initialize_file(const_cast<char*>("tiny.vag"), payload, 10) != 0xffffffcau) return 12;
    if (norrath_archive_initialize_file(const_cast<char*>("t.uni"), payload, 10) != 0u || kind != 7) return 13;
    kind = 0;
    compares = 0;
    if (norrath_archive_initialize_file(const_cast<char*>("e.etex"), payload, 10) != 0u || kind != 0 || compares != 8) return 14;
    compares = 0;
    if (norrath_archive_initialize_file(const_cast<char*>("q.zzz"), payload, 10) != 0u || compares != 8) return 15;
    std::puts("initialize file contract passed");
    return 0;
}
