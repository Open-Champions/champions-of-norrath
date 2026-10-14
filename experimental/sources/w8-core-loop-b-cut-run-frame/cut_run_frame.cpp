// Observed cutRunFrame__Fb at 0x0018c8f8, 1200 bytes. The bool argument is kept in a callee-saved register and
// only tested in the frame-advance step below.
// Always: gameControlPaused = 60.
// Dialog animation load step, entered when displayDialogPos >= animationLoadPos and dialogPacket and dlgDir are
// both non-null:
//   - if the function-local static animationLoaded (local symbol animationLoaded.1097, 0x00484348, a byte) is set:
//     clear it, store animationLoadPos + 1 into animationLoadPos, clear animationLoading;
//   - otherwise, only when anim[0] (0x00790f60, eight-byte slots) has a nonzero first word, cdIsBusy() returns 0,
//     animationLoadPos < nmDialogNodes, and the second word of slot (animationLoadPos & 1) is zero: format
//     "%s.anm" (0x004843b8) with the text at dialogPacket + 1096 * animationLoadPos + 49 into an 80-byte local
//     name. Then scan dlgDir (80-byte entries, ending at an entry whose first byte is zero) for the first entry
//     whose text equals the name. A match loads the file: cdLoadAsync(soundGetDialogFilename(), slot block,
//     &animationLoaded, entry[+144] - entry[+64], entry[+64]) where those two fields are read from the entry at
//     the match and the entry after it, sets animationLoading = 1 and the slot's second word = 1. After a
//     match, or when no entry matched or dlgDir starts with a zero byte, if the entry byte at the final index is
//     zero (an unmatched list, or an empty name) animationLoaded is set to 1 and the slot's second word to 0.
// Then: lightGlobalCharLightScale((float)scene[+112] * 0.1f) with scene = cutCurrentScene; if cutInit is zero,
// cutInitScene(cutCurrentScene) and cutInit = 1; t = (float)displayFrame * (1/60); when (coreLoading == 0 or
// netNetworkMode != 0) and cutSkipMode == 0 and the argument is false, t = advanceFrame(t); setCamera(t).
// Frustum: with frontEndRunning set, worldSetFrustum(myFOV, 1.33, myFOV * 1.6, 1.0); otherwise
// worldSetFrustum(myFOV, 1.33, myFOV * 1.6, 0.75). Both decimal values are the file floats noted below.
// Then a 48-byte matrix is set to identity and rotated (axis, angle): (0, 16339), (1, myRoll), (0, myPitch),
// (2, myYaw), and worldSetViewMatricies(&cutCameraPos, matrix) and worldSetViewCenter() are called.
// If coreLoadingScreenRunning is nonzero the function ends there.
// Skip detection, when frontEndRunning is zero and nmPlayers > 0: cutSkipMode becomes 1 if any of the nmPlayers
// 88-byte records of pad, whose word at +12 is tested, has bit 0x800 set (otherwise it is unchanged).
// If cutSkipMode is nonzero: engineSyncVBlank, engineSetDisplayDisable(1), cutSkipMode = 1, soundResetStream(2),
// and soundResetStream(0) when stream 0 status has 0x80 set and 0x100 clear. While displayActionPos != nmActions,
// advanceFrame((float)displayFrame * (1/60)) is called repeatedly.
// Finish, when displayActionPos == nmActions (checked after the above): with frontEndRunning set, displayFrame,
// displayActionPos and cutTime are zeroed and the function ends. Otherwise cutStop(); when (flags & 4) == 0 or
// cutSkipMode != 0, engineDisplayDisable = 3 and coreStartLoading(); then gameCam->SetMode(1), hudNoDraw = 0,
// gameCam->SetCutWasActive(), scriptActivatePostAction(), and eRandState = frameNm. When the positions differ,
// only eRandState = frameNm is stored.
// Float values: 0.1, 1/60, 1.6 and 1.33 are the file words 0x3dcccccc, 0x3c888888, 0x3fcccccc and 0x3faa3d70,
// one unit below the nearest float of each decimal, so hexadecimal literals are used. The 0.75 and 1.0 are exact.
// Inferred: advanceFrame returns a float (its result feeds setCamera); the +112 word of the scene is an integer.
// Unknown: the layout of the dialog packet record, of dlgDir entries beyond +0, +64 and +144, the scene record
// beyond +112, and the meaning of 16339 and of flags bit 4.
struct CutAnimSlot {
    void* block;
    int second;
};
struct CutPadRecord {
    unsigned char before[12];
    unsigned buttons;
    unsigned char after[72];
};

extern "C" int gameControlPaused;
extern "C" int animationLoadPos;
extern "C" int displayDialogPos;
extern "C" unsigned char* dialogPacket;
extern "C" char* dlgDir;
extern "C" int nmDialogNodes;
extern "C" unsigned char animationLoading;
extern "C" CutAnimSlot anim[2];
extern "C" unsigned char* cutCurrentScene;
extern "C" unsigned char cutInit;
extern "C" int displayFrame;
extern "C" int coreLoading;
extern "C" signed char netNetworkMode;
extern "C" unsigned char cutSkipMode;
extern "C" int frontEndRunning;
extern "C" float myFOV;
extern "C" short myRoll;
extern "C" short myPitch;
extern "C" short myYaw;
extern "C" unsigned char cutCameraPos[12];
extern "C" signed char coreLoadingScreenRunning;
extern "C" int nmPlayers;
extern "C" CutPadRecord pad[];
extern "C" int displayActionPos;
extern "C" int nmActions;
extern "C" int cutTime;
extern "C" int flags;
extern "C" int engineDisplayDisable;
extern "C" unsigned char hudNoDraw;
extern "C" int eRandState;
extern "C" int frameNm;
extern "C" void* gameCam;

