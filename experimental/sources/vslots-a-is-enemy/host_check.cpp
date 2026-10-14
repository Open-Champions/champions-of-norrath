#include <cstdio>
#include <cstring>

extern "C" int isEnemy__8CreatureP10GameObject(void* self, void* other);

static int failures = 0;
static void expect(int actual, int wanted, const char* message) {
    if (actual != wanted) {
        std::fprintf(stderr, "fail: %s (got %d wanted %d)\n", message, actual, wanted);
        ++failures;
    }
}
static void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
static void write_half(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
}

int main() {
    alignas(16) unsigned char self[800];
    alignas(16) unsigned char other[800];
    auto reset = [&](unsigned flags, unsigned own, unsigned theirs) {
        std::memset(self, 0, sizeof self);
        std::memset(other, 0, sizeof other);
        write_word(other + 12, flags);
        write_half(self + 792, own);
        write_half(other + 792, theirs);
    };
    expect(isEnemy__8CreatureP10GameObject(self, nullptr), 0, "null");
    reset(0x20, 0, 1);
    expect(isEnemy__8CreatureP10GameObject(self, self), 0, "same object");
    reset(0x10, 1, 1);
    expect(isEnemy__8CreatureP10GameObject(self, other), 0, "neither 0x20 nor 0x40");
    reset(0x28, 1, 1);
    expect(isEnemy__8CreatureP10GameObject(self, other), 0, "bit 0x8");
    reset(0x40, 1, 0);
    expect(isEnemy__8CreatureP10GameObject(self, other), 0, "own half and bit 0x40");
    reset(0x20, 1, 5);
    expect(isEnemy__8CreatureP10GameObject(self, other), 0, "both halves with bit 0x20");
    reset(0x20, 1, 0);
    expect(isEnemy__8CreatureP10GameObject(self, other), 1, "own half only");
    reset(0x20, 1, 0);
    write_word(other + 160, 0x04000000u);
    expect(isEnemy__8CreatureP10GameObject(self, other), 0, "bit 26 of the doubleword");
    reset(0x20, 1, 0);
    write_word(other + 160, 0x800u);
    expect(isEnemy__8CreatureP10GameObject(self, other), 0, "bit 11 of the doubleword");
    reset(0x40, 0, 9);
    expect(isEnemy__8CreatureP10GameObject(self, other), 2, "bit 0x40 and no own half");
    reset(0x60, 0, 9);
    expect(isEnemy__8CreatureP10GameObject(self, other), 2, "both flag bits");
    reset(0x20, 0, 0);
    expect(isEnemy__8CreatureP10GameObject(self, other), 0, "no halves and only bit 0x20");
    reset(0x20, 0, 4);
    expect(isEnemy__8CreatureP10GameObject(self, other), 1, "other half and bit 0x20");
    reset(0x60, 0, 4);
    write_word(other + 160, 0x04000000u);
    expect(isEnemy__8CreatureP10GameObject(self, other), 0, "bit 26 blocks the 0x40 result");
    return failures ? 1 : 0;
}
