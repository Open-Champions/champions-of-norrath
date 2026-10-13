#include <stdint.h>
#include <string.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_sound_sprite_obj.cpp"

static std::string tr; static int id_in, find_ret, find_type, find_handle, create_ret, add_ret, end_rets[3], end_calls, begin_rets[2], begin_calls, sound_ret, float_calls;
static unsigned short second_in; static float float_values[2]; static void* shared_device; static int shared_sound;
alignas(16) static float sprite_mem[32];
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = 0x1234; *second = second_in; tr += "B;"; return begin_rets[begin_calls++]; }
extern "C" int Read__9VIObjFileRi(void*, int* v) { *v = id_in; tr += "I;"; return 0; }
extern "C" int Read__9VIObjFileRf(void*, float* v) { *v = float_values[float_calls++]; tr += "F;"; return 0; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_rets[end_calls++]; }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* d, unsigned id, int* t, int* h) {
    if (d != reinterpret_cast<void*>(0x30) || static_cast<int>(id) != id_in) tr += "BADFIND"; *t = find_type; *h = find_handle; tr += "Find;"; return find_ret; }
extern "C" int CreateSprite__7VIScene12VISpriteTypei(void* s, int t, int a) { if (s != reinterpret_cast<void*>(0x20) || t != 9 || a != 0x99) tr += "BADCREATE"; tr += "C;"; return create_ret; }
extern "C" void* Sprite__7VIScenei(void*, int) { tr += "Sp;"; return sprite_mem; }
extern "C" int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void*, void* r, unsigned id, int t, int h) {
    if (r != sprite_mem || static_cast<int>(id) != id_in || t != 17 || h != create_ret) tr += "BADADD"; tr += "A;"; return add_ret; }
extern "C" int ParseSound__10VIESFParse(void*) { tr += "PS;"; return sound_ret; }
extern "C" void ShareSound__13VISoundDevicei(void* d, int s) { shared_device = d; shared_sound = s; tr += "SH;"; }

static int run(unsigned device) {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[0] = 0x99; w[4] = device; w[6] = 0x20; w[8] = 0x30; w[9] = 0x55; tr.clear(); end_calls = 0; begin_calls = 0; float_calls = 0; shared_device = nullptr;
    for (float& f : sprite_mem) f = 99.0f;
    return ParseSoundSpriteObj__10VIESFParse(parse);
}
int main() {
    id_in = 7; find_ret = 0; find_type = 17; find_handle = 11; create_ret = 6; add_ret = 0; sound_ret = 4; second_in = 3;
    for (int& e : end_rets) e = 0; begin_rets[0] = begin_rets[1] = 0; float_values[0] = 2.0f; float_values[1] = 0.5f;
    if (run(0x77) != 6 || tr != "B;I;F;F;E;Find;C;Sp;A;PS;SH;E;") return 1;
    if (sprite_mem[11] != 2.0f || sprite_mem[2] != -2.0f || sprite_mem[3] != -2.0f || sprite_mem[4] != -2.0f || sprite_mem[5] != 2.0f || sprite_mem[6] != 2.0f || sprite_mem[7] != 2.0f) return 2;
    if (sprite_mem[21] != 0.5f || *reinterpret_cast<int*>(&sprite_mem[16]) != 4 || shared_device != reinterpret_cast<void*>(0x77) || shared_sound != 4) return 3;
    // no sound device: the next sub-object header is skipped instead
    if (run(0) != 6 || tr != "B;I;F;F;E;Find;C;Sp;A;B;E;E;") return 4;
    // second output zero: both floats are 1.0 and are not read
    second_in = 0; if (run(0x77) != 6 || tr != "B;I;E;Find;C;Sp;A;PS;SH;E;" || sprite_mem[11] != 1.0f || sprite_mem[21] != 1.0f) return 5;
    second_in = 3;
    // negative sound result is stored and fails
    sound_ret = -2; if (run(0x77) != -1 || *reinterpret_cast<int*>(&sprite_mem[16]) != -2 || tr.find("SH") != std::string::npos) return 6;
    sound_ret = 4;
    // dictionary hits
    find_ret = 1; if (run(0x77) != 11 || tr != "B;I;F;F;E;Find;E;") return 7;
    find_type = 3; if (run(0x77) != -1 || tr != "B;I;F;F;E;Find;") return 8;
    find_type = 17; find_ret = 0;
    end_rets[0] = -1; if (run(0x77) != -1 || tr != "B;I;F;F;E;") return 9;
    end_rets[0] = 0; add_ret = -1; if (run(0x77) != -1 || tr.find("PS") != std::string::npos) return 10;
    add_ret = 0; create_ret = -3; if (run(0x77) != -1 || tr.find("Sp;") != std::string::npos) return 11;
    create_ret = 6; end_rets[1] = -1; if (run(0x77) != -1) return 12;
    end_rets[1] = 0; begin_rets[0] = -1; if (run(0x77) != -1 || tr != "B;") return 13;
    begin_rets[0] = 0; begin_rets[1] = -1; if (run(0) != -1) return 14;
    begin_rets[1] = 0; end_rets[1] = -1; if (run(0) != -1) return 15;
    end_rets[1] = 0; id_in = 0; if (run(0x77) != 6 || tr.find("Find") != std::string::npos || tr.find("A;") != std::string::npos) return 16;
    return 0;
}
