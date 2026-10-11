#include <cstdio>
#include <cstring>
extern "C" void norrath_vt_msg_collision_world_10GameObject(void* self, const void* point);
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
int main() {
    unsigned char object[32];
    unsigned char* point = new unsigned char[12];
    std::memset(object, 0x3c, sizeof object);
    for (int i = 0; i < 12; ++i) point[i] = static_cast<unsigned char>(0x80 + i);
    norrath_vt_msg_collision_world_10GameObject(object, point); norrath_vt_msg_collision_world_10GameObject(nullptr, point);
    bool intact = true;
    for (unsigned char b : object) if (b != 0x3c) intact = false;
    for (int i = 0; i < 12; ++i) if (point[i] != static_cast<unsigned char>(0x80 + i)) intact = false;
    expect(intact, "object and point stay put");
    delete[] point;
    return failures ? 1 : 0;
}
