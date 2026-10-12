#include "player_selection_start.cpp"
#include <cstdio>
#include <cstring>

unsigned char firstTime;
Camera* gameCam;
signed char language;
int playerSelRunning;
char playerSelectSoundLump[32];

static char trace[16][48];
static int steps;
static int lightArgs[9];
static float lightDir[3];
static char loadedLump[2][32];
static int lumpCount;
static Camera* modeCamera;
static int modeValue;
static void (*taskFn)();
static int taskPriority, taskFlag;
static char taskName[32];
static int fadeResetArg = -1, fadeArg = -1;

static void note(const char* s) { std::snprintf(trace[steps++], sizeof trace[0], "%s", s); }

extern "C" void hudResetFadeScreen__Fi(int v) { note("reset"); fadeResetArg = v; }
extern "C" void hudFadeScreen__Fi(int v) { note("fade"); fadeArg = v; }
extern "C" void SetMode__6Camerai(Camera* c, int m) { note("mode"); modeCamera = c; modeValue = m; }
extern "C" void lightPushSceneLights__FG6Point3iiiiiiiii(const Point3* d, int a, int b, int c, int e,
                                                         int f, int g, int h, int i, int j) {
    note("light");
    lightDir[0] = d->x; lightDir[1] = d->y; lightDir[2] = d->z;
    const int args[9] = {a, b, c, e, f, g, h, i, j};
    std::memcpy(lightArgs, args, sizeof args);
}
extern "C" void lumpLoad__FPc(char* n) {
    note("lump");
    std::snprintf(loadedLump[lumpCount < 2 ? lumpCount : 1], 32, "%s", n);
    ++lumpCount;
}
extern "C" void engineAddTask__FPFv_viPci(void (*cb)(), int p, char* n, int f) {
    note("task");
    taskFn = cb; taskPriority = p; taskFlag = f;
    std::snprintf(taskName, sizeof taskName, "%s", n);
}
extern "C" void playerSelectionTask__Fv() {}

static int run(int lang, const char* want) {
    steps = lumpCount = 0;
    std::memset(playerSelectSoundLump, 'x', sizeof playerSelectSoundLump);
    firstTime = 0;
    playerSelRunning = 0;
    Camera* const cam = reinterpret_cast<Camera*>(&firstTime);
    gameCam = cam;
    language = static_cast<signed char>(lang);
    playerSelectionStart__Fv();
    static const char* order[] = {"reset", "fade", "mode", "light", "lump", "lump", "task"};
    if (steps != 7) return 1;
    for (int i = 0; i < 7; ++i)
        if (std::strcmp(trace[i], order[i]) != 0) return 2;
    if (firstTime != 1 || playerSelRunning != 1) return 3;
    if (fadeResetArg != 1 || fadeArg != 0) return 4;
    if (modeCamera != cam || modeValue != 0) return 5;
    if (lightDir[0] != 0.0f || lightDir[1] != 0.0f || lightDir[2] != -1.0f) return 6;
    static const int expected[9] = {128, 0, 0, 128, 0, 0, 0, 0, 128};
    if (std::memcmp(lightArgs, expected, sizeof expected) != 0) return 7;
    if (std::strcmp(loadedLump[0], "interface.lmp") != 0) return 8;
    if (std::strcmp(loadedLump[1], want) != 0) return 9;
    if (std::strcmp(playerSelectSoundLump, want) != 0 || playerSelectSoundLump[11] != 'x') return 10;
    if (taskFn != playerSelectionTask__Fv || taskPriority != 9 || taskFlag != 1) return 11;
    if (std::strcmp(taskName, "playerSelection") != 0) return 12;
    return 0;
}

int main() {
    int rc;
    if ((rc = run(3, "sel_fr.lmp"))) return rc;
    if ((rc = run(4, "sel_ge.lmp"))) return 20 + rc;
    if ((rc = run(0, "sel_en.lmp"))) return 40 + rc;
    if ((rc = run(2, "sel_en.lmp"))) return 60 + rc;
    if ((rc = run(-3, "sel_en.lmp"))) return 80 + rc;
    return 0;
}
