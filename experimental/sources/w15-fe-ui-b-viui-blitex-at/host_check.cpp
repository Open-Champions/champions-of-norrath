#include "blitex_at.cpp"
#include <cstring>
#include <string>

static std::string trace;
static int record[9] = {7, 1, 207, 203, 114, 44, 0, 0x3f800000, 0};
static const int* record_ptr = record;
static int find_result = 1;
static int last_type_seen;
struct Blitted { unsigned raster; int index; unsigned flags; int at[2], a[2], b[2], c[2]; int blend; float alpha; int calls; };
static Blitted blitted;

extern "C" const int* GetUITexture__F9VIUITexId(int texture) { trace += "T" + std::to_string(texture) + ";"; return record_ptr; }
extern "C" const unsigned short* GetString__13VIStringTableii(VIStringTable* table, int id, int extra) {
    static const unsigned short text[3] = {'a', 'b', 0};
    trace += "G" + std::to_string(id) + "," + std::to_string(extra) + ";";
    (void)table;
    return text;
}
extern "C" void str16to8__8VIStringPcUiPCUs(char* destination, unsigned size, const unsigned short* source) {
    trace += "S;";
    for (unsigned i = 0; i < size; ++i) { destination[i] = static_cast<char>(source[i]); if (!source[i]) break; }
}
extern "C" unsigned VIHashResourceID_Icon__FPCc(const char* name) { trace += "H" + std::string(name) + ";"; return 0xabcd; }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(unsigned dictionary, unsigned hash, int& type, int& index) {
    trace += "F" + std::to_string(dictionary) + "," + std::to_string(hash) + "," + std::to_string(index) + ";";
    (void)type;
    if (find_result) index = 55;
    return find_result;
}
extern "C" void Blit__8VIRasteriUiRC8VIVect2IN2316VIAlphaBlendModef(unsigned raster, int index, unsigned flags,
                                                                     const VIVect2I& at, const VIVect2I& source,
                                                                     const VIVect2I& size, int blend, float alpha) {
    blitted = {raster, index, flags, {at.x, at.y}, {source.x, source.y}, {size.x, size.y}, {0, 0}, blend, alpha, blitted.calls + 1};
}
static VIVect2I make(int x, int y) { VIVect2I v; v.x = x; v.y = y; return v; }

int main() {
    alignas(16) unsigned char ui[128] = {};
    const unsigned raster = 0x1234, dictionary = 0x5678;
    const int extra = 9;
    std::memcpy(ui + 72, &raster, 4);
    std::memcpy(ui + 80, &dictionary, 4);
    std::memcpy(ui + 48, &extra, 4);
    char* u = reinterpret_cast<char*>(ui);
    if (BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(u, 7, make(10, 20), 3, 0.5f, 1u) != 0) return 1;
    if (trace != "T7;G1,9;S;Hab;F22136,43981,-1;") return 2;
    if (blitted.calls != 1 || blitted.raster != raster || blitted.index != 55 || blitted.flags != 1u) return 3;
    if (blitted.at[0] != 10 || blitted.at[1] != 20 || blitted.a[0] != 207 || blitted.a[1] != 203) return 4;
    if (blitted.b[0] != 114 || blitted.b[1] != 44 || blitted.blend != 3 || blitted.alpha != 0.5f) return 5;
    find_result = 0;
    blitted.calls = 0;
    if (BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(u, 7, make(1, 2), 3, 0.5f, 1u) != -1) return 6;
    if (blitted.calls != 0) return 7;
    record_ptr = nullptr;
    trace.clear();
    if (BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(u, 7, make(1, 2), 3, 0.5f, 1u) != -1) return 8;
    if (trace != "T7;") return 9;
    return 0;
}
