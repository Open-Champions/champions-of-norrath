#include "npc_named_ctor.cpp"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

unsigned char vt_3NPC[16] __asm__("_vt$3NPC");
unsigned char playedStinger = 0xaa;

static std::vector<std::string> order;
static unsigned char* current;
static unsigned point_seen[3];
static short type_seen;
static void* map_seen = reinterpret_cast<void*>(1);
static float scale_seen;
static char** arguments_seen;
static std::vector<std::string> lookups;
static bool have_animation = true;
static bool have_tag = true;
static unsigned char stinger_at_finish;
static unsigned flags_at_slow_list;
static unsigned long long chunks_word_at_slow_list;
static unsigned words_at_init[2];
static void* animation_header_added;
static int animation_flag_added;
static void* chunk_model_seen;
static int chunk_request_seen;

static unsigned word(const unsigned char* base, unsigned offset) { unsigned v; std::memcpy(&v, base + offset, 4); return v; }
static unsigned short half(const unsigned char* base, unsigned offset) { unsigned short v; std::memcpy(&v, base + offset, 2); return v; }
static float flt(const unsigned char* base, unsigned offset) { float v; std::memcpy(&v, base + offset, 4); return v; }

extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type, void* resource_map, float scale,
                                                char** arguments) {
    order.push_back("creature");
    std::memcpy(point_seen, point, 12);
    type_seen = type;
    map_seen = resource_map;
    scale_seen = scale;
    arguments_seen = arguments;
    unsigned flags = 0x00000040u;
    std::memcpy(static_cast<unsigned char*>(object) + 12, &flags, 4);
    return 0;
}
extern "C" void* norrath_lump_find_resource(char* lump_name, char* resource_name) {
    order.push_back("lump");
    lookups.push_back(std::string(lump_name) + "|" + resource_name);
    if (std::strcmp(resource_name, "anm.name") == 0) return have_animation ? reinterpret_cast<void*>(0x7100) : nullptr;
    if (std::strcmp(resource_name, "model.name") == 0) return reinterpret_cast<void*>(0x7200);
    return reinterpret_cast<void*>(0x7300);
}
extern "C" void norrath_actor_animation_initialize(void* state) {
    order.push_back("anim_init");
    std::memcpy(words_at_init, static_cast<unsigned char*>(state) + (1136 - 288), 4);
    if (state != current + 288) std::fprintf(stderr, "fail: animation state address\n");
}
extern "C" void norrath_actor_animation_add(void* state, void* header, int flags) {
    order.push_back("anim_add");
    animation_header_added = header;
    animation_flag_added = flags;
}
extern "C" void norrath_actor_animation_randomize_time(void* state) { order.push_back("randomize"); }
extern "C" unsigned long long norrath_actor_model_active_chunks(void* model, int requested) {
    order.push_back("chunks");
    chunk_model_seen = model;
    chunk_request_seen = requested;
    return 0x1122334455667788ull;
}
extern "C" unsigned norrath_actor_add_slow_run_list(void* object) {
    order.push_back("slowlist");
    chunks_word_at_slow_list = (static_cast<unsigned long long>(word(current, 556)) << 32) | word(current, 552);
    return 0;
}
extern "C" char* norrath_actor_find_tag_string(void* object, char* tag, int occurrence) {
    order.push_back(std::string("tag:") + tag);
    flags_at_slow_list = word(current, 12);
    return have_tag ? tag : nullptr;
}
extern "C" unsigned norrath_creature_finish_construction(void* object) {
    order.push_back("finish");
    stinger_at_finish = playedStinger;
    return 0x600d;
}

