// Observed Update__6Camera at 0x0026bfe0, 284 bytes.
// First clears the byte hudDisplayNetCamToggle (0x00484b01). Then sets the
// word at camera+96: zero when gameIsPaused__Fv is nonzero, or the word
// `world` (0x004844a0) is zero, or gameNoDraw (0x00483f18) or coreLoading
// (0x00486680) is nonzero, or the signed byte hudNetWaiting (0x00484b00) is
// nonzero, or DLG_Active__Fv is nonzero, or gameControlPaused (0x00483f1c) is
// nonzero. These are tested in that order and later ones are not called once
// one holds. Otherwise the word is 1 exactly when playerLocalPlayerBusy__Fv
// returns zero. Then, with mode = word at camera+52 and state = word at
// camera+72: mode 1 or a nonzero state runs doNormalCam__6Camera, after which
// a state that is exactly 1 becomes 2 unless the mode is 1. A zero state with
// mode 3, 4 or 5 runs doDebugCam__6Camera; with mode 2 it runs
// doInnorukCam__6Camera; with any other mode nothing more happens. Nothing is
// returned. Nothing here was executed on the EE.
extern "C" {
int gameIsPaused__Fv();
int DLG_Active__Fv();
int playerLocalPlayerBusy__Fv();
void doNormalCam__6Camera(void* camera);
void doDebugCam__6Camera(void* camera);
void doInnorukCam__6Camera(void* camera);
extern signed char hudDisplayNetCamToggle;
extern signed char hudNetWaiting;
extern void* world;
extern int gameNoDraw;
extern int coreLoading;
extern int gameControlPaused;
}

typedef int __attribute__((may_alias)) AliasedInt;

extern "C" void Update__6Camera(unsigned char* camera) {
    hudDisplayNetCamToggle = 0;
    int* active = reinterpret_cast<AliasedInt*>(camera + 96);
    if (gameIsPaused__Fv() != 0) *active = 0;
    else if (world == nullptr) *active = 0;
    else if (gameNoDraw != 0) *active = 0;
    else if (coreLoading != 0) *active = 0;
    else if (hudNetWaiting != 0) *active = 0;
    else if (DLG_Active__Fv() != 0) *active = 0;
    else if (gameControlPaused != 0) *active = 0;
    else *active = playerLocalPlayerBusy__Fv() == 0 ? 1 : 0;

    const int mode = *reinterpret_cast<AliasedInt*>(camera + 52);
    int* state = reinterpret_cast<AliasedInt*>(camera + 72);
    if (mode == 1 || *state != 0) {
        doNormalCam__6Camera(camera);
        if (*state == 1 && *reinterpret_cast<AliasedInt*>(camera + 52) != 1) *state = 2;
    } else if (static_cast<unsigned>(mode - 3) < 3u) {
        doDebugCam__6Camera(camera);
    } else if (mode == 2) {
        doInnorukCam__6Camera(camera);
    }
}
