// Host contract for msg_load__3OrcPc; not an EE input. Creature load and initType are stubs
// that log; initType sets the state halfword to a chosen value so both branches are exercised.
#include <cstring>
#include <initializer_list>
#include <string>
static std::string calls; static int count_to_return; static short state_after_init;
static unsigned char* the_object;
extern "C" int msg_load__8CreaturePc(void*, char*) { calls += "load;"; return count_to_return; }
extern "C" void initType__3Orcb(void* self, int flag) {
    int w1116, w972, w1112, w208;
    std::memcpy(&w1116, the_object + 1116, 4); std::memcpy(&w972, the_object + 972, 4);
    std::memcpy(&w1112, the_object + 1112, 4); std::memcpy(&w208, the_object + 208, 4);
    calls += "init" + std::to_string(flag) + ":" + std::to_string(w1116) + "," + std::to_string(w972) + "," + std::to_string(w1112) + "," + std::to_string(w208) + ";";
    std::memcpy(the_object + 572, &state_after_init, 2);
    (void)self;
}
#include "msg_load.cpp"
int main() {
    alignas(16) static unsigned char o[2048];
    alignas(16) static unsigned char buf[128];
    int bad = 0;
    for (int n : {0, 12, 40}) for (short st : {(short)10, (short)9, (short)0}) {
        std::memset(o, 0xcc, sizeof o); the_object = o; calls.clear();
        const int w[4] = {11, -22, 33, -44};
        std::memset(buf, 0, sizeof buf); std::memcpy(buf + n, w, 16);
        count_to_return = n; state_after_init = st;
        const int r = msg_load__3OrcPc(o, reinterpret_cast<char*>(buf));
        int g[4]; std::memcpy(&g[0], o + 1116, 4); std::memcpy(&g[1], o + 972, 4); std::memcpy(&g[2], o + 1112, 4); std::memcpy(&g[3], o + 208, 4);
        int w1124; std::memcpy(&w1124, o + 1124, 4);
        if (r != n + 16 || std::memcmp(g, w, 16) != 0) bad = 1;
        if (calls != "load;init1:11,-22,33,-44;") bad = 1;
        if (w1124 != (st == 10 ? 0 : static_cast<int>(0xcccccccc))) bad = 1;
    }
    return bad;
}
