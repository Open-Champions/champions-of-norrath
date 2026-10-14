// Observed: Init__7VISetupi at 0x0114f7a0 is 964 bytes. Arguments are (this, load options flag).
// It stores -1 at this + 0x153c4 and 0 at this + 0x153dc, prints "INIT\n" and initializes, in
// order, with early return of -1 on any negative result: Init__5VIPadii(this + 0xae58, 0, 0)
// (result unused); Init__8VIRasterPviii(this, 0, 640, 448, 1); Init__9VICollide(this + 0x4c60);
// Init__12VIDictionaryi(this + 0x5080, 0); Init__13VISoundDevice(this + 0x5098); then
// Init__7VIScenelR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(this + 0x6220,
// VITimer::GetTime(), dictionary, this, sound device, collide). A nonzero flag then calls
// VIGetGameOptions__Fv() and Load__13VIGameOptions on its result. Init__4VIUI...(this + 0xade0,
// this, sound device, dictionary) must not be negative; VISetWindowUI__FP4VIUI(this + 0xade0) runs.
// Two stack objects are constructed (__10VIESFParse and __9VICSFFile, the first at stack
// offset 0 for at most 1072 bytes, the second at offset 1072). Parse__9VICSFFile... is called on
// the scene with the paths "data2\\ArialUI.csf", then (after VIDisplayLoadingScreen__Fb(1))
// "data2\\ui.csf", "data2\\conui.csf", "data2\\statui.csf" and "data2\\setup.csf"; any negative
// result destroys the parse object with _$_10VIESFParse(object, 2) and returns -1.
// On success: Flush__13VISoundDevice, SetTimeOfDay__7VIScenef(scene, 12.0f),
// SetWireframe__8VIRasteri(this, 0), SetClearColor with a stack color of floats 0, 0, 0, 1,
// VISplashFxInit__FP8VIRasterP12VIDictionary(this, dictionary), a zero stored at this + 0xaf60,
// Init__15VIWindowManagerP5VIPadP10VIKeyboard(this + 0xaf64, pad, 0), Init__11VIWndSplashP7VIScene
// (this + 0xafe8, scene), SetAppPtr__5VIWndPv(this + 0xafe8, this), Init__12VIWndPatcher
// (this + 0xd5b0), Init__9VIWndDnasP7VIScene(this + 0x12060, scene), Init__10VIWndLegal
// (this + 0x15390), Init__9VIWndEula(this + 0x12d68), zero at this + 0x153cc, SetRoot and SetFocus(…, 1)
// of the window manager onto the legal window, Init__7VIFaderP8VIRasterP12VIDictionaryf
// (this + 0xae38, this, dictionary, 1.0f), Begin__7VIFaderff(fader, 0.0f, 0.5f), 1 at
// this + 0x153c8, -1 at this + 0x153e0, 0 at this + 0x153c0, the pad pointer stored in sPad, and
// _$_10VIESFParse(object, 2). It returns 0.
// Inferred: the sub-object offsets are those the matching Free/destructor routines use; the
// result is 0 for success and -1 for failure.
// Unknown: the member types, the stack object sizes, and the meaning of the words at
// this + 0x153c0 to 0x153e0.

extern "C" {
int printf(const char* format, ...);
int Init__5VIPadii(void* pad, int a, int b);
int Init__8VIRasterPviii(void* raster, void* surface, int width, int height, int flag);
int Init__9VICollide(void* collide);
int Init__12VIDictionaryi(void* dictionary, int value);
int Init__13VISoundDevice(void* sound);
long GetTime__7VITimer();
int Init__7VIScenelR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(void* scene, long time, void* dictionary,
                                                                        void* raster, void* sound, void* collide);
void* VIGetGameOptions__Fv();
void Load__13VIGameOptions(void* options);
int Init__4VIUIP8VIRasterP13VISoundDeviceP12VIDictionary(void* ui, void* raster, void* sound, void* dictionary);
void VISetWindowUI__FP4VIUI(void* ui);
void* __10VIESFParse(void* object);
void* __9VICSFFile(void* object);
int Parse__9VICSFFilePCcR10VIESFParseR7VIScene(void* file, const char* path, void* parse, void* scene);
void _$_10VIESFParse(void* object, int flags);
void VIDisplayLoadingScreen__Fb(bool flag);
void Flush__13VISoundDevice(void* sound);
void SetTimeOfDay__7VIScenef(void* scene, float time);
void SetWireframe__8VIRasteri(void* raster, int value);
void SetClearColor__8VIRasterRC10VIColor32F(void* raster, void* color);
void VISplashFxInit__FP8VIRasterP12VIDictionary(void* raster, void* dictionary);
void Init__15VIWindowManagerP5VIPadP10VIKeyboard(void* manager, void* pad, void* keyboard);
void Init__11VIWndSplashP7VIScene(void* window, void* scene);
void SetAppPtr__5VIWndPv(void* window, void* application);
void Init__12VIWndPatcher(void* window);
void Init__9VIWndDnasP7VIScene(void* window, void* scene);
void Init__10VIWndLegal(void* window);
void Init__9VIWndEula(void* window);
void SetRoot__15VIWindowManagerP5VIWnd(void* manager, void* window);
void SetFocus__5VIWndP5VIWndb(void* manager, void* window, bool flag);
void Init__7VIFaderP8VIRasterP12VIDictionaryf(void* fader, void* raster, void* dictionary, float value);
void Begin__7VIFaderff(void* fader, float from, float to);
extern void* sPad;
}

