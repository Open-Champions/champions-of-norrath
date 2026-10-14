#define NORRATH_HOST_FLOOR_DRAW 1
#include "../floor-storage/floor_storage.cpp"
#include "floor_draw.cpp"

#include <cstdint>
#include <cstdio>
#include <cstring>

unsigned long long norrath_floor_host_address_base;
extern "C" {
unsigned char norrath_floor_program_start_view[32];
unsigned char norrath_floor_program_end_view[32];
unsigned glistp;
unsigned long long zwritesOff;
unsigned long long zwritesOn;
unsigned char worldWorldViewMatrix[64];
unsigned char worldCameraPos[12];
}

static int pause_calls;
static int pause_result;
static int cosine_calls;
static int sine_calls;
static int clip_calls;
static int clip_result;
static int construct_calls;
static char* construct_group;
static char* construct_name;
static int draw_calls;
static void* draw_packet;
static int draw_list;
static void* draw_texture;
static void* draw_record;
static int draw_flags;
static int failures;

static void expect(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned read_half(const unsigned char* p) {
    return p[0] | (unsigned(p[1]) << 8);
}
static void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
static void write_float(unsigned char* p, float value) {
    std::memcpy(p, &value, 4);
}

extern "C" int norrath_game_is_paused() {
    ++pause_calls;
    return pause_result;
}
extern "C" float norrath_integer_cosine(int) {
    ++cosine_calls;
    return 0.0f;
}
extern "C" float norrath_integer_sine(int) {
    ++sine_calls;
    return 0.0f;
}
extern "C" int worldClipCubeToFrustum__FPA1_fi(float*, int) {
    ++clip_calls;
    return clip_result;
}
extern "C" void __11ResourceMapPcT1(void*, char* group, char* name) {
    ++construct_calls;
    construct_group = group;
    construct_name = name;
}
extern "C" void* norrath_player_resource_map_get(void*) {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(0x51u));
}
extern "C" void* drawAddToList__FPviP8_textureP11_drawRecordi(
    void* packet, int list, void* texture, void* record, int flags) {
    ++draw_calls;
    draw_packet = packet;
    draw_list = list;
    draw_texture = texture;
    draw_record = record;
    draw_flags = flags;
    return nullptr;
}

static unsigned char cursor[65536];
static unsigned char targets[8];
static unsigned char packet[768];

static void reset_calls() {
    pause_calls = cosine_calls = sine_calls = clip_calls = 0;
    construct_calls = draw_calls = 0;
    pause_result = 0;
    clip_result = 0;
    construct_group = construct_name = nullptr;
    draw_packet = draw_texture = draw_record = nullptr;
    draw_list = draw_flags = -1;
}

