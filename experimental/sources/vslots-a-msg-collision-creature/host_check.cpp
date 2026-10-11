#include <cstdio>
#include <cstring>

extern "C" void msg_collision__8CreatureP10GameObject(void* self, void* other);

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

int main() {
    unsigned char self[16];
    unsigned char other[16];
    std::memset(self, 0x11, sizeof self);
    std::memset(other, 0x22, sizeof other);
    msg_collision__8CreatureP10GameObject(self, other);
    msg_collision__8CreatureP10GameObject(nullptr, nullptr);
    bool intact = true;
    for (unsigned char byte : self) if (byte != 0x11) intact = false;
    for (unsigned char byte : other) if (byte != 0x22) intact = false;
    expect(intact, "both objects stay put");
    return failures ? 1 : 0;
}