int main() {
    int bad = 0;
    auto expect = [&](bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); bad = 1; } };
    alignas(16) unsigned char object[1152];
    current = object;
    std::memset(object, 0xcc, sizeof object);
    const unsigned point[3] = {0x11, 0x22, 0x33};
    char arguments_storage[8];
    char* arguments[2] = {arguments_storage, nullptr};
    char lump[] = "lump.lmp", model[] = "model.name", texture[] = "tex.name", animation[] = "anm.name";

    const unsigned result = __3NPCG6Point3sPcN33ifPPc(object, point, -5, lump, model, texture, animation, 0x12345, 60.0f, arguments);
    expect(result == 0x600d, "finishConstruction result returned");
    expect(std::memcmp(point_seen, point, 12) == 0 && type_seen == -5 && map_seen == nullptr && scale_seen == 8.0f &&
               arguments_seen == arguments, "creature constructor arguments");
    expect(order.front() == "creature" && order.back() == "finish", "first and last call");
    const char* want[] = {"creature", "lump", "lump", "anim_init", "lump", "lump", "anim_add", "randomize", "chunks",
                          "slowlist", "tag:showName", "finish"};
    expect(order.size() == 12, "twelve calls with an animation");
    for (size_t i = 0; i < order.size() && i < 12; ++i) expect(order[i] == want[i], "call order");
    expect(word(object, 72) == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_3NPC)), "vtable");
    expect(std::strcmp(reinterpret_cast<char*>(object + 960), "lump.lmp") == 0 &&
               std::strcmp(reinterpret_cast<char*>(object + 992), "model.name") == 0 &&
               std::strcmp(reinterpret_cast<char*>(object + 1024), "tex.name") == 0, "three names copied");
    expect(half(object, 1066) == 0x2345, "integer low 16 bits at +1066");
    expect(word(object, 12) == (0x40u | 0x00100008u | 0x00010000u), "flags");
    for (int i = 0; i < 5; ++i) expect(flt(object, 88 + 4 * i) == 1.0f, "five floats set to 1.0");
    expect(flt(object, 84) != 1.0f && flt(object, 108) != 1.0f, "float run is exactly 88..107");
    expect(object[40] == 36 && flt(object, 28) == 20.0f && word(object, 236) == 0u, "byte 40, float 28, word 236");
    expect(lookups.size() == 4 && lookups[0] == "lump.lmp|model.name" && lookups[1] == "lump.lmp|tex.name" &&
               lookups[2] == "lump.lmp|anm.name" && lookups[3] == "lump.lmp|anm.name", "lookups");
    expect(word(object, 1056) == 0x7200u && word(object, 1060) == 0x7300u, "resource words");
    expect(half(object, 572) == 0 && flt(object, 168) == 0.5f && flt(object, 176) == 60.0f, "half 572, 0.5 and the float");
    expect(words_at_init[0] == 0u, "word +1136 cleared before the animation state is initialised");
    expect(animation_header_added == reinterpret_cast<void*>(0x7100) && animation_flag_added == 1, "animation added with flag 1");
    expect(chunk_model_seen == reinterpret_cast<void*>(0x7200) && chunk_request_seen == 0, "chunks arguments");
    expect(chunks_word_at_slow_list == 0x1122334455667788ull, "64-bit chunks stored before the run-list call");
    expect(flt(object, 168) == 0.5f && word(object, 12) == flags_at_slow_list, "showName present leaves the flag");
    expect(stinger_at_finish == 0 && playedStinger == 0, "playedStinger cleared before finishConstruction");

    // No animation resource and no tag.
    order.clear();
    lookups.clear();
    have_animation = false;
    have_tag = false;
    playedStinger = 0xaa;
    std::memset(object, 0xcc, sizeof object);
    __3NPCG6Point3sPcN33ifPPc(object, point, 7, lump, model, texture, animation, 0, 1.5f, arguments);
    expect(order.size() == 9 && lookups.size() == 3, "no animation: one lookup, no add");
    expect((word(object, 12) & 0x00100000u) == 0u && (word(object, 12) & 0x00010008u) == 0x00010008u,
           "tag missing clears bit 0x00100000 only");
    expect(flt(object, 176) == 1.5f && half(object, 1066) == 0, "other arguments");
    if (bad) return 1;
    std::puts("named NPC constructor contract passed");
    return 0;
}
