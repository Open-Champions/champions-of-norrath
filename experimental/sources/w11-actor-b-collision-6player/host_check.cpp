// Host contract for msg_collision__6Player; not an EE input. Includes the source.
#include "msg_collision.cpp"
#include <cstring>
#include <initializer_list>
static int tfCalls, checkCalls, hitCalls;
static void* hitSpell; static void* hitOther;
static int typeToken;
static int spellToken;
extern "C" const void* __tf11SpellCharge() { ++tfCalls; return &typeToken; }
extern "C" void* checkIfSpellIsRunning__8CreatureRC9type_info(void*, const void* t) {
    ++checkCalls;
    return t == &typeToken ? &spellToken : nullptr;
}
extern "C" void hitSomething__11SpellChargeP10GameObject(void* spell, void* other) { ++hitCalls; hitSpell = spell; hitOther = other; }
int main() {
    alignas(8) unsigned char self[1500], other[200];
    for (unsigned sf : {0u, 0x20000u, 0x1u, 0xfffdffffu, 0xffffffffu})
    for (int state : {0, 8, 9, 10})
    for (unsigned of : {0x20u, 0x220u, 0x1000020u, 0x28u, 0x0u, 0x1000220u, 0x40u, 0xffffffffu, 0x20u | 0x1u})
    for (float ds : {0.0f, 59.9f, 60.0f, -59.9f, -60.0f, 1000.0f}) {
        std::memset(self, 0, sizeof self); std::memset(other, 0, sizeof other);
        std::memcpy(self + 12, &sf, 4); std::memcpy(self + 1400, &state, 4);
        float base = 5.0f; std::memcpy(self + 24, &base, 4);
        float ov = base + ds; std::memcpy(other + 12, &of, 4); std::memcpy(other + 24, &ov, 4);
        tfCalls = checkCalls = hitCalls = 0; hitSpell = hitOther = nullptr;
        msg_collision__6PlayerP10GameObject(self, other);
        bool acts = !(sf & 0x20000u) && state == 9;
        if (!acts) { if (tfCalls || checkCalls || hitCalls) return 1; continue; }
        if (tfCalls != 1 || checkCalls != 1 || hitCalls != 1 || hitSpell != &spellToken) return 2;
        bool near = (ov - base) < 60.0f && (ov - base) > -60.0f;
        bool want = (of & 0x1000220u) == 0x20u && near;
        if (hitOther != (want ? static_cast<void*>(other) : nullptr)) return 3;
    }
    return 0;
}
