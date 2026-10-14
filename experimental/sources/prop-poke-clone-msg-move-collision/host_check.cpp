#include "msg_move_collision.cpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
int main() {
    unsigned char object[32];
    void* list[3] = {object, nullptr, object};
    std::memset(object, 0x3c, sizeof object);
    msg_moveCollision__10GameObjectPP10GameObject(object, list);
    for (unsigned char b : object) expect(b == 0x3c, "object unchanged");
    expect(list[0] == object && list[1] == nullptr && list[2] == object, "list unchanged");
    msg_moveCollision__10GameObjectPP10GameObject(object, nullptr);
    return failures ? 1 : 0;
}
