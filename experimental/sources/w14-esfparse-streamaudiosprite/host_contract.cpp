#include <stdint.h>
#include <string.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_stream_audio_sprite_obj.cpp"

static std::string tr; static unsigned id_in; static int find_ret, find_type, find_handle, create_ret, add_ret, end_ret, int_calls;
static unsigned long added_id; static int added_type, added_handle; static int float_calls; static StreamAudioBox box_seen; static int limit_seen;
alignas(8) static unsigned char sprite_mem[128];
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = id_in; tr += "U;"; return 0; }
extern "C" int Read__9VIObjFileRf(void*, float* v) { *v = 1.0f + float_calls++; tr += "F;"; return 0; }
extern "C" int Read__9VIObjFileRi(void*, int* v) { *v = 100 + int_calls++; tr += "I;"; return 0; }
extern "C" int ReadString__9VIObjFilePcUs(void*, char* b, unsigned short n) { limit_seen = n; strcpy(b, "dir/song.xa"); tr += "S;"; return 0; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* d, unsigned id, int* t, int* h) {
    if (d != reinterpret_cast<void*>(0x30) || id != id_in) tr += "BADFIND"; *t = find_type; *h = find_handle; tr += "Find;"; return find_ret; }
extern "C" int CreateSprite__7VIScene12VISpriteTypei(void* s, int t, int a) { if (s != reinterpret_cast<void*>(0x20) || t != 12 || a != 0x99) tr += "BADCREATE"; tr += "C;"; return create_ret; }
extern "C" void* Sprite__7VIScenei(void*, int) { tr += "Sp;"; return sprite_mem; }
extern "C" int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void*, void* r, unsigned id, int t, int h) {
    if (r != sprite_mem) tr += "BADADD"; added_id = id; added_type = t; added_handle = h; tr += "A;"; return add_ret; }
extern "C" void Init__19VIStreamAudioSpriteRC6VIBBox(void* s, const StreamAudioBox* b) { if (s != sprite_mem) tr += "BADINIT"; box_seen = *b; tr += "I0;"; }

static int run() {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[0] = 0x99; w[6] = 0x20; w[8] = 0x30; w[9] = 0x55; tr.clear(); float_calls = 0; int_calls = 0; memset(sprite_mem, 0xee, sizeof sprite_mem);
    return ParseStreamAudioSpriteObj__10VIESFParse(parse);
}
int main() {
    id_in = 7; find_ret = 1; find_type = 24; find_handle = 11; create_ret = 6; add_ret = 0; end_ret = 0;
    if (run() != 11 || tr != "U;F;F;F;F;F;F;Find;E;") return 1;
    end_ret = -1; if (run() != 11) return 2;
    find_type = 23; if (run() != -1 || tr != "U;F;F;F;F;F;F;Find;") return 3;
    find_ret = 0; end_ret = 0;
    if (run() != 6 || tr != "U;F;F;F;F;F;F;Find;C;Sp;A;I0;S;I;I;E;") return 4;
    if (added_id != 7 || added_type != 24 || added_handle != 6) return 5;
    for (int i = 0; i < 6; ++i) if (box_seen.v[i] != 1.0f + i) return 6;
    if (strcmp(reinterpret_cast<char*>(sprite_mem + 72), "dir/song.xa") != 0 || limit_seen != 127) return 7;
    if (*reinterpret_cast<int*>(sprite_mem + 64) != 100 || *reinterpret_cast<int*>(sprite_mem + 68) != 101) return 8;
    end_ret = -2; if (run() != -1) return 9;
    end_ret = 0; add_ret = -1; if (run() != -1 || tr.find("I0;") != std::string::npos) return 10;
    add_ret = 0; create_ret = -3; if (run() != -1 || tr.find("Sp;") != std::string::npos) return 11;
    create_ret = 2; id_in = 0; if (run() != 2 || tr != "U;F;F;F;F;F;F;C;Sp;I0;S;I;I;E;") return 12;
    return 0;
}
