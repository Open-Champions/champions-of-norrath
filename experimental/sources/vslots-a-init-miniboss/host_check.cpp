#include <cstdio>
#include <cstring>

extern "C" int initMiniboss__8Creature(void* self);

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

int main() {
    unsigned char object[80];
    std::memset(object, 0x3c, sizeof object);
    expect(initMiniboss__8Creature(object) == 1, "returns 1");
    expect(object[76] == 1, "byte 76 becomes 1");
    bool others = true;
    for (int i = 0; i < 80; ++i) if (i != 76 && object[i] != 0x3c) others = false;
    expect(others, "only byte 76 changes");
    object[76] = 9;
    expect(initMiniboss__8Creature(object) == 1, "second call still returns 1");
    expect(object[76] == 1, "byte 76 is overwritten with 1");
    return failures ? 1 : 0;
}
