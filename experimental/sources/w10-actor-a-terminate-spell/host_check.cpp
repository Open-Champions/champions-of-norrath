// Host contract for terminateSpell__8CreatureRC9type_info; not an EE input.
// The source reads 32-bit pointer words, so it cannot run on a 64-bit host. This file restates its
// control flow over host-sized objects and checks the contract: the spell finder is called once,
// a null result calls nothing, and a found spell has its table entry (table + 8) called with
// the object adjusted by the entry's signed halfword and the flag value 3.
// The authored object itself is compared against the original in a private R5900 run.
#include <cstdio>
#include <string>
struct Entry { short adjust; short pad; void (*fn)(void*, int); };
struct Spell { char pad[8]; const Entry* table; };
static void* found;
static std::string calls;
static void* g_self;
static int g_flags;
static void* finder(void*, const void*) { calls += "check;"; return found; }
static void dtor(void* self, int flags) { g_self = self; g_flags = flags; calls += "dtor;"; }
static void terminate_model(void* creature, const void* ti) {
    Spell* s = static_cast<Spell*>(finder(creature, ti));
    if (s) s->table->fn(reinterpret_cast<char*>(s) + s->table->adjust, 3);
}
int main() {
    int bad = 0;
    const Entry e = {-16, 0, dtor};
    Spell s = {{}, &e};
    found = nullptr; calls.clear();
    terminate_model(nullptr, nullptr);
    if (calls != "check;") { std::puts("fail: null spell must not call the entry"); bad = 1; }
    found = &s; calls.clear();
    terminate_model(nullptr, nullptr);
    if (calls != "check;dtor;" || g_flags != 3 || g_self != reinterpret_cast<char*>(&s) - 16) { std::puts("fail: entry call"); bad = 1; }
    return bad;
}
