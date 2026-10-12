#define NORRATH_VSLOTS_B_HOST 1
#include "msg_get_poke_name.cpp"
#include <cstdio>
#include <cstring>
extern "C" unsigned char player[23984u * 2u] = {};
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
static int cos_angle = 0, sin_angle = 0, cos_calls = 0, sin_calls = 0;
static float cos_value = 1.0f, sin_value = 0.0f;
static char key_seen[32];
static unsigned short text[3] = {9, 8, 0};
static unsigned short* copy_dest = nullptr;
static unsigned short* copy_source = nullptr;
extern "C" float icos__Fi(int angle) { cos_angle = angle; ++cos_calls; return cos_value; }
extern "C" float isin__Fi(int angle) { sin_angle = angle; ++sin_calls; return sin_value; }
extern "C" unsigned short* textGetString__FPc(char* key) { std::strcpy(key_seen, key); return text; }
extern "C" void ustrcpy__FPUsT0(unsigned short* destination, unsigned short* source) { copy_dest = destination; copy_source = source; }
static void set_single(unsigned char* base, unsigned offset, float value) {
    unsigned bits;
    std::memcpy(&bits, &value, 4);
    vslots_b::write_word(vslots_b::address(base) + offset, bits);
}
static void run(const char* label, unsigned char* object, unsigned char* entity, int index, unsigned short* buffer, const char* want) {
    vslots_b::host_note(entity);
    vslots_b::write_word(vslots_b::address(player) + static_cast<unsigned>(index) * 23984u + 21872u, vslots_b::address_bits(entity));
    key_seen[0] = 0; copy_dest = nullptr;
    const int result = msg_getPokeName__5LeverPUsi(object, buffer, index);
    expect(result == 1, label);
    expect(std::strcmp(key_seen, want) == 0, label);
    expect(copy_dest == buffer && copy_source == text, label);
}
int main() {
    unsigned char object[96];
    unsigned char entity[64];
    unsigned short buffer[4] = {};
    std::memset(object, 0, sizeof object);
    std::memset(entity, 0, sizeof entity);
    vslots_b::write_u16(vslots_b::address(object) + 56u, 0x2000u);
    set_single(object, 16, 1.0f); set_single(object, 20, 2.0f); set_single(object, 24, 3.0f);
    object[80] = 1;
    cos_value = 1.0f; sin_value = 0.0f;
    // entity ahead on x: dot = 4 * 1 > 0
    set_single(entity, 16, 5.0f); set_single(entity, 20, 2.0f); set_single(entity, 24, 3.0f);
    run("ahead pulls", object, entity, 0, buffer, "PullLever");
    expect(cos_angle == 0x2000 - 16384 && sin_angle == 0x2000 - 16384 && cos_calls == 1 && sin_calls == 1, "angle is half minus 16384");
    // entity behind: dot = -2 -> push
    set_single(entity, 16, -1.0f);
    run("behind pushes", object, entity, 0, buffer, "PushLever");
    // negative scale byte flips it
    object[80] = 0xff;
    run("negative scale pulls", object, entity, 0, buffer, "PullLever");
    set_single(entity, 16, 5.0f);
    run("negative scale ahead pushes", object, entity, 0, buffer, "PushLever");
    // zero scale: result zero is not greater than zero
    object[80] = 0;
    run("zero scale pushes", object, entity, 0, buffer, "PushLever");
    // y term uses sin, z term uses 0.0
    object[80] = 1;
    cos_value = 0.0f; sin_value = 1.0f;
    set_single(entity, 16, 1.0f); set_single(entity, 20, 9.0f); set_single(entity, 24, -100.0f);
    run("y term pulls, z ignored", object, entity, 0, buffer, "PullLever");
    // second record index
    set_single(entity, 20, -9.0f);
    run("record 1 pushes", object, entity, 1, buffer, "PushLever");
    return failures ? 1 : 0;
}
