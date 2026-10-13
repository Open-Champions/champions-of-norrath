// Host contract for deleteCommander__3Orc; not an EE input. Words that hold addresses are offsets from a host
// base; listDestroy logs the index of the head it is given. Returns non-zero when wrong.
#define W11_ACTOR_A_HOST 1
#include <cstring>
#include <string>
unsigned char* w11_actor_a_host_base;
extern "C" {
unsigned Orc_commanderList[20] __asm__("_3Orc$m_commanderList");
}
static std::string calls;
extern "C" void listDestroy__FP8ListHead(void* head) {
    calls += std::to_string(static_cast<unsigned*>(head) - Orc_commanderList) + ";";
}
#include "delete_commander.cpp"
int main() {
    alignas(16) static unsigned char arena[0x4000];
    w11_actor_a_host_base = arena;
    int bad = 0;
    unsigned char* self = arena + 0x100;
    unsigned selfw = 0x100, otherw = 0x200;
    // nodes at 0x1000 + 0x40*i; node i holds owner in word +12
    auto set = [&](int i, unsigned owner) { std::memcpy(arena + 0x1000 + 0x40 * i + 12, &owner, 4); };
    auto run = [&](unsigned mask_zero, unsigned mask_self) {
        std::memset(Orc_commanderList, 0, sizeof Orc_commanderList);
        calls.clear();
        std::string want;
        for (int i = 0; i < 20; ++i) {
            if (mask_zero >> i & 1) continue;
            Orc_commanderList[i] = 0x1000 + 0x40 * i;
            set(i, mask_self >> i & 1 ? selfw : otherw);
            if (mask_self >> i & 1) want += std::to_string(i) + ";";
        }
        deleteCommander__3Orc(self);
        return calls == want;
    };
    if (!run(0xfffff, 0)) bad = 1;              // all empty
    if (!run(0, 0)) bad = 1;                    // none belongs to self
    if (!run(0, 0xfffff)) bad = 1;              // all belong to self
    if (!run(0x0aaaa, 0x55555)) bad = 1;
    if (!run(0x00001, 0x80000)) bad = 1;
    if (!run(0x80000, 0x00001)) bad = 1;
    if (!run(0x00008, 0x00008)) bad = 1;        // an empty head is skipped even when marked
    return bad;
}
