// Host check only; not a build input. Build: c++ -std=c++17 -O0 -fno-builtin host_check.cpp
// The unit keeps string data pointers as 32-bit words, so the check places its representations in an arena whose
// addresses are mapped through a table: the sed line below redirects the word-to-pointer cast.
//   sed 's/static_cast<__UINTPTR_TYPE__>(word)/norrath_host_long(word)/' dtor.cpp > /tmp/dtor_host.cpp
//   c++ -std=c++17 -O0 -fno-builtin -DUNIT='"/tmp/dtor_host.cpp"' host_check.cpp
#include <cstdio>
#include <cstring>
#include <string>

static unsigned char arena[0x4000];
static const unsigned kBase = 0x200000u;
unsigned long norrath_host_long(unsigned word) { return reinterpret_cast<unsigned long>(arena + (word - kBase)); }

#include UNIT

unsigned char vt_realm_interface[8];
unsigned free_list[40];
static std::string trace;
extern "C" {
void _$_11VIUDPSocket(void* s, int f) { trace += "udp" + std::to_string(f) + ";"; (void)s; }
void _$_Q212RealmService15RealmGatewayAPI(void*, int f) { trace += "base" + std::to_string(f) + ";"; }
void __wrap_free(void* p) { trace += "free(" + std::to_string(static_cast<unsigned char*>(p) - arena) + ");"; }
void __builtin_delete(void*) { trace += "delete;"; }
}

int main() {
    static unsigned char object[6000];
    bool ok = true;
    StringRep* reps = reinterpret_cast<StringRep*>(arena);
    // data pointer of representation k is kBase + 16 * k + 16 (the representation header is 16 bytes)
    // rep 0: capacity 10, count 2, used by slots 5 and 150: reaches zero on the second release -> list index 3
    // rep 1: capacity 200, count 1, used by slot 3: large block -> __wrap_free
    // rep 2: capacity 0, count 1000, used by the other 198 slots: stays alive
    reps[0] = StringRep{3, 10, 2, 0};
    reps[1] = StringRep{3, 200, 1, 0};
    reps[2] = StringRep{3, 0, 1000, 0};
    unsigned* slots = reinterpret_cast<unsigned*>(object + 4284);
    for (int i = 0; i < 200; ++i) slots[i] = kBase + 16 * 2 + 16;
    slots[3] = kBase + 16 * 1 + 16;
    slots[5] = kBase + 16 * 0 + 16;
    slots[150] = kBase + 16 * 0 + 16;
    trace.clear();
    dtor_16VIRealmInterface(object, 1);
    ok = ok && reps[2].references == 1000 - 197 && reps[0].references == 0 && reps[1].references == 0;
    ok = ok && free_list[3] == static_cast<unsigned>(reinterpret_cast<unsigned long>(&reps[0])) && *reinterpret_cast<unsigned*>(&reps[0]) == 0;
    ok = ok && trace == "free(16);udp2;base0;delete;";
    ok = ok && *reinterpret_cast<unsigned*>(object + 8) == static_cast<unsigned>(reinterpret_cast<unsigned long>(vt_realm_interface));
    trace.clear();
    dtor_16VIRealmInterface(object, 2);
    ok = ok && trace.find("delete;") == std::string::npos && trace.find("base0;") != std::string::npos;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
