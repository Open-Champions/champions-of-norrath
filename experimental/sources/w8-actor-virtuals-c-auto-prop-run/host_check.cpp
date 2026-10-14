#define NORRATH_W8C_HOST 1
#include "run.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
unsigned char player[95936];
unsigned char nmPlayers[4];
static int failures = 0;
static int active_answer = 1, advance_calls = 0, attack_calls = 0, strticmp_answer = 0;
static int advance_answer = 0, heading_answer = 0;
static float cos_answer = 0.0f, sin_answer = 0.0f, scale_answer = 1.0f;
static void* attack_victim = nullptr;
static void* attack_attacker = nullptr;
static float attack_scale = 0.0f;
static int attack_ints[4];
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" int objectActiveProcessing__FP10GameObjectii(void*, int, int b) { return b == 480 + 7 ? active_answer : -99; }
extern "C" int animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(void*, void*, void*, short*) { ++advance_calls; return advance_answer; }
extern "C" int strticmp__FPCcT0(const char* l, const char* r) { return std::strcmp(r, "pipepistona") == 0 && std::strcmp(l, "PipePistonA") == 0 ? strticmp_answer : 1; }
extern "C" float icos__Fi(int) { return cos_answer; }
extern "C" float isin__Fi(int) { return sin_answer; }
extern "C" float gameDifficultyScale__Fv() { return scale_answer; }
extern "C" int iatan2__Fff(float, float) { return heading_answer; }
extern "C" void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(
    void* victim, void* attacker, float a, float b, int c, int d, int e, int f, void* g, int h) {
    ++attack_calls; attack_victim = victim; attack_attacker = attacker; attack_scale = a;
    expect(a == b && g == nullptr && h == 0, "scale twice, no description, flag zero");
    attack_ints[0] = c; attack_ints[1] = d; attack_ints[2] = e; attack_ints[3] = f;
}
static unsigned char self_[900], target[900], skipped[900];
static void setup(unsigned phase, unsigned tracking, float tx, float ty) {
    using namespace w8c;
    std::memset(self_, 0, sizeof self_); std::memset(target, 0, sizeof target); std::memset(skipped, 0, sizeof skipped);
    std::memcpy(self_ + 76, "PipePistonA", 12);
    self_[40] = 7;
    write_word(address(self_) + 192u, phase);
    write_word(address(self_) + 292u, tracking);
    write_u16(address(self_) + 56u, 0x1000u);
    write_single(address(self_) + 16u, 100.0f); write_single(address(self_) + 20u, 200.0f);
    write_single(address(target) + 16u, tx); write_single(address(target) + 20u, ty);
    write_single(address(skipped) + 16u, 100.0f); write_single(address(skipped) + 20u, 200.0f);
    write_word(address(skipped) + 12u, 8u);
    std::memset(player, 0, sizeof player);
    host_note(target); host_note(skipped);
    write_word(address(player) + 21872u, address_bits(skipped));
    write_word(address(player) + 21872u + 23984u, address_bits(target));
    write_word(address(player) + 21872u + 2u * 23984u, 0u);
    w8c::write_word(address(nmPlayers), 3u);
    advance_calls = attack_calls = 0;
}
int main() {
    using namespace w8c;
    setup(1u, 0u, 100.0f + 48.0f - 17.0f, 200.0f);
    cos_answer = 1.0f; sin_answer = 0.0f; heading_answer = 0x1000 + 0x9000;
    msg_run__8AutoProp(self_);
    expect(read_word(address(self_) + 280u) == 1u, "active flag stored");
    expect(read_word(address(self_) + 380u) == 1u, "counter incremented");
    expect(read_word(address(self_) + 192u) == 0u && advance_calls == 0, "no animation when byte 156 is zero");
    expect(read_word(address(self_) + 292u) == 0u, "a zero phase leaves tracking unchanged");
    setup(0u, 1u, 100.0f + 48.0f - 17.0f, 200.0f);
    self_[156] = 1; advance_answer = 1;
    msg_run__8AutoProp(self_);
    expect(advance_calls == 1 && read_word(address(self_) + 192u) == 1u, "animation result stored at 192");
    expect(read_word(address(self_) + 292u) == 1u, "bit 1 sets tracking");
    expect(attack_calls == 1 && attack_victim == target && attack_attacker == self_, "target 17 away is attacked");
    expect(attack_ints[0] == 10000 && attack_ints[1] == 0 && attack_ints[2] == 0 && attack_ints[3] == 65, "attack ints");
    expect(std::fabs(attack_scale - 0.3f) < 1e-6f, "scale is the difficulty times 0.3");
    expect(read_single(address(target) + 756u) == 100.0f, "float 756 is 100");
    expect(read_u16(address(target) + 748u) == ((0x1000u - 16384u) & 0xffffu), "negative arm");
    expect(attack_calls == 1, "tracked player in the skipped slot is not attacked");
    setup(0u, 1u, 100.0f + 48.0f - 18.0f, 200.0f);
    self_[156] = 1; advance_answer = 2; heading_answer = 0x1000 + 0x100;
    msg_run__8AutoProp(self_);
    expect(read_word(address(self_) + 292u) == 0u, "bit 2 clears tracking");
    expect(attack_calls == 0, "cleared tracking ends the call");
    setup(0u, 1u, 100.0f + 48.0f - 18.0f, 200.0f);
    self_[156] = 1; advance_answer = 3;
    msg_run__8AutoProp(self_);
    expect(attack_calls == 0, "distance 18 is not inside");
    setup(0u, 1u, 100.0f + 48.0f - 17.0f, 200.0f);
    self_[156] = 1; advance_answer = 1; heading_answer = 0x1000 + 0x100;
    msg_run__8AutoProp(self_);
    expect(read_u16(address(target) + 748u) == 0x1000u + 16384u, "positive arm");
    active_answer = 0;
    setup(0u, 1u, 100.0f + 48.0f - 17.0f, 200.0f);
    self_[156] = 1; advance_answer = 1;
    msg_run__8AutoProp(self_);
    expect(read_word(address(self_) + 280u) == 0u && advance_calls == 0 && attack_calls == 0, "inactive ends the call");
    active_answer = 1;
    strticmp_answer = 1;
    setup(0u, 1u, 100.0f + 48.0f - 17.0f, 200.0f);
    self_[156] = 1; advance_answer = 1;
    msg_run__8AutoProp(self_);
    expect(read_word(address(self_) + 380u) == 1u && attack_calls == 0, "name mismatch ends the call after the counter");
    return failures ? 1 : 0;
}
