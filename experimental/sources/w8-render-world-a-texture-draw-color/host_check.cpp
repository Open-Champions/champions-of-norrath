#include <cstdio>
#include <cstring>
struct Packet;
extern "C" unsigned char* glistp;
extern "C" unsigned char* gstart;
extern "C" int GLISTSIZE;
unsigned char* glistp;
unsigned char* gstart;
int GLISTSIZE;
static int draw_calls, list_calls;
static int seen_args[6];
static void* seen_list_packet;
static int seen_order;
static void* seen_record;
static int seen_flag;
static unsigned long long* cursor_at_call;
static int extra_units = 6;  // 8-byte units the draw call appends
#include "texture_draw_color.cpp"
extern "C" void drawTexture__FR6PacketP8_textureiiiiii(Packet& packet, void*, int a, int b, int c, int d, int e, int f) {
    ++draw_calls;
    seen_args[0] = a; seen_args[1] = b; seen_args[2] = c; seen_args[3] = d; seen_args[4] = e; seen_args[5] = f;
    ::Packet& real = reinterpret_cast<::Packet&>(packet);
    cursor_at_call = real.cursor;
    for (int i = 0; i < extra_units; ++i) real.cursor[i] = 0xAA00ull + i;
    real.cursor += extra_units;
}
extern "C" void drawAddToList__FPviP8_textureP11_drawRecordi(void* packet, int order, void*, void* record, int flag) {
    ++list_calls; seen_list_packet = packet; seen_order = order; seen_record = record; seen_flag = flag;
}
static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
int main() {
    alignas(16) static unsigned long long buffer[4096];
    short texture[8] = {64, 32, 0, 0, 0, 0, 0, 0};
    gstart = reinterpret_cast<unsigned char*>(buffer);
    glistp = gstart + 800;  // 100 units used
    unsigned long long* const start = reinterpret_cast<unsigned long long*>(glistp);
    GLISTSIZE = 3000 + 100 - 1;
    _textureDrawColor__FP8_textureiiiUi(texture, 10, 20, 7, 0x80402010u);
    expect(draw_calls == 0 && list_calls == 0 && glistp == gstart + 800, "fewer than 3000 units left: nothing happens");
    GLISTSIZE = 3000 + 100;
    _textureDrawColor__FP8_textureiiiUi(texture, 10, 20, 7, 0x80402010u);
    expect(draw_calls == 1 && list_calls == 1, "one draw call and one list call");
    expect(seen_args[0] == 10 && seen_args[1] == 21 && seen_args[2] == 0 && seen_args[3] == 0 && seen_args[4] == 64 && seen_args[5] == 32,
           "draw arguments: x, y + 1, 0, 0, texture halfwords");
    expect(cursor_at_call == start + 18, "draw call starts after the 144-byte header");
    unsigned words[4];
    std::memcpy(words, &start[1], 8);
    expect(words[0] == 0x11000000u, "flush code at +8");
    const unsigned units = 18 + extra_units;  // 24 units: header 18 plus the 6 drawn
    expect(units == 24, "unit count assumed by the literals below");
    expect(start[0] == 0x6000000Bull, "tag: (24 - 2) >> 1 = 11 as the count, id 1 turned into id 6");
    expect(words[1] == 0x5000000Bu, "direct count: (192 - 12 - 4) >> 4 = 11");
    expect(start[2] == 0x100000000000800Aull && start[3] == 14ull, "GIF tag: loop count (22 - 2) >> 1 = 10");
    expect(start[4] == 0 && start[5] == 0x3f && start[6] == 0 && start[7] == 6 && start[8] == 1 && start[9] == 0x14, "first three pairs");
    expect(start[10] == 0x80402010ull && start[11] == 1, "colour pair");
    expect(start[12] == 0x44 && start[13] == 0x42 && start[14] == 0x156 && start[15] == 0 && start[16] == 0x3001dull && start[17] == 0x47, "last pairs");
    expect(start[18] == 0xAA00ull, "drawn data stays in place");
    expect(glistp == reinterpret_cast<unsigned char*>(start + units), "glistp moves to the end");
    expect(seen_list_packet == start && seen_order == 7 && seen_record == nullptr && seen_flag == 1, "list call arguments");
    return failures;
}
