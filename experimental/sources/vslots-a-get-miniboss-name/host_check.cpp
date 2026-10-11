#include <cstdio>
#include <cstring>

extern "C" const char* getMinibossName__8Creature(void* self);

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

int main() {
    unsigned char object[4] = {1, 2, 3, 4};
    const char* name = getMinibossName__8Creature(object);
    expect(name != nullptr && std::strcmp(name, "Pablo The Great") == 0, "fixed name");
    expect(getMinibossName__8Creature(nullptr) == name, "null creature returns the same bytes");
    expect(object[0] == 1 && object[1] == 2 && object[2] == 3 && object[3] == 4, "creature stays put");
    return failures ? 1 : 0;
}
