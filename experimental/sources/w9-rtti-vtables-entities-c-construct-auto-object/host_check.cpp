// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "construct.cpp"
#include <cmath>
#include <cstdio>
#include <cstddef>
#include <cstring>
#include <map>
#include <string>
// Upper-case bit (1) set for the letters A to Z, as in the original table.
extern "C" const unsigned char _ctype_[257] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
extern "C" char cutMode = 0;
extern "C" unsigned char norrath_rtti_entities_a_view_44ed78[8];
unsigned char norrath_rtti_entities_a_view_44ed78[8];
static std::string log_text;
static std::map<std::string, std::pair<const char*, int>> tags;
static int box_values[6];
static char lump_calls[4][64];
static int lump_n = 0;
static char scripted[32];
static unsigned char heap[8];
static void* constructed = nullptr;
static unsigned char point_seen[12];
static char** args_seen = nullptr;
static void* resource_map_seen = reinterpret_cast<void*>(1);
static int box_selector = 0;
static unsigned char the_object[1400];
static unsigned model_word = 0;
static void* vif_result = nullptr;
extern "C" unsigned norrath_game_object_construct(void* object, const void* point, void* resource_map, char** arguments) {
    log_text += "C"; constructed = object; std::memcpy(point_seen, point, 12); resource_map_seen = resource_map; args_seen = arguments;
    rtti_c::write_word(rtti_c::address(object) + 144u, model_word);
    return 0;
}
extern "C" char* findTagString__10GameObjectPci(void*, char* tag, int) {
    auto it = tags.find(tag);
    return it != tags.end() && it->second.first != nullptr ? const_cast<char*>(it->second.first) : nullptr;
}
extern "C" int findTagInt__10GameObjectPci(void*, char* tag, int) {
    auto it = tags.find(tag);
    return it != tags.end() ? it->second.second : 0;
}
extern "C" char* strcpy(char* d, const char* s) { char* o = d; while ((*d++ = *s++) != 0) {} return o; }
extern "C" char* strcat(char* d, const char* s) { char* o = d; while (*d != 0) ++d; while ((*d++ = *s++) != 0) {} return o; }
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) {
    std::snprintf(lump_calls[lump_n++ & 3], 64, "%s|%s", lump, name);
    const std::size_t length = std::strlen(name);
    if (length >= 4 && std::strcmp(name + length - 4, ".vif") == 0) return vif_result;
    return heap + 1;
}
extern "C" void modelGetBoundingBox__FP12_modelHeaderlPiN52(void*, long selector, int* a, int* b, int* c, int* d, int* e, int* f) {
    log_text += "B"; box_selector = static_cast<int>(selector);
    *a = box_values[0]; *b = box_values[1]; *c = box_values[2]; *d = box_values[3]; *e = box_values[4]; *f = box_values[5];
}
extern "C" void animInitState__FP14AnimationState(void*) { log_text += "A"; }
extern "C" void scriptStart__FPc(char* name) { log_text += "S"; std::strncpy(scripted, name, 31); }
static rtti_c::Address a() { return rtti_c::address(the_object); }
static unsigned w(unsigned o) { return rtti_c::read_word(a() + o); }
static float f(unsigned o) { return rtti_c::read_single(a() + o); }
static int bad = 0;
static void expect(bool ok, const char* msg, int x = 0) { if (!ok) { std::printf("fail: %s (%d)\n", msg, x); ++bad; } }
static unsigned run(const char* name, short type = 3) {
    std::memset(the_object, 0xee, sizeof the_object);
    log_text.clear(); lump_n = 0; scripted[0] = 0;
    for (auto& l : lump_calls) l[0] = 0;
    unsigned char point[12];
    for (int i = 0; i < 12; ++i) point[i] = static_cast<unsigned char>(0x30 + i);
    char* arguments[1] = {nullptr};
    char buffer[40];
    for (unsigned i = 0;; ++i) { buffer[i] = name[i]; if (name[i] == 0) break; }
    rtti_c::host_note(heap); rtti_c::host_note(heap + 1);
    const unsigned r = norrath_construct_AutoObject(the_object, buffer, point, type, arguments);
    expect(constructed == the_object && std::memcmp(point_seen, point, 12) == 0 && resource_map_seen == nullptr && args_seen == arguments, "base constructor arguments");
    return r;
}
int main() {
    tags.clear(); model_word = 0; cutMode = 1;
    unsigned r = run("Door_1-xyz", -2);
    expect(r == 0u, "plain result");
    expect(w(72) == rtti_c::address_bits(norrath_rtti_entities_a_view_44ed78) && the_object[157] == 1 && rtti_c::read_u16(a() + 56u) == 0xfffeu, "table, flag byte, type");
    expect(std::strcmp(reinterpret_cast<char*>(the_object + 76), "door_1") == 0, "name lowered and cut at '-'");
    expect(std::strcmp(reinterpret_cast<char*>(the_object + 109), "door_1.lmp") == 0, "lump name");
    expect(std::strcmp(lump_calls[0], "door_1.lmp|door_1.vif") == 0 && std::strcmp(lump_calls[1], "door_1.lmp|door_1.tex") == 0, "resource names");
    expect(w(144) == 0u && w(148) == rtti_c::address_bits(heap + 1), "resource words (the .vif lookup goes to +144, the .tex lookup to +148)");
    expect(f(28) == 12.0f && f(32) == 60.0f && the_object[40] == 12 && the_object[41] == 60, "defaults");
    expect(the_object[108] == 0 && log_text == "CA", "no multiwak, no script while cutMode is nonzero");
    run(";x"); expect(the_object[76] == 0, "delimiter first gives an empty name");
    run(""); expect(the_object[76] == 0, "empty name");
    run("a,b"); expect(std::strcmp(reinterpret_cast<char*>(the_object + 76), "a") == 0, "comma ends the name");
    run("a;b"); expect(std::strcmp(reinterpret_cast<char*>(the_object + 76), "a") == 0, "semicolon ends the name");
    tags["LumpName"] = {"Custom", 0};
    run("abc"); expect(std::strcmp(reinterpret_cast<char*>(the_object + 109), "Custom.lmp") == 0 && std::strcmp(lump_calls[0], "Custom.lmp|abc.vif") == 0, "LumpName tag keeps its case");
    tags.clear();
    tags["radius"] = {"x", 0}; run("abc"); expect(f(28) == 0.0f && (w(12) & 1u) == 1u && the_object[40] == 0, "radius tag of zero sets bit 1");
    tags["radius"] = {"x", 40}; run("abc"); expect(f(28) == 40.0f && (w(12) & 1u) == 0u && the_object[40] == 40, "radius tag value");
    tags.clear(); tags["radius2"] = {"x", 0}; run("abc"); expect(f(28) == 0.0f && (w(12) & 1u) == 1u, "radius2 zero");
    tags["radius2"] = {"x", 50}; run("abc"); expect(f(28) == 50.0f, "radius2 value");
    tags.clear(); tags["height"] = {nullptr, 33}; run("abc"); expect(f(32) == 33.0f && the_object[41] == 33, "height tag");
    tags["height"] = {nullptr, 0}; run("abc"); expect(f(32) == 60.0f, "height zero is ignored");
    tags.clear(); tags["drawradius"] = {"x", 300}; tags["drawheight"] = {"x", -7}; run("abc");
    expect(the_object[40] == 255 && the_object[41] == 0xf9, "draw tags clamp at 255 with a signed compare");
    tags.clear(); tags["drawradius"] = {"x", 77}; run("abc"); expect(the_object[40] == 77, "drawradius value");
    tags.clear(); tags["multiwak"] = {"1", 0}; run("abc"); expect(the_object[108] == 1, "multiwak");
    tags.clear(); tags["initScript"] = {"scriptname", 0}; cutMode = 1; run("abc"); expect(log_text == "CA" && scripted[0] == 0, "script skipped while cutMode is set");
    cutMode = 0; run("abc"); expect(log_text == "CAS" && std::strcmp(scripted, "scriptname") == 0, "script started");
    tags.clear(); run("abc"); expect(log_text == "CA", "no script tag");
    tags["fullbright"] = {nullptr, 5}; rtti_c::write_word(rtti_c::address(the_object), 0u);
    std::memset(the_object, 0xee, sizeof the_object); r = run("abc"); expect(w(208) == (0xeeeeeeeeu | 2u) && r == w(208), "fullbright ORs 2 into +208 and returns the word");
    tags["fullbright"] = {nullptr, 0}; r = run("abc"); expect(w(208) == 0xeeeeeeeeu && r == 0u, "fullbright zero");
    // bounding box
    tags.clear(); vif_result = heap + 4; rtti_c::host_note(heap + 4);
    box_values[0] = 0; box_values[1] = 0; box_values[2] = 10; box_values[3] = 60; box_values[4] = 80; box_values[5] = 50;
    run("abc");
    expect(log_text == "CBA" && box_selector == -1, "box lookup");
    expect(the_object[40] == 50 && the_object[41] == 20, "box sizes: sqrt(60^2+80^2)/2 = 50 and |10-50|/2 = 20");
    tags["radius"] = {"x", 70}; run("abc");
    expect(the_object[40] == 70 && the_object[41] == 70, "limit larger than the box sizes replaces both");
    box_values[2] = 10; box_values[5] = 10; tags["radius"] = {"x", 0}; run("abc");
    expect(the_object[40] == 50 && the_object[41] == 0, "equal box heights give a zero byte");
    tags.clear(); tags["drawradius"] = {"x", 9}; run("abc"); expect(the_object[40] == 9, "draw tags override the box result");
    std::printf(bad ? "failed\n" : "norrath_construct_AutoObject passed\n");
    return bad != 0;
}
