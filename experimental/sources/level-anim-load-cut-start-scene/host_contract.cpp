#include "cut_start_scene.cpp"
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
#include <string>
static std::string trace;
static int bad;
extern "C" {
int netWeSentReady = 9, netNmReady = 9, displayActionPos = 9, displayCutPos = 9, displayFrame = 9,
    cutTime = 9, dialogActor = 9, musicSize = 9, frontEndRunning = 0;
void* cutCurrentScene = nullptr;
unsigned char cutInit = 9, cutMode = 9, cutPlayMode = 9, cutSkipMode = 9, hudNoDraw = 9;
unsigned gameControlPaused = 9;
void* gameCam = nullptr;
char cutSceneName[80];
void coreStartLoading__Fv() { trace += "load;"; if (netNmReady != 0 || netWeSentReady != 0) bad |= 1; }
void gameNetKick__Fi(int reason) { trace += "kick" + std::to_string(reason) + ";"; }
void worldSetFarClip__Ff(float d) { trace += "far" + std::to_string(int(d)) + ";"; if (d != 1800.0f) bad |= 2; }
void SetMode__6Camerai(void* camera, int mode) {
    trace += "mode" + std::to_string(mode) + ";"; if (camera != gameCam) bad |= 4;
    if (gameControlPaused != 60) bad |= 8;
}
void worldSetFrustum__Fffff(float a, float b, float c, float d) {
    unsigned bb; std::memcpy(&bb, &b, 4);
    trace += "frustum;"; if (a != 50.0f || bb != 0x3faa3d70u || c != 80.0f) bad |= 16;
    if (d != (frontEndRunning ? 1.0f : 0.75f)) bad |= 32;
    if (hudNoDraw != 1) bad |= 64;
}
char* strcpy(char* d, const char* s) { trace += "copy;"; char* r = d; while ((*d++ = *s++)) {} return r; }
void cutRunFrameTask__Fv() {}
void cutDrawFrame__Fv() {}
static int tasks;
void engineAddTask__FPFv_viPci(void (*cb)(), int prio, char* name, int flag) {
    trace += std::string("task:") + name + ":" + std::to_string(prio) + ":" + std::to_string(flag) + ";";
    if (tasks++ == 0 ? cb != cutRunFrameTask__Fv : cb != cutDrawFrame__Fv) bad |= 128;
}
}
static int run(unsigned scene_flags, int front_end, const char* expect_log) {
    unsigned char* scene = arena_at(0x100);
    sw_(scene, scene_flags);
    trace.clear(); bad = 0; tasks = 0; frontEndRunning = front_end;
    static unsigned char camera[4]; gameCam = camera;
    netWeSentReady = netNmReady = 5;
    char name[] = "gate.cut";
    cutStartScene__FP9_cutscenePc(scene, name);
    if (trace != expect_log) { std::printf("trace %s\n", trace.c_str()); return 1; }
    if (bad) { std::printf("bad %d\n", bad); return 2; }
    if (cutCurrentScene != scene || netWeSentReady || netNmReady || cutInit || displayActionPos ||
        displayCutPos || displayFrame != 1 || cutTime || cutMode != 1 || cutPlayMode != 1 ||
        cutSkipMode || dialogActor || musicSize || gameControlPaused != 60 || hudNoDraw != 1) return 3;
    if (std::strcmp(cutSceneName, "gate.cut") != 0) return 4;
    return 0;
}
int main() {
    const char* tail = "kick0;far1800;mode0;frustum;copy;task:cutRunFrame:300:0;task:cutDrawFrame:300:1;";
    if (run(0, 0, (std::string("load;") + tail).c_str())) return 1;
    if (run(1, 1, (std::string("load;") + tail).c_str())) return 2;
    if (run(2, 0, tail)) return 3;
    if (run(3, 1, tail)) return 4;
    return 0;
}
