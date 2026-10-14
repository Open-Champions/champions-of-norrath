#include "msg_move_collision.cpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
int main() {
    unsigned char object[32];
    unsigned words[4] = {0u, 0x11111111u, 0x22222222u, 0x33333333u};
    std::memset(object, 0x4b, sizeof object);
    msg_moveCollision__8CreaturePP10GameObject(object, words);
    expect(object[0] == 0x4b && object[31] == 0x4b, "null list leaves the object");
    expect(words[1] == 0x11111111u, "null list does not read further");
    words[0] = 1u;
    words[1] = 2u;
    words[2] = 0u;
    words[3] = 0xabcdefu;
    msg_moveCollision__8CreaturePP10GameObject(object, words);
    expect(words[0] == 1u && words[1] == 2u && words[2] == 0u && words[3] == 0xabcdefu, "walk stores nothing");
    expect(object[7] == 0x4b, "walk leaves the object");
    return failures ? 1 : 0;
}
