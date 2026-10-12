// Observed playerSelectionStart__Fv at 0x00279cb8, 384 bytes with padding.
// Steps in order:
//  1. firstTime (byte at 0x00486d88) is set to 1.
//  2. hudResetFadeScreen(1), then hudFadeScreen(0).
//  3. Camera::SetMode(gameCam, 0).
//  4. lightPushSceneLights is called with a Point3 of (0, 0, -1) and nine
//     integers: 128, 0, 0, 128, 0, 0, 0, 0, 128. The Point3 is passed by the
//     address of a stack copy, as the image does; it is not a by-value struct
//     in registers. What the nine integers mean is not established.
//  5. lumpLoad("interface.lmp").
//  6. The 11 bytes "sel_fr.lmp" are copied to playerSelectSoundLump when the
//     signed byte language is 3, "sel_ge.lmp" when it is 4, and "sel_en.lmp"
//     for every other value. That buffer is then passed to lumpLoad.
//  7. playerSelectionTask__Fv (0x0027a658) is added with priority 9, the name
//     "playerSelection" and flag 1.
//  8. playerSelRunning (word) is set to 1.
struct Camera;
struct Point3 {
    float x, y, z;
};

extern "C" unsigned char firstTime;
extern "C" Camera* gameCam;
extern "C" signed char language;
extern "C" int playerSelRunning;
extern "C" char playerSelectSoundLump[];

extern "C" void hudResetFadeScreen__Fi(int value);
extern "C" void hudFadeScreen__Fi(int value);
extern "C" void SetMode__6Camerai(Camera* camera, int mode);
extern "C" void lightPushSceneLights__FG6Point3iiiiiiiii(const Point3* direction, int a, int b, int c,
                                                         int d, int e, int f, int g, int h, int i);
extern "C" void lumpLoad__FPc(char* name);
extern "C" void engineAddTask__FPFv_viPci(void (*callback)(), int priority, char* name, int flag);
extern "C" void playerSelectionTask__Fv();

extern "C" void playerSelectionStart__Fv() {
    firstTime = 1;
    hudResetFadeScreen__Fi(1);
    hudFadeScreen__Fi(0);
    SetMode__6Camerai(gameCam, 0);

    Point3 direction;
    direction.x = 0.0f;
    direction.y = 0.0f;
    direction.z = -1.0f;
    lightPushSceneLights__FG6Point3iiiiiiiii(&direction, 128, 0, 0, 128, 0, 0, 0, 0, 128);

    lumpLoad__FPc(const_cast<char*>("interface.lmp"));

    if (language == 3) {
        __builtin_memcpy(playerSelectSoundLump, "sel_fr.lmp", 11);
    } else if (language == 4) {
        __builtin_memcpy(playerSelectSoundLump, "sel_ge.lmp", 11);
    } else {
        __builtin_memcpy(playerSelectSoundLump, "sel_en.lmp", 11);
    }
    lumpLoad__FPc(playerSelectSoundLump);

    engineAddTask__FPFv_viPci(playerSelectionTask__Fv, 9, const_cast<char*>("playerSelection"), 1);
    playerSelRunning = 1;
}
