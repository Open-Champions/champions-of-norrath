// Host contract for jumpGateInit; not an EE input.
#include "jump_gate_init.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
signed char jumpGateWho, jumpHidden;
char* jumpGateHurtDest;
unsigned char jumpGateState = 9;
int jumping = 5;
static std::vector<std::string> events;
void jumpGateTaskRun__Fv() {}
void jumpGateTaskDraw__Fv() {}
void engineAddTask__FPFv_viPci(void (*cb)(), int p, char* n, int f) {
    events.push_back(std::string("task:") + (cb == jumpGateTaskRun__Fv ? "run" : cb == jumpGateTaskDraw__Fv ? "draw" : "?") + ":" + std::to_string(p) + ":" + n + ":" + std::to_string(f));
    if (jumpGateState != 0 || jumping != 0) events.push_back("state-not-reset");
}
void gameNetKick__Fi(int v) { events.push_back("kick:" + std::to_string(v)); }
static char resource;
void* lumpFindResource__FPcT0(char* l, char* n) { events.push_back(std::string("find:") + l + ":" + n); return &resource; }
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) {
    events.push_back(std::string("play:") + (h == &resource ? "res" : "?") + ":" + (o ? "obj" : "null") + ":" + std::to_string(v));
    return 0;
}
int main() {
    int bad = 0;
    char dest[4];
    jumpGateInit__FiPcc(0x1ff, dest, 1);
    const char* want[] = {"task:run:20:jumpgate:0", "task:draw:20:jumpgatedraw:1", "kick:0", "find:fx.lmp:spel_gatescroll.anm", "play:res:null:1.000000"};
    if (events.size() != 5) { std::puts("fail: event count"); bad = 1; }
    for (size_t i = 0; i < events.size() && i < 5; ++i)
        if (events[i] != want[i]) { std::printf("fail: event %zu %s\n", i, events[i].c_str()); bad = 1; }
    if (jumpGateWho != -1 || jumpGateHurtDest != dest || jumpHidden != 1 || jumpGateState != 0 || jumping != 0) { std::puts("fail: stores"); bad = 1; }
    if (!bad) std::puts("jumpGateInit ok");
    return bad;
}
