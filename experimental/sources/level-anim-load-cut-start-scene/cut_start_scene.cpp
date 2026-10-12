#include "../level-anim-load-common/mem.hpp"
// Observed cutStartScene__FP9_cutscenePc at 0x0018e1c0, 320 bytes. The first word of the
// scene is read: when its bit 1 (0x2) is clear coreStartLoading runs. netWeSentReady and
// netNmReady are cleared before that test, then gameNetKick(0) is called. The scene
// pointer is published to cutCurrentScene and the cutscene state is reset: cutInit 0,
// displayActionPos 0, displayCutPos 0, displayFrame 1, cutTime 0, cutMode 1,
// cutPlayMode 1, cutSkipMode 0, dialogActor 0, musicSize 0. worldSetFarClip(1800.0) is
// called, then gameControlPaused = 60, Camera::SetMode(gameCam, 0) and hudNoDraw = 1.
// The frustum is worldSetFrustum(50.0, 1.33 (bits 0x3faa3d70), 80.0, f) with f 1.0 when
// frontEndRunning is nonzero and 0.75 otherwise. The name argument is copied to
// cutSceneName (0x00790f10; the next symbol, anim, starts at 0x00790f60), and
// two engine tasks are added at priority 300: cutRunFrameTask__Fv named "cutRunFrame" with
// flag 0, then cutDrawFrame__Fv named "cutDrawFrame" with flag 1 (names read from 0x00463fb0
// and 0x00463fc0).
// Inferred: the first scene word is a flag word; bit 1 means the scene's data is already
// loaded, since the loading call is skipped. Names of the state words are the original
// symbols; their types are guessed from the store widths only.
// Unknown: the cutTime and dialogActor word types, and whether the 1.33 value is
// intended as a 4:3 aspect.
extern "C" {
extern int netWeSentReady;
extern int netNmReady;
extern void* cutCurrentScene;
extern unsigned char cutInit;
extern int displayActionPos;
extern int displayCutPos;
extern int displayFrame;
extern int cutTime;
extern unsigned char cutMode;
extern unsigned char cutPlayMode;
extern unsigned char cutSkipMode;
extern int dialogActor;
extern int musicSize;
extern unsigned gameControlPaused;
extern void* gameCam;
extern unsigned char hudNoDraw;
extern int frontEndRunning;
extern char cutSceneName[];
void coreStartLoading__Fv();
void gameNetKick__Fi(int reason);
void worldSetFarClip__Ff(float distance);
void SetMode__6Camerai(void* camera, int mode);
void worldSetFrustum__Fffff(float first, float second, float third, float fourth);
char* strcpy(char* destination, const char* source);
void engineAddTask__FPFv_viPci(void (*callback)(), int priority, char* name, int flag);
void cutRunFrameTask__Fv();
void cutDrawFrame__Fv();
}
namespace {
float from_bits(unsigned bits) {
    unsigned char bytes[4];
    norrath_lal::store_word(bytes, bits);
    return norrath_lal::load_float(bytes);
}
}
extern "C" void cutStartScene__FP9_cutscenePc(unsigned char* scene, char* name) {
    using namespace norrath_lal;
    netWeSentReady = 0;
    netNmReady = 0;
    if ((load_word(scene) & 2u) == 0) coreStartLoading__Fv();
    gameNetKick__Fi(0);
    cutCurrentScene = scene;
    cutInit = 0;
    displayActionPos = 0;
    displayCutPos = 0;
    displayFrame = 1;
    cutTime = 0;
    cutMode = 1;
    cutPlayMode = 1;
    cutSkipMode = 0;
    dialogActor = 0;
    musicSize = 0;
    worldSetFarClip__Ff(from_bits(0x44e10000u));
    gameControlPaused = 60;
    SetMode__6Camerai(gameCam, 0);
    hudNoDraw = 1;
    const float fourth = frontEndRunning != 0 ? from_bits(0x3f800000u) : from_bits(0x3f400000u);
    worldSetFrustum__Fffff(from_bits(0x42480000u), from_bits(0x3faa3d70u),
                           from_bits(0x42a00000u), fourth);
    strcpy(cutSceneName, name);
    engineAddTask__FPFv_viPci(cutRunFrameTask__Fv, 300, const_cast<char*>("cutRunFrame"), 0);
    engineAddTask__FPFv_viPci(cutDrawFrame__Fv, 300, const_cast<char*>("cutDrawFrame"), 1);
}