int main() {
    norrath_floor_host_address_base =
        reinterpret_cast<uintptr_t>(&floorDrawEnable) & ~0xffffffffull;
    auto round_trip = [](void* pointer) {
        const unsigned low = static_cast<unsigned>(reinterpret_cast<uintptr_t>(pointer));
        return reinterpret_cast<void*>(norrath_floor_host_address_base + low) == pointer;
    };
    expect(round_trip(&glistp), "address round trip");
    expect(round_trip(norrath_floor_sprite_grid_view), "grid round trip");

    floorDrawEnable = 0;
    reset_calls();
    floorDraw__Fv();
    expect(pause_calls == 0 && draw_calls == 0, "disabled draw does not run");

    floorDrawEnable = 1;
    write_word(norrath_floor_sprite_count_word_view, 0);
    reset_calls();
    floorDraw__Fv();
    expect(pause_calls == 0 && cosine_calls == 0, "empty sprite count exits first");

    write_word(norrath_floor_sprite_count_word_view, 1);
    std::memset(norrath_floor_expand_list_view, 0, sizeof norrath_floor_expand_list_view);
    norrath_floor_expand_list_view[0] = 2;
    write_word(norrath_floor_expand_list_view + 4, static_cast<unsigned>(
        reinterpret_cast<uintptr_t>(targets)));
    write_float(norrath_floor_expand_list_view + 12, 0.0f);
    write_float(norrath_floor_expand_list_view + 16, 10.0f);
    norrath_floor_expand_list_view[20] = 1;
    write_word(norrath_floor_expand_list_view + 24, static_cast<unsigned>(
        reinterpret_cast<uintptr_t>(targets + 4)));
    write_float(norrath_floor_expand_list_view + 32, 0.0f);
    write_float(norrath_floor_expand_list_view + 36, 10.0f);
    write_word(norrath_floor_sprite_texture_ready_word, 0);
    glistp = static_cast<unsigned>(reinterpret_cast<uintptr_t>(cursor));
    std::memset(cursor, 0, sizeof cursor);
    pause_result = 0;
    clip_result = 0;
    reset_calls();
    pause_result = 0;
    clip_result = 0;
    floorDraw__Fv();
    expect(pause_calls == 1 && cosine_calls == 2 && sine_calls == 2, "unpaused expansion calls");
    expect(read_word(norrath_floor_expand_list_view + 12) == 0x3d4cccccu, "kind 2 adds 0.05");
    expect(read_word(norrath_floor_expand_list_view + 32) == 0x3e4cccccu, "other kind adds 0.2");
    expect(clip_calls == 1 && construct_calls == 1, "culled draw still clips and constructs");
    expect(std::strcmp(construct_group, "engine.lmp") == 0, "texture group");
    expect(std::strcmp(construct_name, "floorsprite.tex") == 0, "texture name");
    expect(draw_calls == 1 && draw_list == 0 && draw_flags == 0 && draw_record == nullptr,
           "draw list arguments");
    expect(draw_texture == reinterpret_cast<void*>(static_cast<uintptr_t>(0x51u)),
           "texture pointer");

    std::memset(packet, 0, sizeof packet);
    write_word(packet + 760, 1);
    write_word(packet + 764, 50);
    packet[14] = 1;
    packet[6] = 1;
    std::memset(norrath_floor_sprite_grid_view, 0, sizeof norrath_floor_sprite_grid_view);
    write_word(norrath_floor_sprite_grid_view, static_cast<unsigned>(
        reinterpret_cast<uintptr_t>(packet)));
    write_word(norrath_floor_fade_packet_word_view, 0);
    write_word(norrath_floor_blood_level_word_view, 0);
    glistp = static_cast<unsigned>(reinterpret_cast<uintptr_t>(cursor));
    pause_result = 1;
    clip_result = 0x803f;
    clip_calls = 0;
    const int cosine_before = cosine_calls;
    floorDraw__Fv();
    expect(cosine_calls == cosine_before, "paused draw skips expansion");
    expect(clip_calls == 1, "full mask is clipped once");
    expect(read_word(packet + 760) == 0u, "expired sprite leaves the count");
    expect(read_word(packet + 764) == 49u, "contribution drops with the sprite");
    expect(read_half(packet + 6) == 0xfffdu, "alpha loses 4 before removal");
    expect(read_word(norrath_floor_fade_packet_word_view) == 0u, "fade packet is cleared");

    write_word(packet + 760, 1);
    write_word(packet + 764, 50);
    packet[6] = 1;
    write_word(norrath_floor_blood_level_word_view, 1);
    write_word(norrath_floor_fade_packet_word_view, 0);
    glistp = static_cast<unsigned>(reinterpret_cast<uintptr_t>(cursor));
    floorDraw__Fv();
    expect(read_word(packet + 760) == 1u, "blood keeps the sprite");
    expect(read_word(norrath_floor_fade_packet_word_view) ==
               static_cast<unsigned>(reinterpret_cast<uintptr_t>(packet)),
           "blood still publishes the fade packet");

    if (failures != 0) {
        std::fprintf(stderr, "%d floor draw checks failed\n", failures);
        return 1;
    }
    std::puts("floor draw contract passed");
    return 0;
}
