#include <cstdio>
#include <cstring>
#include <vector>

extern "C" void confuse__8Creaturei(void* self, int value);

static std::vector<void*> destroyed;
static int failures = 0;
static unsigned char* live_head = nullptr;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

extern "C" void norrath_list_destroy(void* head) {
    destroyed.push_back(head);
    if (live_head != nullptr) {
        live_head[0] = 0x11;
        live_head[1] = 0x22;
        live_head[2] = 0x33;
        live_head[3] = 0x44;
    }
}

int main() {
    unsigned char creature[800];
    std::memset(creature, 0x5a, sizeof creature);
    live_head = creature + 576;
    confuse__8Creaturei(creature, 0x1abcd);
    expect(destroyed.size() == 1 && destroyed[0] == live_head, "destroys the list at +576");
    expect(creature[796] == 0xcd && creature[797] == 0xab, "low half of the argument");
    expect(creature[576] == 0 && creature[577] == 0 && creature[578] == 0 && creature[579] == 0,
           "list head is zero after destroy");
    bool others = true;
    for (int i = 0; i < 800; ++i) {
        if (i >= 576 && i < 580) continue;
        if (i == 796 || i == 797) continue;
        if (creature[i] != 0x5a) others = false;
    }
    expect(others, "only the halfword and list head change");
    return failures ? 1 : 0;
}