namespace {
bool parse_failed(void* file, const char* path, void* parse, void* scene) {
    return Parse__9VICSFFilePCcR10VIESFParseR7VIScene(file, path, parse, scene) < 0;
}
}

extern "C" int Init__7VISetupi(void* self, int load_options) {
    unsigned char* const base = static_cast<unsigned char*>(self);
    unsigned char parse_object[1072];
    unsigned char file_object[16];
    float color[4];
    *reinterpret_cast<int*>(base + 0x153c4) = -1;
    *reinterpret_cast<int*>(base + 0x153dc) = 0;
    printf("INIT\n");
    Init__5VIPadii(base + 0xae58, 0, 0);
    if (Init__8VIRasterPviii(self, 0, 640, 448, 1) < 0) {
        return -1;
    }
    void* const collide = base + 0x4c60;
    if (Init__9VICollide(collide) < 0) {
        return -1;
    }
    void* const dictionary = base + 0x5080;
    if (Init__12VIDictionaryi(dictionary, 0) < 0) {
        return -1;
    }
    void* const sound = base + 0x5098;
    if (Init__13VISoundDevice(sound) < 0) {
        return -1;
    }
    void* const scene = base + 0x6220;
    if (Init__7VIScenelR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(scene, GetTime__7VITimer(), dictionary, self,
                                                                            sound, collide) < 0) {
        return -1;
    }
    if (load_options != 0) {
        Load__13VIGameOptions(VIGetGameOptions__Fv());
    }
    void* const ui = base + 0xade0;
    if (Init__4VIUIP8VIRasterP13VISoundDeviceP12VIDictionary(ui, self, sound, dictionary) < 0) {
        return -1;
    }
    VISetWindowUI__FP4VIUI(ui);
    __10VIESFParse(parse_object);
    __9VICSFFile(file_object);
    if (parse_failed(file_object, "data2\\ArialUI.csf", parse_object, scene)) {
        _$_10VIESFParse(parse_object, 2);
        return -1;
    }
    VIDisplayLoadingScreen__Fb(true);
    if (parse_failed(file_object, "data2\\ui.csf", parse_object, scene) ||
        parse_failed(file_object, "data2\\conui.csf", parse_object, scene) ||
        parse_failed(file_object, "data2\\statui.csf", parse_object, scene) ||
        parse_failed(file_object, "data2\\setup.csf", parse_object, scene)) {
        _$_10VIESFParse(parse_object, 2);
        return -1;
    }
    void* const manager = base + 0xaf64;
    void* const pad = base + 0xae58;
    void* const splash = base + 0xafe8;
    void* const legal = base + 0x15390;
    void* const fader = base + 0xae38;
    Flush__13VISoundDevice(sound);
    SetTimeOfDay__7VIScenef(scene, 12.0f);
    SetWireframe__8VIRasteri(self, 0);
    color[0] = 0.0f;
    color[1] = 0.0f;
    color[2] = 0.0f;
    color[3] = 1.0f;
    SetClearColor__8VIRasterRC10VIColor32F(self, color);
    VISplashFxInit__FP8VIRasterP12VIDictionary(self, dictionary);
    *reinterpret_cast<int*>(base + 0xaf60) = 0;
    Init__15VIWindowManagerP5VIPadP10VIKeyboard(manager, pad, 0);
    Init__11VIWndSplashP7VIScene(splash, scene);
    SetAppPtr__5VIWndPv(splash, self);
    Init__12VIWndPatcher(base + 0xd5b0);
    Init__9VIWndDnasP7VIScene(base + 0x12060, scene);
    Init__10VIWndLegal(legal);
    Init__9VIWndEula(base + 0x12d68);
    *reinterpret_cast<int*>(base + 0x153cc) = 0;
    SetRoot__15VIWindowManagerP5VIWnd(manager, legal);
    SetFocus__5VIWndP5VIWndb(manager, legal, true);
    Init__7VIFaderP8VIRasterP12VIDictionaryf(fader, self, dictionary, 1.0f);
    Begin__7VIFaderff(fader, 0.0f, 0.5f);
    *reinterpret_cast<int*>(base + 0x153c8) = 1;
    *reinterpret_cast<int*>(base + 0x153e0) = -1;
    *reinterpret_cast<int*>(base + 0x153c0) = 0;
    sPad = pad;
    _$_10VIESFParse(parse_object, 2);
    return 0;
}
