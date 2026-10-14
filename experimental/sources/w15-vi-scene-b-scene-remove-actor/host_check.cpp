// Host check only; not a build input. Erase is a recorder with a settable result.
#define NORRATH_HOST_ARENA 1
#include "remove_actor.cpp"
#include <cstdint>
static unsigned char arena[0x4000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
extern "C" void* norrath_host_function(unsigned) { return nullptr; }
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }
static int erase_calls, erase_fail_at = -1; static unsigned char* erased[8]; static unsigned char* erase_list;
extern "C" int Erase__t6VIList1ZQ27VIScene12VISceneOccupi(unsigned char* list, unsigned char* node) {
    erase_list = list; erased[erase_calls] = node;
    return erase_calls++ == erase_fail_at ? -5 : 0;
}
static unsigned& W(unsigned char* base, int off) { return *reinterpret_cast<unsigned*>(base + off); }
int main() {
    unsigned char* scene = arena;
    unsigned char* element = arena + 0x1000;      // base + 592*1
    unsigned char* base = element - 592;
    unsigned char* cells = arena + 0x1400;        // cell J=2 at +240
    unsigned char* cell = cells + 240;
    unsigned char* actor = arena + 0x2000;
    unsigned char* n[3] = {arena + 0x2400, arena + 0x2440, arena + 0x2480};
    const unsigned none = 0xffffffffu;
    auto build = [&]() {
        W(scene, 80) = word_of(base);
        W(element, 96) = word_of(cells);
        for (int k = 0; k < 3; ++k) {
            *reinterpret_cast<short*>(n[k]) = 1; *reinterpret_cast<short*>(n[k] + 2) = 2;
            W(n[k], 8) = k < 2 ? word_of(n[k + 1]) : none;       // actor chain
        }
        // cell chain order: n0 <-> n1 <-> n2
        W(n[0], 16) = none; W(n[0], 20) = word_of(n[1]);
        W(n[1], 16) = word_of(n[0]); W(n[1], 20) = word_of(n[2]);
        W(n[2], 16) = word_of(n[1]); W(n[2], 20) = none;
        W(cell, 108) = word_of(n[0]); W(cell, 104) = word_of(n[2]);
        W(actor, 152) = word_of(n[0]); W(actor, 156) = 0x77;
        erase_calls = 0;
    };
    build();
    if (RemoveActor__7VIScenei(scene, word_of(actor)) != 0) return 1;
    if (erase_calls != 3 || erased[0] != n[0] || erased[1] != n[1] || erased[2] != n[2] || erase_list != scene + 984) return 2;
    if (W(actor, 152) != none || W(actor, 156) != none) return 3;
    if (W(cell, 108) != none || W(cell, 104) != none) return 4;
    // a failing erase stops the walk and leaves the actor head alone
    build(); erase_fail_at = 1;
    if (RemoveActor__7VIScenei(scene, word_of(actor)) != -1 || erase_calls != 2) return 5;
    if (W(actor, 152) != word_of(n[0]) || W(actor, 156) != 0x77) return 6;
    // empty chain
    erase_fail_at = -1; build(); W(actor, 152) = none;
    if (RemoveActor__7VIScenei(scene, word_of(actor)) != 0 || erase_calls != 0 || W(actor, 156) != none) return 7;
    return 0;
}
