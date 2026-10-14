#include <cstdio>
#include <cstring>
extern "C" void norrath_vt_msg_collision_10GameObject(void* self, void* other);
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
int main() {
    unsigned char object[32];
    std::memset(object, 0x3c, sizeof object);
    void* slot = object;
    norrath_vt_msg_collision_10GameObject(object, object);
    bool intact = true;
    for (unsigned char b : object) if (b != 0x3c) intact = false;
    expect(intact, "object bytes stay put");
    norrath_vt_msg_collision_10GameObject(nullptr, nullptr);
    expect(intact, "null self returns");
    return failures ? 1 : 0;
}
