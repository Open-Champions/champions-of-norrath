// Host contract for msg_run__9Generator. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#include "msg_run.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

void* world;
int frameNm;
static std::string order;
static int every_result = 0, activation_result = 1;
static float hit_result;
static void* hit_object;
static float hs_start[3], hs_dir[3], hs_scale;
static void *hs_world, *hs_self;
static int hs_args[2];
static float box_low[3], box_high[3];
static int box_capacity;
static std::vector<unsigned> box_found;
static std::vector<void*> state_calls;
static int state_values_ok = 1;
static void* destroyed;
static void *link_head, *link_node, *link_item;
static void* created_object;
static char* create_name;
static float create_point[3];
static short create_angle;
static char** create_args;
static unsigned long long created_u64_at_link;
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
extern "C" int everyFrame__8CreatureP12_modelHeader(void*, void* m) { order += 'e'; expect(m == nullptr, "null model"); return every_result; }
extern "C" int objectActivationCheck__8Creaturei(void*, int d) { order += 'o'; expect(d == 600, "distance"); return activation_result; }
extern "C" float icos__Fi(int a) { order += 'c'; return 0.5f + float(a % 7) * 0.01f; }
extern "C" float isin__Fi(int a) { order += 's'; return -0.25f + float(a % 5) * 0.01f; }
extern "C" float worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(
    void* w, const void* start, const void* dir, float scale, void* self, void** hit, int a, int b) {
    order += 'H'; hs_world = w; std::memcpy(hs_start, start, 12); std::memcpy(hs_dir, dir, 12); hs_scale = scale; hs_self = self;
    *hit = hit_object; hs_args[0] = a; hs_args[1] = b; return hit_result;
}
extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(const void* lo, const void* hi, void** found, int cap) {
    order += 'B'; std::memcpy(box_low, lo, 12); std::memcpy(box_high, hi, 12); box_capacity = cap;
    unsigned* words = reinterpret_cast<unsigned*>(found);
    unsigned i = 0;
    for (unsigned w : box_found) words[i++] = w;
    words[i] = 0;
}
extern "C" void listDestroy__FP8ListHead(void* h) { order += 'D'; destroyed = h; }
extern "C" void listOnewayLink__FP8ListHeadT0Pv(void* h, void* n, void* i) {
    order += 'L'; link_head = h; link_node = n; link_item = i;
    created_u64_at_link = vslots_b::read_u64(vslots_b::address(n) + 160u);
}
extern "C" void* objectCreateByName__FPcG6Point3sPPc(char* name, const void* p, short angle, char** args) {
    order += 'C'; create_name = name; std::memcpy(create_point, p, 12); create_angle = angle; create_args = args; return created_object;
}
static void state_function(void* adjusted, int state) { order += 'S'; state_calls.push_back(adjusted); if (state != 3) state_values_ok = 0; }

