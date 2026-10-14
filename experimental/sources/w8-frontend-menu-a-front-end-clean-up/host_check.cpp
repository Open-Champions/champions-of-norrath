#include <cstdio>
extern "C" {
struct Camera { int mode; };
Camera cam;
Camera* gameCam = &cam;
int frontEndRunning;
unsigned char frontEndSuspend, hudNoDraw, attemptLoadGame;
int order[8], count;
void (*removed)();
void cutStop__Fv() { order[count++] = 1; }
void runFrontEnd__Fv() {}
void engineRemoveTask__FPFv_v(void (*task)()) { order[count++] = 2; removed = task; }
void SetMode__6Camerai(Camera* c, int m) { order[count++] = 3; c->mode = m; }
}
#include "clean_up.cpp"
static int run(int arg) {
    count = 0; removed = 0; cam.mode = -1;
    frontEndRunning = 7; hudNoDraw = 9; attemptLoadGame = 5; frontEndSuspend = 0xaa;
    frontEndCleanUp__Fi(arg);
    if (count != 3 || order[0] != 1 || order[1] != 2 || order[2] != 3) return 1;
    if (removed != runFrontEnd__Fv || cam.mode != 1) return 2;
    if (frontEndRunning != 0 || hudNoDraw != 0 || attemptLoadGame != 0) return 3;
    if (frontEndSuspend != static_cast<unsigned char>(arg)) return 4;
    return 0;
}
int main() {
    int values[] = {0, 1, 2, 255, 256, -1};
    for (int v : values) if (int r = run(v)) { std::printf("fail %d at %d\n", r, v); return r; }
    std::puts("frontEndCleanUp__Fi check passed");
    return 0;
}
