#include <stdint.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_hsprite_attachments.cpp"

static std::string tr; static int begin_ret, end_ret, find_ret, attach_ret, skin_ret, count_in, find_calls, fail_find; static unsigned short tag_in;
static int kinds[4], nodes[4], reads; static unsigned ids[4];
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tag_in; *second = 0; tr += "B;"; return begin_ret; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_ret; }
extern "C" int Read__9VIObjFileRi(void*, int* v) {
    if (reads == 0) *v = count_in; else if ((reads - 1) % 3 == 0) *v = kinds[(reads - 1) / 3]; else *v = nodes[(reads - 1) / 3];
    ++reads; tr += "I;"; return 0; }
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = ids[(reads - 1) / 3]; ++reads; tr += "U;"; return 0; }
extern "C" int FindTyped__12VIDictionaryUi14VIResourceTypeRi(void* d, unsigned id, int type, int* h) {
    if (d != reinterpret_cast<void*>(0x30)) tr += "BADDICT"; *h = 900 + (int)id; tr += "F" + std::to_string(id) + "t" + std::to_string(type) + ";";
    return find_calls++ == fail_find ? 0 : find_ret; }
extern "C" int AttachSkin__9VIHSpriteiR7VIScene(void* s, int h, void* sc) {
    if (s != reinterpret_cast<void*>(0x77) || sc != reinterpret_cast<void*>(0x20)) tr += "BADSKIN"; tr += "S" + std::to_string(h) + ";"; return skin_ret; }
extern "C" int Attach__9VIHSpriteiiR7VIScene(void* s, int h, int n, void* sc) {
    if (s != reinterpret_cast<void*>(0x77) || sc != reinterpret_cast<void*>(0x20)) tr += "BADATT"; tr += "A" + std::to_string(h) + "n" + std::to_string(n) + ";"; return attach_ret; }
static int run() { alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse); w[6] = 0x20; w[8] = 0x30; w[9] = 0x55; tr.clear(); reads = 0; find_calls = 0;
    return ParseHSpriteAttachments__10VIESFParseR9VIHSprite(parse, reinterpret_cast<void*>(0x77)); }
int main() {
    begin_ret = end_ret = attach_ret = skin_ret = 0; find_ret = 1; fail_find = -1; tag_in = 0x2500; count_in = 3;
    kinds[0] = 0; ids[0] = 5; nodes[0] = 2; kinds[1] = 1; ids[1] = 6; nodes[1] = -1; kinds[2] = 1; ids[2] = 0; nodes[2] = 9;
    if (run() != 0 || tr != "B;I;I;U;I;F5t4;A905n2;I;U;I;F6t7;S906;I;U;I;E;") return 1;
    count_in = 0; if (run() != 0 || tr != "B;I;E;") return 2;
    count_in = -1; if (run() != 0 || tr != "B;I;E;") return 3;
    count_in = 3; kinds[2] = 2; ids[2] = 0; if (run() != -1 || tr != "B;I;I;U;I;F5t4;A905n2;I;U;I;F6t7;S906;I;U;I;") return 4;
    kinds[2] = 1; kinds[1] = 3; if (run() != -1 || tr.find("F6") != std::string::npos) return 5;
    kinds[1] = 1; fail_find = 1; if (run() != -1 || tr.find("S906") != std::string::npos) return 6;
    fail_find = -1; skin_ret = -1; if (run() != -1 || tr.find("E;") != std::string::npos) return 7;
    skin_ret = 0; attach_ret = -2; if (run() != -1 || tr.find("I;U;I;F6") != std::string::npos) return 8;
    attach_ret = 0; end_ret = -1; if (run() != -1) return 9;
    end_ret = 0; tag_in = 0x2501; if (run() != -1 || tr != "B;") return 10;
    tag_in = 0x2500; begin_ret = -1; if (run() != -1 || tr != "B;") return 11;
    return 0;
}
