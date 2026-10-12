#define NORRATH_COMBAT_REACTIONS_HOST 1
#include "collision_spider.cpp"
#include "../combat-reactions-bytes/host_vcall.hpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
static char log_text[16];
static unsigned log_length = 0;
static void* seen_self = nullptr;
static int seen_state = -1;
static void* base_self = nullptr;
static void* base_other = nullptr;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static void note(char c) { log_text[log_length++] = c; log_text[log_length] = 0; }
extern "C" void set_state_stub(void* self, int state) { note('s'); seen_self = self; seen_state = state; }
extern "C" void msg_collision__8CreatureP10GameObject(void* self, void* other) { note('c'); base_self = self; base_other = other; }
static void run(unsigned char* object, unsigned char* other) {
    log_length = 0; log_text[0] = 0; base_self = base_other = nullptr;
    msg_collision__11SmallSpiderP10GameObject(object, other);
}
int main() {
    unsigned char object[600];
    unsigned char other[100];
    unsigned char table[256];
    std::memset(object, 0, sizeof object);
    std::memset(other, 0, sizeof other);
    std::memset(table, 0, sizeof table);
    combat_reactions::host_install_slot(object, table, 184u, -8, reinterpret_cast<void*>(set_state_stub));
    combat_reactions::write_word(combat_reactions::address(other) + 12u, 0x40u);

    run(object, nullptr);
    expect(std::strcmp(log_text, "c") == 0 && base_self == object && base_other == nullptr, "null other goes straight to the base");
    run(object, other);
    expect(std::strcmp(log_text, "sc") == 0, "state 0 with bit 0x40 sets state first");
    expect(seen_self == object - 8 && seen_state == 10, "slot gets the adjusted object and 10");
    expect(base_self == object && base_other == other, "base gets the original arguments");
    combat_reactions::write_u16(combat_reactions::address(object) + 572u, 10u);
    run(object, other);
    expect(std::strcmp(log_text, "c") == 0, "state 10 skips the slot");
    combat_reactions::write_u16(combat_reactions::address(object) + 572u, 4u);
    combat_reactions::write_word(combat_reactions::address(other) + 12u, 0xffffffbfu);
    run(object, other);
    expect(std::strcmp(log_text, "c") == 0, "missing bit 0x40 skips the slot");
    return failures ? 1 : 0;
}
