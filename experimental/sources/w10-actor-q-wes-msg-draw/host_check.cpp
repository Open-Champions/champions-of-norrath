// Host check only; not an EE build input.
#define W10_ACTOR_Q_HOST 1
#include "msg_draw.cpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_q_host_base;
static std::string calls; static int failures;
static void* the_object;
extern "C" int draw__8CreatureP12_modelHeaderP8_textureT1(void* s, void* m, void* t, void* z) {
    calls += "draw(" + std::string(s == the_object ? "self" : "?") + "," + std::to_string((long)((unsigned char*)m - w10_actor_q_host_base)) + "," +
             std::to_string((long)((unsigned char*)t - w10_actor_q_host_base)) + "," + (z ? "nz" : "0") + ");";
    return 0;
}
extern "C" void drawArrow__14WoodElfSoldier(void* s) { calls += std::string("arrow(") + (s == the_object ? "self" : "?") + ");"; }
int main() {
    static unsigned char base[4096];
    w10_actor_q_host_base = base;
    unsigned char* object = base + 256; unsigned char* block = base + 1024;
    the_object = object;
    *reinterpret_cast<unsigned*>(object + 36) = 1024;
    *reinterpret_cast<unsigned*>(block + 12) = 3000; *reinterpret_cast<unsigned*>(block + 20) = 3100;
    msg_draw__14WoodElfSoldier(object);
    if (calls != "draw(self,3000,3100,0);arrow(self);") { std::fprintf(stderr, "fail: %s\n", calls.c_str()); ++failures; }
    if (!failures) std::puts("wes msg_draw ok");
    return failures ? 1 : 0;
}
