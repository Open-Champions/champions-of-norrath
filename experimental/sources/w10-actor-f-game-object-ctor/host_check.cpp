// Host contract for the GameObject constructor. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#define _ctype_ host_ctype_table
#include "game_object_ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
const unsigned char norrath_game_object_vtable_view[8] = {};
unsigned char norrath_constructor_default_head_view[8];
unsigned char norrath_constructor_grid_a_view[120 * 120 * 4];
unsigned char norrath_constructor_grid_b_view[120 * 120 * 4];
unsigned char norrath_constructor_random_state_view[4];
unsigned char norrath_constructor_resource_frame_view[4];
unsigned char norrath_constructor_tag_a_view[8] = {'h', 'i', 'd', 'e', 0};
unsigned char norrath_constructor_tag_b_view[8] = {'n', 'a', 'm', 'e', 0};
}

// Upper-case letters carry bit 1 in the class table (index 1 + character).
extern "C" const unsigned char host_ctype_table[257] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static std::string log_text;
static int find_tag_result;
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s (log %s)\n", m, log_text.c_str()); ++failures; } }
static void add(const std::string& t) { log_text += t; log_text += ';'; }
static unsigned char object[1300], mapmem[128], args_mem[64], strings[512], node_mem[64], record_mem[64], prev_slot[16], next_rec[16], other_rec[16];
static unsigned lump_counter;
static unsigned char* link_head;

extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) {
    add(std::string("lump/") + lump + "/" + name);
    return reinterpret_cast<void*>(0x2000u + 16u * (++lump_counter));
}
extern "C" void* listOnewayLink__FP8ListHeadT0Pv(void* head, void* node, void* item) {
    link_head = static_cast<unsigned char*>(head);
    add(std::string("link") + (node == object + 8 ? "/node" : "/bad") + (item == object ? "/item" : "/bad"));
    vslots_b::write_word(vslots_b::address(object) + 8u, vslots_b::address_bits(node_mem));
    return nullptr;
}
extern "C" int findTagInt__10GameObjectPci(void*, char* tag, int n) { add(std::string("tag/") + tag + "/" + std::to_string(n)); return find_tag_result; }
extern "C" int host_strncmp(const char* a, const char* b, unsigned n) asm("strncmp");
extern "C" int host_strncmp(const char* a, const char* b, unsigned n) {
    for (unsigned i = 0; i < n; ++i) {
        if (a[i] != b[i]) return static_cast<unsigned char>(a[i]) - static_cast<unsigned char>(b[i]);
        if (!a[i]) return 0;
    }
    return 0;
}
extern "C" void objectAddString__FPcP10GameObject(char* s, void*) { add(std::string("add/") + (s ? s : "null")); }

