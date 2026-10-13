// Host contract for msg_poke__11PushTriggeri; not an EE input. Tag lookups, the action runner
// and the table entry are stubs that log. Every combination of the three tag results is run.
#define W10_ACTOR_K_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_k_host_base;
static std::string calls; static bool has_action; static int off_only, once;
extern "C" char* findTagString__10GameObjectPci(void*, const char* name, int n) {
    calls += std::string("str(") + name + "," + std::to_string(n) + ");";
    static char text[] = "x"; return has_action ? text : nullptr;
}
extern "C" int findTagInt__10GameObjectPci(void*, const char* name, int n) {
    calls += std::string("int(") + name + "," + std::to_string(n) + ");";
    return name[0] == 'o' && name[1] == 'f' ? off_only : once;
}
extern "C" void objectDoMyActions__FP10GameObjecti(void*, int f) { calls += "do" + std::to_string(f) + ";"; }
unsigned w10_actor_k_host_call(unsigned fn, void* adjusted, int n, unsigned long a1, unsigned long) {
    calls += "entry(" + std::to_string(fn) + "," + std::to_string(static_cast<unsigned char*>(adjusted) - w10_actor_k_host_base) + "," + std::to_string(n) + "," + std::to_string(a1) + ");";
    return 0;
}
#include "msg_poke.cpp"
alignas(16) static unsigned char arena[0x1000];
int main() {
    w10_actor_k_host_base = arena;
    int bad = 0;
    unsigned char* self = arena + 0x100;
    const unsigned table = 0x800; const unsigned fnword = 0x4242; const short adj = -24;
    std::memset(arena, 0, sizeof arena);
    std::memcpy(self + 72, &table, 4); std::memcpy(arena + table + 8, &adj, 2); std::memcpy(arena + table + 12, &fnword, 4);
    for (int a = 0; a < 2; ++a) for (int off : {0, 5}) for (int on : {0, 3}) {
        calls.clear(); has_action = a; off_only = off; once = on;
        msg_poke__11PushTriggeri(self, 99);
        std::string want = "str(action,0);";
        if (a) {
            want += "int(offOnly,0);do" + std::string(off == 0 ? "1" : "0") + ";int(once,0);";
            if (on) want += "entry(16962,232,1,3);";
        }
        if (calls != want) { std::printf("fail: %s\n", calls.c_str()); bad = 1; }
    }
    return bad;
}
