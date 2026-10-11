#define NORRATH_VSLOTS_B_HOST 1
#include "msg_hurt.cpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
static int atan_calls = 0;
static int destroy_calls = 0;
static int link_calls = 0;
static float seen_first = 0;
static float seen_second = 0;
static int atan_result = 100;
static void* seen_head = nullptr;
static void* seen_link = nullptr;
static void* seen_value = nullptr;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" unsigned frameNm = 77u;
extern "C" int iatan2__Fff(float first, float second) {
    ++atan_calls;
    seen_first = first;
    seen_second = second;
    return atan_result;
}
extern "C" void norrath_list_destroy(void* head) {
    ++destroy_calls;
    seen_head = head;
    unsigned char* object = static_cast<unsigned char*>(head) - 752;
    write_single(vslots_b::address(object) + 560u, 10.0f);
    write_single(vslots_b::address(object) + 192u, 3.0f);
}
extern "C" void* norrath_list_oneway_link(void* head, void* object_head, void* value) {
    ++link_calls;
    seen_head = head;
    seen_link = object_head;
    seen_value = value;
    return value;
}
static void put_single(unsigned char* object, unsigned offset, float value) {
    write_single(vslots_b::address(object) + offset, value);
}
static float get_single(unsigned char* object, unsigned offset) {
    return read_single(vslots_b::address(object) + offset);
}
int main() {
    unsigned char object[900];
    unsigned char other[120];
    std::memset(object, 0x11, sizeof object);
    std::memset(other, 0x22, sizeof other);
    object[570] = 1;
    vslots_b::write_word(vslots_b::address(object) + 744u, 0x0100u);
    msg_hurt__8CreaturefP10GameObjecti(object, 3.0f, other, 0x80);
    expect(object[570] == 1 && vslots_b::read_word(vslots_b::address(object) + 744u) == 0x0100u, "blocked byte writes nothing");
    expect(atan_calls == 0 && destroy_calls == 0 && link_calls == 0, "blocked byte makes no calls");

    std::memset(object, 0x11, sizeof object);
    object[570] = 0;
    vslots_b::write_word(vslots_b::address(object) + 744u, 0x1u);
    put_single(object, 560u, 4.0f);
    put_single(object, 192u, 1.5f);
    msg_hurt__8CreaturefP10GameObjecti(object, 1.25f, nullptr, 0x10);
    expect(vslots_b::read_word(vslots_b::address(object) + 744u) == 0x11u, "null other ORs the kind");
    expect(vslots_b::read_word(vslots_b::address(object) + 812u) == 77u, "frame number is copied");
    expect(get_single(object, 560u) == 2.75f, "null other reduces 560");
    expect(get_single(object, 192u) == 2.75f, "null other increases 192");
    expect(atan_calls == 0, "null other skips the angle");

    std::memset(object, 0x11, sizeof object);
    std::memset(other, 0, sizeof other);
    object[570] = 0;
    put_single(object, 560u, -1.0f);
    put_single(object, 192u, 8.0f);
    vslots_b::write_word(vslots_b::address(object) + 744u, 0u);
    int before = atan_calls;
    msg_hurt__8CreaturefP10GameObjecti(object, 2.0f, other, 1);
    expect(atan_calls == before && destroy_calls == 0, "non-positive 560 skips the list");
    expect(get_single(object, 560u) == -3.0f, "non-positive 560 still subtracts");

    unsigned char inner[20];
    unsigned char linked[4];
    std::memset(object, 0x11, sizeof object);
    std::memset(other, 0, sizeof other);
    std::memset(inner, 0, sizeof inner);
    object[570] = 0;
    put_single(object, 16u, 5.0f);
    put_single(object, 20u, 9.0f);
    put_single(object, 560u, 2.0f);
    put_single(object, 192u, 1.0f);
    put_single(other, 16u, 1.0f);
    put_single(other, 20u, 4.0f);
    vslots_b::write_word(vslots_b::address(object) + 744u, 0x4u);
    vslots_b::write_word(vslots_b::address(other) + 12u, 0x80u);
    vslots_b::host_note(inner);
    vslots_b::write_word(vslots_b::address(other) + 76u, vslots_b::address_bits(inner));
    vslots_b::host_note(linked);
    vslots_b::write_word(vslots_b::address(inner) + 12u, vslots_b::address_bits(linked));
    atan_result = 16380;
    destroy_calls = 0;
    link_calls = 0;
    msg_hurt__8CreaturefP10GameObjecti(object, 0.5f, other, 0x2);
    expect(atan_calls == before + 1, "positive 560 calls atan");
    expect(seen_first == 4.0f && seen_second == 5.0f, "atan gets the point differences");
    expect(object[748] == 4 && object[749] == 0, "16384 minus atan is stored");
    expect(destroy_calls == 1 && seen_head == object + 752, "destroy runs at 752");
    expect(link_calls == 1 && seen_link == linked && seen_value == linked, "bit 0x80 follows the inner word");
    expect(vslots_b::read_word(vslots_b::address(object) + 744u) == 0x6u, "kind is ORed");
    expect(get_single(object, 560u) == 9.5f && get_single(object, 192u) == 3.5f, "singles reload after the list");
    return failures ? 1 : 0;
}