static unsigned char object[1300], table[256], hit_obj[300], other[3][300], created[300], node[64], first_obj[300], name_text[16];
static void setw(unsigned char* base, unsigned off, unsigned v) { vslots_b::write_word(vslots_b::address(base) + off, v); }
static unsigned getw(unsigned char* base, unsigned off) { return vslots_b::read_word(vslots_b::address(base) + off); }
static void setup(int state, unsigned flags) {
    std::memset(object, 0x41, sizeof object);
    vslots_b::write_u16(vslots_b::address(object) + 572u, static_cast<unsigned>(state) & 0xffffu);
    setw(object, 980, flags);
    setw(object, 72, vslots_b::address_bits(table));
    vslots_b::write_u16(vslots_b::address(table) + 184u, 8u);
    setw(table, 188, vslots_b::address_bits(reinterpret_cast<void*>(state_function)));
    const float pos[3] = {10.0f, 20.0f, 30.0f}; std::memcpy(object + 16, pos, 12);
    vslots_b::write_u16(vslots_b::address(object) + 56u, 0x0123u);
    order.clear(); state_calls.clear(); box_found.clear();
    every_result = 0; activation_result = 1; hit_result = 0.5f; hit_object = hit_obj; setw(hit_obj, 12, 0x40);
    object[569] = 0x05; frameNm = 5;
}
int main() {
    using namespace vslots_b;
    host_note(table); host_note(reinterpret_cast<void*>(state_function)); host_note(object); host_note(hit_obj); host_note(created);
    host_note(node); host_note(first_obj); host_note(name_text);
    for (auto& o : other) host_note(o);
    world = reinterpret_cast<void*>(0x1234);
    // everyFrame non-zero, activation zero, other states
    setup(0, 2); every_result = 1; msg_run__9Generator(object); expect(order == "e", "everyFrame stops");
    setup(0, 2); activation_result = 0; msg_run__9Generator(object); expect(order == "eo", "activation stops");
    for (int st : {1, 2, 4, -1, 13}) { setup(st, 2); msg_run__9Generator(object); expect(order == "eo", "other states return"); }
    // state 0, bit 1 set: hit in range with bit 6 -> setState(3), return
    setup(0, 2); msg_run__9Generator(object);
    expect(order == "eocsHS" && state_calls.size() == 1 && state_calls[0] == object + 8 && state_values_ok, "hit sets state 3");
    expect(hs_world == world && hs_self == object && hs_args[0] == 0 && hs_args[1] == 0 && hs_scale == __builtin_bit_cast(float, 0x3f8cccccu), "hitscan arguments");
    {
        const float c = 0.5f + float(0x123 % 7) * 0.01f, s = -0.25f + float(0x123 % 5) * 0.01f;
        expect(hs_start[0] == 10.0f && hs_start[1] == 20.0f && hs_start[2] == 40.0f, "hitscan start");
        expect(hs_dir[0] == c * 500.0f && hs_dir[1] == s * 500.0f && hs_dir[2] == 0.0f, "hitscan direction");
    }
    // result out of range or hit flag clear or null: no state, frame check passes (frameNm ^ byte) & 7 == 0 -> box search
    for (int variant = 0; variant < 5; ++variant) {
        setup(0, 2);
        box_found = {vslots_b::address_bits(other[0]), vslots_b::address_bits(other[1]), vslots_b::address_bits(other[2])};
        setw(other[0], 12, 0x40); setw(other[1], 12, 0x48); setw(other[2], 12, 0x41);
        if (variant == 0) hit_result = 0.0f;
        if (variant == 1) hit_result = 1.0f;
        if (variant == 2) hit_object = nullptr;
        if (variant == 3) setw(hit_obj, 12, 0x3f);
        if (variant == 4) hit_result = -2.0f;
        msg_run__9Generator(object);
        expect(order == "eocsHcsBSS", "box search and two triggers");
        expect(state_calls.size() == 2 && box_capacity == 128, "two triggers, capacity");
        const float c = 0.5f + float(0x123 % 7) * 0.01f, s = -0.25f + float(0x123 % 5) * 0.01f;
        const float cx = 10.0f + c * 200.0f, cy = 20.0f + s * 200.0f, cz = 30.0f + 0.0f * 200.0f;
        expect(box_low[0] == cx - 200.0f && box_low[1] == cy - 200.0f && box_low[2] == cz - 200.0f, "box low");
        expect(box_high[0] == cx + 200.0f && box_high[1] == cy + 200.0f && box_high[2] == cz + 200.0f, "box high");
    }
    // frame gate: (frameNm ^ signed byte) & 7 != 0 returns; the byte is sign extended
    setup(0, 2); hit_result = 0.0f; frameNm = 4; msg_run__9Generator(object); expect(order == "eocsH", "frame gate returns");
    setup(0, 2); hit_result = 0.0f; frameNm = -1; object[569] = 0xff; msg_run__9Generator(object); expect(order == "eocsHcsB", "negative byte matches -1");
    setup(0, 2); hit_result = 0.0f; frameNm = 3; object[569] = 0x0b; msg_run__9Generator(object); expect(order == "eocsHcsB", "equal low bits pass");
    // bit 1 clear, bit 0 clear: hitscan only
    setup(0, 0); msg_run__9Generator(object);
    expect(order == "eocsHS" && state_calls.size() == 1, "branch B sets state");
    setup(0, 0); hit_result = 2.0f; msg_run__9Generator(object); expect(order == "eocsH", "branch B no hit");
    // bit 1 clear, bit 0 set: return
    setup(0, 1); msg_run__9Generator(object); expect(order == "eo", "bit 0 set returns");
    setup(0, 0xfffffffdu); msg_run__9Generator(object); expect(order == "eo", "bit 0 set (other bits) returns");
    // state 3
    setw(node, 12, vslots_b::address_bits(first_obj));
    setup(3, 0); setw(object, 984, 0); setw(object, 976, 1); setw(object, 972, 3); setw(object, 960, address_bits(name_text));
    created_object = created; setw(created, 12, 0x20); vslots_b::write_u64(vslots_b::address(created) + 160u, 0x1100000000ull);
    msg_run__9Generator(object);
    expect(order == "eoDCL" && destroyed == object + 984, "spawn order");
    expect(create_name == reinterpret_cast<char*>(name_text) && create_angle == 0x123 && create_args == nullptr, "create arguments");
    expect(create_point[0] == 10.0f && create_point[2] == 30.0f, "create point");
    expect(link_head == object + 984 && link_node == created && link_item == created, "link arguments");
    expect(created_u64_at_link == 0x1101000000ull, "u64 or before the link");
    expect(getw(object, 976) == 2 && getw(object, 984) == 0, "counter and list head");
    // not below the limit
    setup(3, 0); setw(object, 984, 0); setw(object, 976, 3); setw(object, 972, 3); msg_run__9Generator(object); expect(order == "eo", "counter at limit");
    setup(3, 0); setw(object, 984, 0); setw(object, 976, 0xffffffffu); setw(object, 972, 0); setw(object, 960, address_bits(name_text)); created_object = nullptr;
    msg_run__9Generator(object); expect(order == "eoDC" && getw(object, 976) == 0, "null object: counter still incremented, signed compare");
    // non-empty list: bit 3 of the first object decides
    setw(first_obj, 12, 0);
    setup(3, 0); setw(object, 984, address_bits(node)); setw(object, 976, 0); setw(object, 972, 5); msg_run__9Generator(object);
    expect(order == "eo", "list head object without bit 3 returns");
    setw(first_obj, 12, 8);
    setup(3, 0); setw(object, 984, address_bits(node)); setw(object, 976, 0); setw(object, 972, 5); setw(object, 960, address_bits(name_text));
    created_object = created; setw(created, 12, 0x01); msg_run__9Generator(object);
    expect(order == "eoDC" && getw(object, 976) == 1 && getw(object, 984) == 0, "bit 3 set continues; created object without bit 5 is not linked");
    return failures ? 1 : 0;
}
