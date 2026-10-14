#include <cstdio>
#include <cstring>

extern "C" void msg_alert__8CreatureifG6Point3P10GameObject(
    void* self, int code, float amount, const void* point, void* other);

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

int main() {
    unsigned char creature[8];
    unsigned char other[8];
    unsigned char point[12];
    std::memset(creature, 0x10, sizeof creature);
    std::memset(other, 0x20, sizeof other);
    for (int i = 0; i < 12; ++i) point[i] = static_cast<unsigned char>(0x30 + i);
    msg_alert__8CreatureifG6Point3P10GameObject(creature, 9, 1.5f, point, other);
    bool intact = true;
    for (unsigned char byte : creature) if (byte != 0x10) intact = false;
    for (unsigned char byte : other) if (byte != 0x20) intact = false;
    for (int i = 0; i < 12; ++i) if (point[i] != static_cast<unsigned char>(0x30 + i)) intact = false;
    expect(intact, "arguments stay put");
    return failures ? 1 : 0;
}
