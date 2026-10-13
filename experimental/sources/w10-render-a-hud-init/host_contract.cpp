// Host contract for HUDInit__Fv. Not an EE plan input.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
static std::vector<std::string> events;
extern "C" {
void HUDRunFrame__Fv() {}
void HUDDrawFrame__Fv() {}
char* crystalTex[4];
char tokens[16];
float bustAlpha = 0.25f;
unsigned char hudBustCovered = 9;
unsigned char hudNetWaiting = 9;
void engineAddTask__FPFv_viPci(void (*callback)(), int priority, char* name, int flag) {
    events.push_back(std::string("task ") + (callback == HUDRunFrame__Fv ? "run" : callback == HUDDrawFrame__Fv ? "draw" : "?") +
                     " " + std::to_string(priority) + " " + name + " " + std::to_string(flag));
}
char* lumpFindResource__FPcT0(char* lump, char* name) {
    events.push_back(std::string("find ") + lump + " " + name);
    return tokens + events.size();
}
}
#include "hud_init.cpp"
int main() {
    HUDInit__Fv();
    const char* want[] = {"task run 50 HUD Run 0", "task draw 51 HUD Draw 1", "find hud.lmp playerred.tex",
                          "find hud.lmp playergreen.tex", "find hud.lmp playeryellow.tex", "find hud.lmp playerblue.tex"};
    if (events.size() != 6) return 1;
    for (int i = 0; i != 6; ++i) if (events[i] != want[i]) { std::printf("event %d: %s\n", i, events[i].c_str()); return 2; }
    for (int i = 0; i != 4; ++i) if (crystalTex[i] != tokens + 3 + i) return 3;
    if (bustAlpha != 1.0f || hudBustCovered != 0 || hudNetWaiting != 0) return 4;
    std::puts("HUDInit contract passed");
    return 0;
}
