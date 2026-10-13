// Host contract for the JumpGate destructor; not an EE input. Includes the source.
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

unsigned char vt_JumpGate[8];
alignas(16) static unsigned char object[1100];
static std::string order;
static std::vector<unsigned> wordSeen;
static int frees, failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s (order %s)\n", what, order.c_str()); ++failures; } }
void light_deactivate(void* a) { order += "light_deactivate@" + std::to_string(static_cast<unsigned char*>(a) - object); }
void fx_creature_state_destructor(void* a, int f) { order += "fx_creature_state_destructor@" + std::to_string(static_cast<unsigned char*>(a) - object) + "/" + std::to_string(f) + ";"; wordSeen.push_back(*reinterpret_cast<unsigned*>(object + 72)); }
void light_destructor(void* a, int f) { order += "light_destructor@" + std::to_string(static_cast<unsigned char*>(a) - object) + "/" + std::to_string(f) + ";"; wordSeen.push_back(*reinterpret_cast<unsigned*>(object + 72)); }
void animation_state_destructor(void* a, int f) { order += "animation_state_destructor@" + std::to_string(static_cast<unsigned char*>(a) - object) + "/" + std::to_string(f) + ";"; wordSeen.push_back(*reinterpret_cast<unsigned*>(object + 72)); }
void gameobject_destructor(void* a, int f) { order += "gameobject_destructor@" + std::to_string(static_cast<unsigned char*>(a) - object) + "/" + std::to_string(f) + ";"; wordSeen.push_back(*reinterpret_cast<unsigned*>(object + 72)); }
void blockFree__FPv(void* a) { ++frees; order += "F"; expect(a == object, "frees the same object"); }
int main() {
    std::memset(object, 0x3c, sizeof object);
    const unsigned wanted = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_JumpGate));
    jumpgate_destructor(object, 0);
    expect(order == "light_deactivate@256fx_creature_state_destructor@316/2;light_destructor@256/2;animation_state_destructor@216/2;gameobject_destructor@0/0;" && frees == 0, "flag 0: parts in order, no free");
    for (unsigned w : wordSeen) expect(w == wanted, "table pointer stored before any part destructor");
    expect(!wordSeen.empty(), "part destructors ran");
    order.clear();
    jumpgate_destructor(object, 2);
    expect(order == "light_deactivate@256fx_creature_state_destructor@316/2;light_destructor@256/2;animation_state_destructor@216/2;gameobject_destructor@0/0;" && frees == 0, "flag 2 does not free");
    order.clear();
    jumpgate_destructor(object, 1);
    expect(order == "light_deactivate@256fx_creature_state_destructor@316/2;light_destructor@256/2;animation_state_destructor@216/2;gameobject_destructor@0/0;F" && frees == 1, "flag 1 frees last");
    order.clear();
    jumpgate_destructor(object, 5);
    expect(order == "light_deactivate@256fx_creature_state_destructor@316/2;light_destructor@256/2;animation_state_destructor@216/2;gameobject_destructor@0/0;F" && frees == 2, "any odd flag frees; part flags stay constant");
    if (!failures) std::puts("JumpGate dtor ok");
    return failures ? 1 : 0;
}
