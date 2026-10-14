// Host contract for CreateSpellEffect__19VISpellEffectSystemi; not an EE input. Stubs record the calls in order. The authored
// function was also compared with the original in a private R5900 run.
#include "create_spell_effect.cpp"
#include <cstring>
#include <string>
static std::string trace;
static unsigned char block[48];
static bool fail_allocation, fail_create;
static void* pooled;
static void* pool_seen;
extern "C" {
void* __builtin_new(unsigned size) { trace += "N" + std::to_string(size) + ";"; return fail_allocation ? nullptr : block; }
void __13VISpellEffect(void*) { trace += "C;"; }
int Create__13VISpellEffectP7VIScene(void*, void* scene) { trace += scene ? "S;" : "s;"; return fail_create ? -5 : 0; }
void _$_13VISpellEffect(void*, int flags) { trace += "X" + std::to_string(flags) + ";"; }
int Add__t6VIPool1ZP13VISpellEffectRCP13VISpellEffect(void* pool, void* const* item) {
    trace += "A;"; pool_seen = pool; pooled = *item; return 77;
}
}
int main() {
    alignas(16) static unsigned char system[1200];
    int scene;
    void* const scene_pointer = &scene;
    std::memcpy(system, &scene_pointer, sizeof scene_pointer);
    fail_allocation = true;
    if (CreateSpellEffect__19VISpellEffectSystemi(system, 1) != -1 || trace != "N48;") return 1;
    fail_allocation = false; trace.clear(); fail_create = true;
    if (CreateSpellEffect__19VISpellEffectSystemi(system, 1) != -1 || trace != "N48;C;S;X3;") return 2;
    fail_create = false; trace.clear(); std::memset(block, 0, sizeof block);
    if (CreateSpellEffect__19VISpellEffectSystemi(system, 0) != 77 || trace != "N48;C;S;A;" || pooled != block || pool_seen != system + 1112) return 3;
    int count;
    std::memcpy(&count, block + 4, 4);
    if (count != 0) return 4;
    trace.clear();
    if (CreateSpellEffect__19VISpellEffectSystemi(system, 5) != 77) return 5;
    std::memcpy(&count, block + 4, 4);
    return count == 1 ? 0 : 6;
}
