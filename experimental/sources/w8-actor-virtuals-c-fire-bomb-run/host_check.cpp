#define NORRATH_W8C_HOST 1
#include "run.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
unsigned syncRandState = 0;
static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static int find_calls = 0, script_calls = 0, script_answer = 1, visible_answer = 1, visible_calls = 0, bomblet_calls = 0;
static char tag_text[16], tag_result[] = "go";
static char* tag_answer = nullptr;
static float visible_range = 0.0f, bomblet_pos[3], bomblet_anchor[3], bomblet_vel[3];
static bool vel_null = false;
static short bomblet_heading = 0; static int bomblet_value = 0; static void* bomblet_marker = nullptr;
static int heading_seen = 0;
static unsigned char block[200];
static unsigned char* alloc_answer = block;
extern "C" char* norrath_actor_find_tag_string(void*, char* tag, int occurrence) {
    ++find_calls; std::strcpy(tag_text, tag); expect(occurrence == 0, "occurrence zero"); return tag_answer;
}
extern "C" int scriptStart__FPc(char* name) { ++script_calls; expect(name == tag_result, "script gets the tag text"); return script_answer; }
extern "C" int gamePositionVisible__FP6Point3f(const void* p, float range) { ++visible_calls; visible_range = range; (void)p; return visible_answer; }
extern "C" float icos__Fi(int a) { heading_seen = a; return 3.0f; }
extern "C" float isin__Fi(int) { return 4.0f; }
extern "C" unsigned char* norrath_block_alloc(int size) { expect(size == 184, "184-byte block"); return alloc_answer; }
extern "C" void __11FireBombletG6Point3sPcT1P6Point3i(void* self, const void* pos, short heading, char* marker, const void* anchor,
                                                    const void* velocity, int value) {
    ++bomblet_calls; expect(self == block, "block");
    std::memcpy(bomblet_pos, pos, 12); std::memcpy(bomblet_anchor, anchor, 12);
    vel_null = velocity == nullptr;
    if (velocity) std::memcpy(bomblet_vel, velocity, 12);
    bomblet_heading = heading; bomblet_marker = marker; bomblet_value = value;
}
static unsigned char object[200];
static void setup(unsigned old, unsigned period, unsigned char b78) {
    using namespace w8c;
    std::memset(object, 0, sizeof object);
    write_word(address(object) + 84u, old); write_word(address(object) + 80u, period);
    object[78] = b78; object[79] = 0xfe; write_u16(address(object) + 56u, 100u);
    write_single(address(object) + 16u, 10.0f); write_single(address(object) + 20u, 20.0f); write_single(address(object) + 24u, 30.0f);
    find_calls = script_calls = visible_calls = bomblet_calls = 0;
    tag_answer = nullptr; script_answer = 1; visible_answer = 1; alloc_answer = block;
}
int main() {
    using namespace w8c;
    setup(2, 3, 1);
    msg_run__8FireBomb(object);
    expect(read_word(address(object) + 84u) == 3u && find_calls == 0, "counter advances, nothing else");
    setup(3, 3, 1); syncRandState = 0;
    msg_run__8FireBomb(object);
    expect(read_word(address(object) + 84u) == 0u && find_calls == 1 && std::strcmp(tag_text, "script") == 0 && script_calls == 0, "counter restarts; no script");
    expect(visible_calls == 1 && visible_range == 960.0f && bomblet_calls == 1, "visible check and bomblet");
    expect(syncRandState == 0x3c6ef35fu, "generator advanced once");
    expect(heading_seen == static_cast<short>(100 + (0x3c6ef35fu >> 16) % 8123 - 4061), "heading drawn from the generator");
    expect(std::fabs(bomblet_anchor[0] - (10.0f + 0.6f * 401.0f)) < 0.01f && std::fabs(bomblet_anchor[1] - (20.0f + 0.8f * 401.0f)) < 0.01f, "anchor ahead of position by K1");
    expect(bomblet_anchor[2] == 30.0f + 357.0f, "anchor height");
    expect(!vel_null && bomblet_marker == object + 77 && bomblet_value == -2 && bomblet_heading == 100, "bomblet args");
    expect(bomblet_pos[0] == 10.0f && bomblet_pos[2] == 30.0f, "position copy");
    setup(3, 3, 0); syncRandState = 0;
    msg_run__8FireBomb(object);
    expect(syncRandState != 0x3c6ef35fu && bomblet_calls == 1 && vel_null, "second draw, null velocity");
    expect(bomblet_anchor[2] == 30.0f - 1037.0f, "second branch height");
    const float reach = static_cast<float>(((syncRandState >> 16) % 386u) + 332);
    expect(std::fabs(bomblet_anchor[0] - (10.0f + 0.6f * reach)) < 0.05f, "reach from the second draw");
    setup(3, 3, 1); tag_answer = tag_result; script_answer = 0;
    msg_run__8FireBomb(object);
    expect(find_calls == 2 && script_calls == 1 && visible_calls == 0, "zero script result ends the call");
    setup(3, 3, 1); tag_answer = tag_result; script_answer = 9;
    msg_run__8FireBomb(object);
    expect(visible_calls == 1 && bomblet_calls == 1, "nonzero script result continues");
    setup(3, 3, 1); visible_answer = 0;
    msg_run__8FireBomb(object);
    expect(bomblet_calls == 0, "invisible ends the call");
    setup(3, 3, 1); alloc_answer = nullptr;
    msg_run__8FireBomb(object);
    expect(bomblet_calls == 0, "null block ends the call");
    return failures ? 1 : 0;
}