extern "C" int cdIsBusy__Fv();
extern "C" int sprintf(char* to, const char* format, ...);
extern "C" int strcmp(const char* a, const char* b);
extern "C" char* soundGetDialogFilename__Fv();
extern "C" void cdLoadAsync__FPcPvT0ii(char* name, void* destination, void* flag, int size, int offset);
extern "C" void lightGlobalCharLightScale__Ff(float scale);
extern "C" void cutInitScene__FP9_cutscene(void* scene);
extern "C" float advanceFrame__Ff(float step);
extern "C" void setCamera__Ff(float step);
extern "C" void worldSetFrustum__Fffff(float first, float second, float third, float fourth);
extern "C" void matIdent__FP8Matrix34(void* matrix);
extern "C" void matRot__FP8Matrix34is(void* matrix, int axis, short angle);
extern "C" void worldSetViewMatricies__FP6Point3P8Matrix34(void* position, void* matrix);
extern "C" void worldSetViewCenter__Fv();
extern "C" void engineSyncVBlank__Fv();
extern "C" void engineSetDisplayDisable__Fi(int value);
extern "C" void soundResetStream__Fi(int stream);
extern "C" int soundGetStreamPlayStatus__Fi(int stream);
extern "C" void cutStop__Fv();
extern "C" void coreStartLoading__Fv();
extern "C" void SetMode__6Camerai(void* camera, int mode);
extern "C" void SetCutWasActive__6Camera(void* camera);
extern "C" void scriptActivatePostAction__Fv();

extern "C" void cutRunFrame__Fb(bool noAdvance) {
    static char animationLoaded;
    int position = animationLoadPos;
    gameControlPaused = 60;
    if (!(displayDialogPos < position) && dialogPacket && dlgDir) {
        if (animationLoaded) {
            animationLoaded = 0;
            animationLoadPos = position + 1;
            animationLoading = 0;
        } else if (anim[0].block && !cdIsBusy__Fv() && animationLoadPos < nmDialogNodes &&
                   anim[animationLoadPos & 1].second == 0) {
            char name[80];
            int index = 0;
            sprintf(name, "%s.anm", dialogPacket + animationLoadPos * 1096 + 49);
            if (dlgDir[0]) {
                for (;;) {
                    if (strcmp(name, dlgDir + index * 80) == 0) {
                        char* entry = dlgDir + index * 80;
                        int offset = *(int*)(entry + 64);
                        int next = *(int*)(entry + 144);
                        char* file = soundGetDialogFilename__Fv();
                        cdLoadAsync__FPcPvT0ii(file, anim[animationLoadPos & 1].block, &animationLoaded,
                                               next - offset, offset);
                        animationLoading = 1;
                        anim[animationLoadPos & 1].second = 1;
                        break;
                    }
                    ++index;
                    if (!dlgDir[index * 80]) break;
                }
            }
            if (dlgDir[index * 80] == 0) {
                animationLoaded = 1;
                anim[animationLoadPos & 1].second = 0;
            }
        }
    }

    lightGlobalCharLightScale__Ff((float)*(int*)(cutCurrentScene + 112) * 0x1.999998p-4f);
    if (!cutInit) {
        cutInitScene__FP9_cutscene(cutCurrentScene);
        cutInit = 1;
    }
    float step = (float)displayFrame * 0x1.111110p-6f;
    if ((coreLoading == 0 || netNetworkMode != 0) && cutSkipMode == 0 && !noAdvance)
        step = advanceFrame__Ff(step);
    setCamera__Ff(step);

    float fov = myFOV;
    if (frontEndRunning)
        worldSetFrustum__Fffff(fov, 0x1.547ae0p+0f, fov * 0x1.999998p+0f, 1.0f);
    else
        worldSetFrustum__Fffff(fov, 0x1.547ae0p+0f, fov * 0x1.999998p+0f, 0.75f);

    unsigned char matrix[48];
    matIdent__FP8Matrix34(matrix);
    matRot__FP8Matrix34is(matrix, 0, 16339);
    matRot__FP8Matrix34is(matrix, 1, myRoll);
    matRot__FP8Matrix34is(matrix, 0, myPitch);
    matRot__FP8Matrix34is(matrix, 2, myYaw);
    worldSetViewMatricies__FP6Point3P8Matrix34(cutCameraPos, matrix);
    worldSetViewCenter__Fv();
    if (coreLoadingScreenRunning) return;

    if (!frontEndRunning && nmPlayers > 0) {
        unsigned char skip = cutSkipMode;
        for (int player = 0; player < nmPlayers; ++player)
            if (pad[player].buttons & 0x800) skip = 1;
        cutSkipMode = skip;
    }
    if (cutSkipMode) {
        engineSyncVBlank__Fv();
        engineSetDisplayDisable__Fi(1);
        cutSkipMode = 1;
        soundResetStream__Fi(2);
        if ((soundGetStreamPlayStatus__Fi(0) & 0x80) && !(soundGetStreamPlayStatus__Fi(0) & 0x100))
            soundResetStream__Fi(0);
        while (displayActionPos != nmActions)
            advanceFrame__Ff((float)displayFrame * 0x1.111110p-6f);
    }
    if (displayActionPos == nmActions) {
        if (frontEndRunning) {
            displayFrame = 0;
            displayActionPos = 0;
            cutTime = 0;
            return;
        }
        cutStop__Fv();
        if ((flags & 4) == 0 || cutSkipMode) {
            engineDisplayDisable = 3;
            coreStartLoading__Fv();
        }
        SetMode__6Camerai(gameCam, 1);
        hudNoDraw = 0;
        SetCutWasActive__6Camera(gameCam);
        scriptActivatePostAction__Fv();
    }
    eRandState = frameNm;
}
