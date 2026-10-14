#include <stdint.h>
#include <string.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_point_light_obj.cpp"

static std::string tr; static unsigned id_in; static int find_ret, find_type, find_handle, create_ret, add_ret, end_ret, float_calls;
static float float_values[5];
alignas(16) static float sprite_mem[32];
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = id_in; tr += "U;"; return 0; }
extern "C" int Read__9VIObjFileRf(void*, float* v) { *v = float_values[float_calls++]; tr += "F;"; return 0; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* d, unsigned id, int* t, int* h) {
    if (d != reinterpret_cast<void*>(0x30) || id != id_in) tr += "BADFIND"; *t = find_type; *h = find_handle; tr += "Find;"; return find_ret; }
extern "C" int CreateSprite__7VIScene12VISpriteTypei(void* s, int t, int a) { if (s != reinterpret_cast<void*>(0x20) || t != 8 || a != 0x99) tr += "BADCREATE"; tr += "C;"; return create_ret; }
extern "C" void* Sprite__7VIScenei(void*, int) { tr += "Sp;"; return sprite_mem; }
extern "C" int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void*, void* r, unsigned id, int t, int h) {
    if (r != sprite_mem || id != id_in || t != 16 || h != create_ret) tr += "BADADD"; tr += "A;"; return add_ret; }

static int run() {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[0] = 0x99; w[6] = 0x20; w[8] = 0x30; w[9] = 0x55; tr.clear(); float_calls = 0;
    for (float& f : sprite_mem) f = 99.0f;
    return ParsePointLightObj__10VIESFParse(parse);
}
int main() {
    id_in = 7; find_ret = 1; find_type = 16; find_handle = 11; create_ret = 6; add_ret = 0; end_ret = 0;
    float_values[0] = 2.0f; float_values[1] = 0.25f; float_values[2] = 0.5f; float_values[3] = 0.75f; float_values[4] = 1.0f;
    if (run() != 11 || tr != "U;F;F;F;F;F;Find;E;") return 1;
    find_type = 3; if (run() != -1 || tr != "U;F;F;F;F;F;Find;") return 2;
    find_ret = 0;
    if (run() != 6 || tr != "U;F;F;F;F;F;Find;C;Sp;A;E;") return 3;
    // radius at +44, centre zeros at +32, box -r/+r around the centre, vector copied to +64
    if (sprite_mem[11] != 2.0f || sprite_mem[8] != 0 || sprite_mem[9] != 0 || sprite_mem[10] != 0) return 4;
    if (sprite_mem[2] != -2.0f || sprite_mem[3] != -2.0f || sprite_mem[4] != -2.0f || sprite_mem[5] != 2.0f || sprite_mem[6] != 2.0f || sprite_mem[7] != 2.0f) return 5;
    if (sprite_mem[16] != 0.25f || sprite_mem[17] != 0.5f || sprite_mem[18] != 0.75f || sprite_mem[19] != 1.0f || sprite_mem[0] != 99.0f || sprite_mem[20] != 99.0f) return 6;
    // zero radius: the x lane of the lower corner is negative zero, the others are positive zero
    float_values[0] = 0.0f; run();
    if (!__builtin_signbit(sprite_mem[2]) || __builtin_signbit(sprite_mem[3]) || __builtin_signbit(sprite_mem[4])) return 7;
    end_ret = -1; if (run() != -1) return 8;
    end_ret = 0; add_ret = -1; if (run() != -1 || tr != "U;F;F;F;F;F;Find;C;Sp;A;" || sprite_mem[11] != 99.0f) return 9;
    add_ret = 0; create_ret = -3; if (run() != -1 || tr.find("Sp;") != std::string::npos) return 10;
    create_ret = 2; id_in = 0; if (run() != 2 || tr != "U;F;F;F;F;F;C;Sp;E;") return 11;
    return 0;
}
