#include <cstdio>
#include <cstring>

extern "C" void msg_collisionWorld__8CreatureG6Point3(void* self, const void* point);

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

int main() {
    unsigned char creature[8];
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    std::memset(creature, 0x44, sizeof creature);
    msg_collisionWorld__8CreatureG6Point3(creature, point);
    bool intact = true;
    for (unsigned char byte : creature) if (byte != 0x44) intact = false;
    for (int i = 0; i < 12; ++i) if (point[i] != static_cast<unsigned char>(i + 1)) intact = false;
    expect(intact, "creature and point stay put");
    unsigned char zeros[12] = {};
    msg_collisionWorld__8CreatureG6Point3(nullptr, zeros);
    expect(intact, "null creature still leaves the point");
    return failures ? 1 : 0;
}
