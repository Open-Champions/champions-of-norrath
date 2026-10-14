#include <stdint.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_point_sprite.cpp"

static std::string tr; static unsigned id_in; static int kind_in, find_ret, find_type, find_handle, create_ret, add_ret, end_ret;
static unsigned long added_id; static int added_type, added_handle, init_kind; static void* init_sprite;
static void* const kSprite = reinterpret_cast<void*>(0x4000);
extern "C" int Read__9VIObjFileRUi(void* f, unsigned* v) { (void)f; *v = id_in; tr += "RU;"; return 0; }
extern "C" int Read__9VIObjFileRi(void* f, int* v) { (void)f; *v = kind_in; tr += "Ri;"; return 0; }
extern "C" int ReadEnd__9VIObjFile(void* f) { (void)f; tr += "E;"; return end_ret; }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* d, unsigned id, int* t, int* h) {
    if (d != reinterpret_cast<void*>(0x30) || id != id_in) tr += "BADFIND"; *t = find_type; *h = find_handle; tr += "F;"; return find_ret; }
extern "C" int CreateSprite__7VIScene12VISpriteTypei(void* s, int t, int a) { if (s != reinterpret_cast<void*>(0x20) || t != 2 || a != 0x99) tr += "BADCREATE"; tr += "C;"; return create_ret; }
extern "C" void* Sprite__7VIScenei(void* s, int h) { if (s != reinterpret_cast<void*>(0x20) || h != create_ret) tr += "BADSPRITE"; tr += "S;"; return kSprite; }
extern "C" int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void* d, void* r, unsigned id, int t, int h) {
    if (d != reinterpret_cast<void*>(0x30) || r != kSprite) tr += "BADADD"; added_id = id; added_type = t; added_handle = h; tr += "A;"; return add_ret; }
extern "C" void Init__13VIPointSprite11VIPointType(void* s, int k) { init_sprite = s; init_kind = k; tr += "I;"; }

static int run() {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[0] = 0x99; w[6] = 0x20; w[8] = 0x30; w[9] = 0x55; tr.clear();
    return ParsePointSpriteObj__10VIESFParse(parse);
}
int main() {
    id_in = 7; kind_in = 4; find_ret = 1; find_type = 23; find_handle = 11; create_ret = 6; add_ret = 0; end_ret = 0;
    // known id of the right type: the stored handle comes back, ReadEnd runs, nothing is created
    if (run() != 11 || tr != "RU;Ri;F;E;") return 1;
    end_ret = -1; if (run() != 11) return 2;
    find_type = 5; if (run() != -1 || tr != "RU;Ri;F;") return 3;
    // unknown id: create, register, initialise with the kind, close
    find_ret = 0; end_ret = 0; if (run() != 6 || tr != "RU;Ri;F;C;S;A;I;E;" || added_id != 7 || added_type != 23 || added_handle != 6 || init_kind != 4 || init_sprite != kSprite) return 4;
    end_ret = -2; if (run() != -1) return 5;
    end_ret = 0; add_ret = -1; if (run() != -1 || tr != "RU;Ri;F;C;S;A;") return 6;
    add_ret = 0; create_ret = -3; if (run() != -1 || tr != "RU;Ri;F;C;") return 7;
    // id 0: no lookup, no registration
    create_ret = 2; id_in = 0; if (run() != 2 || tr != "RU;Ri;C;S;I;E;") return 8;
    return 0;
}
