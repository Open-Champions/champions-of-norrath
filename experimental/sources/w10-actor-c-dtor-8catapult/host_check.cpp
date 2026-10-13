// Host check only. Build with -DNORRATH_W10AC_HOST (32-bit address words are offsets from
// norrath_w10ac_host_base).
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" void dtor_8Catapult(void* object, int flags) asm("_$_8Catapult");
extern "C" unsigned char norrath_ctors_ant_lantern_view_4506b0[8] = {};
extern "C" signed char cutMode = 0;
extern "C" unsigned char* norrath_w10ac_host_base;
unsigned char* norrath_w10ac_host_base;
extern "C" void animation_state_destructor(void* state, int flags) asm("_$_14AnimationState");

static int failures = 0;
static unsigned char* memory;
static unsigned char* current;
static std::vector<std::string> events;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static void put(unsigned off, unsigned v) { std::memcpy(memory + off, &v, 4); }
static unsigned get(unsigned off) { unsigned v; std::memcpy(&v, memory + off, 4); return v; }
static std::string where(void* p) {
    return std::to_string(static_cast<unsigned char*>(p) - memory);
}
// Host functions behind the vtable function words.
static void slot_a(void* self, int arg) { events.push_back("slotA:" + where(self) + ":" + std::to_string(arg)); }
static void slot_b(void* self, int arg) { events.push_back("slotB:" + where(self) + ":" + std::to_string(arg)); }
extern "C" void* norrath_w10ac_host_function(unsigned word) {
    return word == 0x11 ? reinterpret_cast<void*>(&slot_a) : reinterpret_cast<void*>(&slot_b);
}
extern "C" void listDestroy__FP8ListHead(void* head) { events.push_back("list:" + where(head)); }
extern "C" void animation_state_destructor(void* state, int flags) {
    events.push_back("anim:" + where(state) + ":" + std::to_string(flags));
}
extern "C" void norrath_vt_dtor_10GameObject(void* object, int flags) {
    events.push_back("base:" + where(object) + ":" + std::to_string(flags));
}
extern "C" void norrath_block_free(void* object) { events.push_back("free:" + where(object)); }

int main() {
    static unsigned char block[0x2000];
    memory = block;
    norrath_w10ac_host_base = block;
    const unsigned O = 0x100;       // the catapult
    // Two target objects with vtables at 0x800 / 0x900; entry at +8 holds {adjust, function}.
    const unsigned T1 = 0x400, T2 = 0x500, T3 = 0x600;
    const unsigned N1 = 0xa00, N2 = 0xa40, N3 = 0xa80;
    for (int variant = 0; variant < 8; ++variant)
        for (int flags : {0, 1, 2, 3, 6, -1}) {
            std::memset(block, 0x3c, sizeof block);
            current = block + O;
            events.clear();
            bool cut = (variant & 1) != 0;
            bool partner = (variant & 2) != 0;
            bool empty_middle = (variant & 4) != 0;
            cutMode = cut ? -3 : 0;
            // vtables: entry at vt+8
            put(T1 + 72, 0x800); put(T2 + 72, 0x900); put(T3 + 72, 0x800);
            unsigned short adj1 = 0x10, adj2 = static_cast<unsigned short>(-8);
            std::memcpy(block + 0x808, &adj1, 2); put(0x80c, 0x11);
            std::memcpy(block + 0x908, &adj2, 2); put(0x90c, 0x22);
            put(O + 248, N1);
            put(N1, N2); put(N1 + 12, T1);
            put(N2, N3); put(N2 + 12, empty_middle ? 0u : T2);
            put(N3, 0); put(N3 + 12, T3);
            put(O + 88, partner ? T2 : 0u);
            unsigned char before_object[0x100];
            std::memcpy(before_object, block + O, sizeof before_object);
            dtor_8Catapult(block + O, flags);
            std::vector<std::string> want;
            want.push_back("slotA:" + std::to_string(T1 + 0x10) + ":3");
            if (!empty_middle) want.push_back("slotB:" + std::to_string(T2 - 8) + ":3");
            want.push_back("slotA:" + std::to_string(T3 + 0x10) + ":3");
            if (partner && cut) want.push_back("slotB:" + std::to_string(T2 - 8) + ":3");
            want.push_back("list:" + std::to_string(O + 248));
            want.push_back("anim:" + std::to_string(O + 92) + ":0");
            want.push_back("base:" + std::to_string(O) + ":0");
            if (flags & 1) want.push_back("free:" + std::to_string(O));
            expect(events == want, "call sequence");
            if (events != want) {
                for (auto& e : events) std::fprintf(stderr, "  got %s\n", e.c_str());
                for (auto& e : want) std::fprintf(stderr, "  want %s\n", e.c_str());
            }
            expect(get(N1 + 12) == 0 && get(N3 + 12) == 0 && get(N2 + 12) == 0, "node target words cleared");
            expect(get(N1) == N2 && get(N2) == N3, "links kept");
            std::memcpy(before_object + 72, block + O + 72, 4);
            expect(std::memcmp(before_object, block + O, sizeof before_object) == 0, "object only changes at +72");
            unsigned vt;
            std::memcpy(&vt, block + O + 72, 4);
            expect(vt == static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_ctors_ant_lantern_view_4506b0)),
                   "vtable word stored");
        }
    return failures ? 1 : 0;
}
