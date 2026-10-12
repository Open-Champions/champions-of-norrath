#define NORRATH_COMBAT_REACTIONS_HOST 1
#include "charm.cpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
static int destroy_calls = 0;
static void* seen_head = nullptr;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" void norrath_list_destroy(void* head) {
    ++destroy_calls;
    seen_head = head;
    combat_reactions::write_word(combat_reactions::address(head), 0x77u);
}
int main() {
    unsigned char object[800];
    std::memset(object, 0x11, sizeof object);
    combat_reactions::write_word(combat_reactions::address(object) + 12u, 0x1fu);
    charm__8Creaturei(object, 0x12345);
    expect(destroy_calls == 0, "clear bit makes no call");
    expect(object[792] == 0x11 && object[793] == 0x11 && object[576] == 0x11, "clear bit writes nothing");

    combat_reactions::write_word(combat_reactions::address(object) + 12u, 0x20u);
    charm__8Creaturei(object, 0x12345);
    expect(destroy_calls == 1 && seen_head == object + 576, "set bit destroys the list at 576");
    expect(object[792] == 0x45 && object[793] == 0x23 && object[794] == 0x11, "low half is stored at 792");
    expect(combat_reactions::read_word(combat_reactions::address(object) + 576u) == 0u, "head word is cleared after the call");
    return failures ? 1 : 0;
}
