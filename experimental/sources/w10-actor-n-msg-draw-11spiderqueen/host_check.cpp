// Host contract for msg_draw__11SpiderQueen; not an EE input. 32-bit words hold offsets from
// the host base (see access.hpp); the callees record their arguments as offsets.
#define W10_ACTOR_N_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_n_host_base;
#include "msg_draw.cpp"

static unsigned char arena[256] __attribute__((aligned(16)));
static std::string calls;
static std::string off(const void* p) { return p ? std::to_string(static_cast<const unsigned char*>(p) - arena) : "null"; }
extern "C" void modelSetShadowScale__Ff(float s) { calls += "shadow" + std::to_string(static_cast<int>(s)) + ";"; }
extern "C" void draw__8CreatureP12_modelHeaderP8_textureT1(void* s, void* m, void* t, void* t2) {
    calls += "draw(" + off(s) + "," + off(m) + "," + off(t) + "," + off(t2) + ");";
}
extern "C" void modelResetOptionalParams__Fv() { calls += "reset;"; }
int main() {
    w10_actor_n_host_base = arena;
    unsigned record = 100, a = 150, b = 170;
    std::memcpy(arena + 36, &record, 4);
    std::memcpy(arena + record + 12, &a, 4);
    std::memcpy(arena + record + 20, &b, 4);
    msg_draw__11SpiderQueen(arena);
    if (calls != "shadow4;draw(0,150,170,null);reset;") { std::puts(calls.c_str()); return 1; }
    return 0;
}