static unsigned getw(unsigned char* base, unsigned off) { return vslots_b::read_word(vslots_b::address(base) + off); }
static void setw(unsigned char* base, unsigned off, unsigned v) { vslots_b::write_word(vslots_b::address(base) + off, v); }
static void reset() {
    std::memset(object, 0x41, sizeof object);
    std::memcpy(norrath_constructor_tag_b_view, "name\0\0\0", 8);
    log_text.clear(); find_tag_result = 0; lump_counter = 0; link_head = nullptr;
    setw(norrath_constructor_random_state_view, 0, 12345u);
    setw(norrath_constructor_resource_frame_view, 0, 7u);
}
static void construct(float x, float y, float z, void* map, char** args) {
    const float p[3] = {x, y, z};
    __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(object, p, map, args);
}
int main() {
    using namespace vslots_b;
    host_note(object); host_note(mapmem); host_note(args_mem); host_note(node_mem); host_note(record_mem);
    host_note(prev_slot); host_note(next_rec); host_note(other_rec);
    host_note(norrath_constructor_default_head_view); host_note(const_cast<unsigned char*>(norrath_game_object_vtable_view));
    for (unsigned i = 0; i < 6; ++i) host_note(strings + 16 * i);
    for (unsigned i = 1; i <= 4; ++i) host_note(reinterpret_cast<void*>(0x2000u + 16u * i));
    // plain construction
    reset();
    construct(100.0f, -20.5f, 33.0f, nullptr, nullptr);
    expect(getw(object, 0) == 0 && getw(object, 4) == 0 && getw(object, 12) == 0 && getw(object, 52) == 0 && getw(object, 36) == 0 && getw(object, 44) == 0, "zeroed words");
    expect(getw(object, 72) == address_bits(norrath_game_object_vtable_view) && getw(object, 28) == 0x41c00000u && getw(object, 32) == 0x42900000u, "vtable and floats");
    expect(object[40] == 25 && object[41] == 73, "bytes");
    { float got[3]; std::memcpy(got, object + 16, 12); expect(got[0] == 100.0f && got[1] == -20.5f && got[2] == 33.0f, "position copied"); }
    expect(getw(object, 68) == ((12345u * 1664525u + 1013904223u) >> 16) && getw(norrath_constructor_random_state_view, 0) == 12345u * 1664525u + 1013904223u, "seed");
    expect(log_text == "link/node/item;tag/hide/0;" && link_head == norrath_constructor_grid_a_view + 4 * (120 * 59 + 60), "grid cell (60, 59)");
    // position overlapping the object: the copy is taken first
    reset();
    { const float p[3] = {5.0f, 6.0f, 7.0f}; std::memcpy(object + 16, p, 12); }
    __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(object, object + 16, nullptr, nullptr);
    { float got[3]; std::memcpy(got, object + 16, 12); expect(got[0] == 5.0f && got[1] == 6.0f && got[2] == 7.0f, "overlapping position survives"); }
    // grid cells and the off-grid list
    struct Case { float x, y; int cx, cy; };
    const Case cases[] = {{0.0f, 0.0f, 60, 60}, {-15360.0f, 0.0f, 0, 60}, {15200.0f, 15200.0f, 119, 119}, {0.0f, 8000.0f, 91, 60},
                          {-15361.0f, 0.0f, -1, 60}, {15360.0f, 0.0f, 120, 60}, {0.0f, -15361.0f, 60, -1}, {0.0f, 15360.0f, 60, 120}};
    for (const Case& c : cases) {
        reset();
        construct(c.x, c.y, 0.0f, nullptr, nullptr);
        const bool inside = c.cx >= 0 && c.cx < 120 && c.cy >= 0 && c.cy < 120;
        if (c.x == 0.0f && c.y == 8000.0f) { expect(link_head == norrath_constructor_grid_a_view + 4 * (120 * 91 + 60), "cell (60, 91)"); continue; }
        if (inside) expect(link_head == norrath_constructor_grid_a_view + 4 * (120 * c.cy + c.cx), "grid cell");
        else expect(link_head == norrath_constructor_default_head_view, "off the grid");
    }
    // resource map: same stamp does nothing; a stale stamp with no entries only restamps; entries call the loader
    std::memcpy(strings, "lump.lmp\0", 9); std::memcpy(strings + 16, "res_a\0", 6); std::memcpy(strings + 32, "res_b\0", 6);
    reset(); std::memset(mapmem, 0, sizeof mapmem); setw(mapmem, 0, address_bits(strings)); setw(mapmem, 4, 7u);
    construct(0, 0, 0, mapmem, nullptr);
    expect(log_text.find("lump/") == std::string::npos && getw(mapmem, 4) == 7u && getw(object, 36) == address_bits(mapmem), "same stamp: nothing");
    reset(); setw(mapmem, 4, 3u); setw(mapmem, 8, 0u);
    construct(0, 0, 0, mapmem, nullptr);
    expect(log_text.find("lump/") == std::string::npos && getw(mapmem, 4) == 7u, "stale stamp, no entries: restamp only");
    reset(); setw(mapmem, 4, 3u); setw(mapmem, 8, address_bits(strings + 16)); setw(mapmem, 16, address_bits(strings + 32)); setw(mapmem, 24, 0u);
    construct(0, 0, 0, mapmem, nullptr);
    expect(log_text.find("lump/lump.lmp/res_a;lump/lump.lmp/res_b;") == 0 && getw(mapmem, 12) == 0x2010u && getw(mapmem, 20) == 0x2020u && getw(mapmem, 4) == 7u, "two entries loaded and stored");
    // hide tag: the list node moves to the front of the default list
    reset(); find_tag_result = 1;
    std::memset(node_mem, 0, sizeof node_mem); std::memset(record_mem, 0, sizeof record_mem);
    std::memset(prev_slot, 0, sizeof prev_slot); std::memset(next_rec, 0, sizeof next_rec); std::memset(other_rec, 0, sizeof other_rec);
    setw(node_mem, 8, address_bits(record_mem));
    setw(record_mem, 0, address_bits(next_rec)); setw(record_mem, 4, address_bits(prev_slot));
    setw(norrath_constructor_default_head_view, 0, address_bits(other_rec));
    construct(0, 0, 0, nullptr, nullptr);
    expect(getw(prev_slot, 0) == address_bits(next_rec), "old neighbour linked past the record");
    expect(getw(next_rec, 4) == address_bits(prev_slot), "next record points back to the old previous");
    expect(getw(record_mem, 4) == address_bits(norrath_constructor_default_head_view) && getw(record_mem, 0) == address_bits(other_rec), "record pushed in front");
    expect(getw(norrath_constructor_default_head_view, 0) == address_bits(record_mem) && getw(other_rec, 4) == address_bits(record_mem), "head and old first record updated");
    // with no next record the back link is not written
    reset(); find_tag_result = 1;
    std::memset(prev_slot, 0, sizeof prev_slot); std::memset(next_rec, 0x77, sizeof next_rec);
    setw(record_mem, 0, 0u); setw(record_mem, 4, address_bits(prev_slot));
    setw(norrath_constructor_default_head_view, 0, 0u);
    construct(0, 0, 0, nullptr, nullptr);
    expect(getw(prev_slot, 0) == 0u && next_rec[4] == 0x77 && getw(norrath_constructor_default_head_view, 0) == address_bits(record_mem) && getw(record_mem, 0) == 0u, "no next record, empty default list");
    // arguments: the tag is lower-cased in place; the n-th "name=" string is chosen; each is added once, in order
    reset();
    std::memcpy(norrath_constructor_tag_b_view, "NaMe\0", 5);
    std::memcpy(strings, "foo=1\0", 6); std::memcpy(strings + 16, "name=alpha\0", 11); std::memcpy(strings + 32, "namex\0", 6);
    std::memcpy(strings + 48, "NAME=beta\0", 10); std::memcpy(strings + 64, "name\0", 5); std::memcpy(strings + 80, "name=gamma\0", 11);
    host_note(strings); host_note(strings + 16);
    setw(args_mem, 0, address_bits(strings)); setw(args_mem, 4, address_bits(strings + 16)); setw(args_mem, 8, address_bits(strings + 32));
    setw(args_mem, 12, address_bits(strings + 48)); setw(args_mem, 16, address_bits(strings + 64)); setw(args_mem, 20, address_bits(strings + 80)); setw(args_mem, 24, 0u);
    host_note(strings + 80);
    construct(0, 0, 0, nullptr, reinterpret_cast<char**>(args_mem));
    expect(std::memcmp(norrath_constructor_tag_b_view, "name", 5) == 0, "tag lower-cased in place");
    expect(getw(object, 44) == address_bits(args_mem), "args stored");
    expect(log_text.find("add/alpha;add/gamma;") != std::string::npos && log_text.rfind("add/") == log_text.find("add/gamma"), "chosen strings: name=alpha then name=gamma (NAME=beta is upper case and does not match)");
    // an empty argument list adds nothing
    reset(); setw(args_mem, 0, 0u);
    construct(0, 0, 0, nullptr, reinterpret_cast<char**>(args_mem));
    expect(log_text.find("add/") == std::string::npos, "empty list");
    return failures ? 1 : 0;
}
