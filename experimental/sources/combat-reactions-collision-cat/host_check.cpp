#define NORRATH_COMBAT_REACTIONS_HOST 1
#include "collision_cat.cpp"
#include "../combat-reactions-bytes/host_vcall.hpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
static char log_text[16];
static unsigned log_length = 0;
static int ready_answer = 0;
static void* seen_self = nullptr;
static int seen_state = -1;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static void note(char c) { log_text[log_length++] = c; log_text[log_length] = 0; }
extern "C" int readyToSwitch__3Cat(void*) { note('r'); return ready_answer; }
extern "C" void switchToRun__3Cat(void*) { note('w'); }
extern "C" void set_state_stub(void* self, int state) { note('s'); seen_self = self; seen_state = state; }
static void run(unsigned char* object) {
    log_length = 0; log_text[0] = 0;
    msg_collision__3CatP10GameObject(object, nullptr);
}
int main() {
    unsigned char object[980];
    unsigned char table[256];
    std::memset(object, 0, sizeof object);
    std::memset(table, 0, sizeof table);
    combat_reactions::host_install_slot(object, table, 184u, 16, reinterpret_cast<void*>(set_state_stub));
    ready_answer = 1;

    combat_reactions::write_word(combat_reactions::address(object) + 964u, 1u);
    run(object);
    expect(log_length == 0, "word 1 at 964 returns first");
    combat_reactions::write_word(combat_reactions::address(object) + 964u, 2u);
    combat_reactions::write_u16(combat_reactions::address(object) + 572u, 5u);
    run(object);
    expect(log_length == 0, "state 5 returns");
    combat_reactions::write_u16(combat_reactions::address(object) + 572u, 0xffffu);
    run(object);
    expect(log_length == 0, "state -1 returns");

    combat_reactions::write_u16(combat_reactions::address(object) + 572u, 13u);
    ready_answer = 0;
    run(object);
    expect(std::strcmp(log_text, "r") == 0, "not ready stops after the check");
    ready_answer = 1;
    run(object);
    expect(std::strcmp(log_text, "rws") == 0, "state 13 runs the check, the switch, then the slot");
    expect(seen_self == object + 16 && seen_state == 3, "slot gets the adjusted object and 3");
    combat_reactions::write_u16(combat_reactions::address(object) + 572u, 0u);
    run(object);
    expect(std::strcmp(log_text, "rws") == 0, "state 0 also passes");
    return failures ? 1 : 0;
}
